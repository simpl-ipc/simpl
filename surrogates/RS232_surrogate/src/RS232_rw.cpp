/*******************************************************************************
FILE:			RS232_rw.cpp

DATE:			January 27, 2025

DESCRIPTION:	This program immediately forks into two separate
				programs: viz. rs232_r (port reader) and rs232_w (port writer).

AUTHOR:			John Collins

NOTES:			Make certain you have chmod a+rw /dev/ttyS* !!!
*******************************************************************************/

// application header
#include "RS232.h"

int main(int argc, char **argv)
{
pid_t childPid;
const std::string fn("rs232_rw");

// initialize
if (initialize(argc, argv) == -1)
	{
	sryLog("%s: cannot initialize.\n", fn.c_str());
	exit(EXIT_FAILURE);
	}

// fork off into surrogate receiver and sender parent processes 
childPid = fork();
if (childPid < 0) // failure
	sryLog("%s: cannot fork-%s.\n", fn.c_str(), strerror(errno));
else if (childPid == 0) // child
	rs232_r();
else // parent
	rs232_w();

return 0;
}

/**********************************************************************
FUNCTION:	initialize(int, char **argv)

PURPOSE:	set serial port parameters
  
RETURNS:	success: 0
			failure: -1
**********************************************************************/	

int initialize(int argc, char **argv)
{
// MemArea is global
// MemSize is global
// Verbose is global
// SerialDevice is global
// IntWidth is global
const std::string fn("rs232_initialize");
struct termios tio;
int fd, opt, var;

// set global memory size to a default 1K bytes
MemSize = 1024;

// initialize global memory
MemArea = (char *)malloc(MemSize);
if (!MemArea)
	{ 
	sryLog("%s: memory allocation error-%s.\n", fn.c_str(), strerror(errno));
	return -1;
	}

// set the default serial port
strcpy(SerialDevice, SerialDeviceDefault);

// set Verbosity off
Verbose = VerboseDefault;

// IntWidth is a global variable which is 2/4/8 for 16/32/64-bit respectively
IntWidth = IntWidthDefault;

// process command line arguments (if any)
while ((opt = getopt(argc, argv, "i:s:v")) != -1)
	{
	switch (opt)
		{
		case 'i': // integer width = 2/4/8 for 16/32/64-bit device/respectively
			var = atoi(optarg);
			if ((var == 2) || (var == 4) || (var == 8))
				IntWidth = var;
			break;

		case 's': // serial device
			strcpy(SerialDevice, optarg);
			break;

		case 'v': // verbosity
			Verbose = true; // turn on, off by default
			break;
		}
	}

// open the serial port
fd = open(SerialDevice, OPEN_FLAGS);
if (fd < 0)
	{
	sryLog("%s: cannot open %s for rw-%s\n", fn, SerialDevice, strerror(errno));
	return -1;
	}

// clear the new structure for port settings
bzero(&tio, sizeof(tio));

/*
set the control flags to the following defaults (defined in rs232.h):
1. hardware handshaking
2. 8 bits, no parity, 1 stop bit
3. local connection, ie. no modem
4. enable ability to receive characters
*/
tio.c_cflag |= CONTROL_MODE_FLAGS;

// set the input flags to ignore parity errors, otherwise raw
tio.c_iflag |= INPUT_MODE_FLAGS;

// set the output flags to raw
tio.c_oflag = OUTPUT_MODE_FLAGS;

// set the local flags to non-canonical, no echo etc.
tio.c_lflag = LOCAL_MODE_FLAGS;

/*
set the control characters to:
1. no intercharacter timer use
2. return after at least one byte
*/
tio.c_cc[VTIME] = 0;
tio.c_cc[VMIN] = 1;

// clear out
tcflush(fd, TCIFLUSH);

// set the new port settings
tcsetattr(fd, TCSANOW, &tio);

close(fd);

return 0;
}

/**********************************************************************
FUNCTION:	rs232_r(void)

PURPOSE:	receive messages via serial port and SIMPL send them to the
			appropriate surrogate.
  
RETURNS:	void
**********************************************************************/	

void rs232_r()
{
// MemArea is global
// MemSize is global
const std::string &n = RS232_READER;
int fd, id, numRead, numBytes, token, pid;
char receiver[MAX_PROGRAM_NAME_LEN + 1];
const int hdrSize = sizeof(SUR_MSG_HDR);
SUR_MSG_HDR hdr;

// create an instance of SRY called 'nee' with receiver name 'n'
SRY nee(n);

// open the serial port
fd = open(SerialDevice, OPEN_FLAGS);
if (fd <  0)
	{
	sryLog("%s: cannot open %s for rw-%s\n", n.c_str(), SerialDevice, 
														strerror(errno));
	exit(EXIT_FAILURE);
	}

while (true)
	{
	// all incoming messages start with a standard header
	numRead = readSerial(fd, hdrSize, (char *)&hdr);	
	if (numRead == -1)
		{
		sryLog("%s: problem reading header from %s-%s\n", n.c_str(),
											SerialDevice, strerror(errno));
		continue;
		}

	// check the header size
	if (numRead != hdrSize)
		{
		sryLog("%s: wrong header size=%d\n", n.c_str(), numRead);
		continue;
		}

	// interpret message header
	//numBytes = stobSI(hdr.nbytes, IntWidth);
	numBytes = atoi(hdr.nbytes);
	//token = stobSI(hdr.token, IntWidth);
	token = atoi(hdr.token);
	//pid = stobSI(hdr.surPid, IntWidth);
	pid = atoi(hdr.surPid);

	// adjust message memory size if necessary
	if ( (numBytes + hdrSize) > MemSize )
		{
		if (upMemory(numBytes + hdrSize) == -1)
			{
			sryLog("%s: cannot adjust memory-%s\n", n.c_str(), strerror(errno));
			exit(EXIT_FAILURE);
			}
		}

	// copy original header to the start of nmessage memory
	memcpy(MemArea, (char *)&hdr, hdrSize);

	// read the message content
	numRead = readSerial(fd, numBytes, MemArea + hdrSize);
	if (numRead == -1)
		{
		sryLog("%s: problem reading message content from %s-%s\n",
			n.c_str(), SerialDevice, strerror(errno));
		continue;
		}

	// check the size of the message
	if (numRead != numBytes)
		{
		sryLog("%s: wrong message size=%d\n", n.c_str(), numRead);
		continue;
		}

	if (Verbose)
		{
		switch (token)
			{
			case SUR_NAME_LOCATE:
				printf("%s: SUR_NAME_LOCATE\n", n.c_str());
				break;
			case SUR_REPLY:
				printf("%s: SUR_REPLY\n", n.c_str());
				break;
			case SUR_ERROR:
				printf("%s: SUR_ERROR\n", n.c_str());
				break;
			case SUR_ALIVE_REPLY:
				printf("%s: SUR_ALIVE_REPLY\n", n.c_str());
				break;
			case SUR_SEND:
				printf("%s: SUR_SEND\n", n.c_str());
				break;
			case SUR_PROXY:
				printf("%s: SUR_PROXY\n", n.c_str());
				break;
			case SUR_ALIVE:
				printf("%s: SUR_ALIVE\n", n.c_str());
				break;
			case SUR_CLOSE:
				printf("%s: SUR_CLOSE\n", n.c_str());
				break;
			default:
				printf("%s: unknown token\n", n.c_str());
				break;
			}
		}

	// set the receiving surrogate's SIM name
	switch (token)
		{
		case SUR_NAME_LOCATE:
			strcpy(receiver, RS232_Surr_S);
			break;
		case SUR_REPLY:
		case SUR_ERROR:
		case SUR_ALIVE_REPLY:
			sprintf(receiver, "%s_%d", RS232_Surr_r, pid);
			break;
		case SUR_SEND:
		case SUR_PROXY:
		case SUR_ALIVE:
		case SUR_CLOSE:
			sprintf(receiver, "%s_%d", RS232_Surr_r, pid);
			break;
		default:
			sryLog("%s: unknown message token\n", n.c_str());
			continue;
		}

	// name locate the appropriate program
	id = nee.Locate("", receiver, 0, SIM_LOCAL);
	if (id == -1)
		{
		sryLog("%s: name locate of %s error.\n", n.c_str(), receiver);
		continue;
		}

	// send  message to the appropriate receiver
	if (nee.Send(id, MemArea, hdrSize + numBytes, nullptr, 0) == -1)
		{
		sryLog("%s: send to %s error.\n", n.c_str(), receiver);
		}

	// close the name located id
	close(id);
	}
}	

/**********************************************************************
FUNCTION:	readSerial(int fd, int numBytes)

PURPOSE:	read numBytes from a serial line
  
RETURNS:	number of bytes read
**********************************************************************/	

int readSerial(int fd, int numBytes, char *mem)
{
int numRead = 0, numLeft = numBytes, num;
char *ptr = mem;

while (numLeft > 0)
	{
	num = read(fd, ptr, numLeft);
	if (num < 0)
		{
		return -1;
		}
	else if (num == 0)
		{
		if((numBytes - numLeft) == 0)
			{
			errno = EIO;
			return -1;
			} 
		else
			{
			break;
			}
		}

	numLeft -= num;
	numRead += num;
	ptr += num;
	}

return numRead;
}

/**********************************************************************
FUNCTION:	rs232_w(void)

PURPOSE:	receive messages from surrogates to write out via serial port
  
RETURNS:	void
**********************************************************************/	

void rs232_w()
{
// MemArea is global
// MemSize is global
const std::string &n = RS232_WRITER;
int fd, nBytes;
void *sender;

// create an instance of SRY called 'noo' with receiver name 'n'
SRY noo(n);

// open the serial port
fd = open(SerialDevice, OPEN_FLAGS);
if (fd <  0)
	{
	sryLog("%s: cannot open %s for rw-%s\n", n.c_str(), SerialDevice, 
															strerror(errno));
	exit(EXIT_FAILURE);
	}

while (true)
	{
	// receive incoming message header
	nBytes = noo.Receive(&sender, nullptr, 0);
	if (nBytes == -1)
		{
		sryLog("%s: receive error.\n",n.c_str());
		continue;
		}
	
	// increase memory if necessary
	if (nBytes > MemSize)
		{
		// adjust memory
		if (upMemory(nBytes) == -1)
			{
			sryLog("%s: cannot adjust memory-%s\n", n.c_str(), strerror(errno));
			exit(EXIT_FAILURE);
			}
		}

	// get the message from sender's shmem
	noo.simRcopy(sender, MemArea, nBytes);

	// reply to sender
	noo.Reply(sender, nullptr, 0);

	// write the message out serially
	if (write(fd, MemArea, nBytes) == -1)
		{
		sryLog("%s: problem writing message to %s-%s\n", n.c_str(),
			SerialDevice, strerror(errno));
		continue;
		}
	}
}

/**********************************************************************
FUNCTION:	int upMemory(int)

PURPOSE:	Adjust the global dynamic memory for upward growth. 

RETURNS:	success: 0
			failure: -1	
**********************************************************************/	

int upMemory(int nbytes)
{
// MemArea is global
// MemSize is global
const std::string fn("upMemory");

// realloc memory
MemArea = (char *)realloc(MemArea, nbytes);
if (!MemArea)
	{ 
	sryLog("%s: memory allocation error-%s\n", fn, strerror(errno));
	return -1;
	}

// reset global memory size
MemSize = nbytes;

return 0;
}
