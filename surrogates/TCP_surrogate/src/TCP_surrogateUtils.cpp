/*******************************************************************************
FILE:			TCP_surrogateUtils.cpp

DATE:			January 23, 2025

DESCRIPTION:	This file contains useful utility functions used by the
				various surrogate*.cpp files.

AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _ALLOC extern
#include "TCP_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	int initRsock(struct sockaddr_in *)

PURPOSE:	Create and initialize a socket to be used for receiving.
			Used by a server (TCP socket receiver) in the client/server model.

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int initRsock(struct sockaddr_in *rserver)
{
// surrPort is global
const std::string fn("initRsock");
char port[50];

// set the port number
sprintf(port, "%u", SurrPort);

// configure local address
struct addrinfo hints;
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET6;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;
struct addrinfo *bind_address;
if (getaddrinfo(0, port, &hints, &bind_address))
	{
	sryLog("%s: getaddrinfo error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

/*
create an unitialized (not connected to anything) TCP/IP (AF_INET6)
stream protocol socket (SOCK_STREAM)
*/
int rsock;
rsock = socket(bind_address->ai_family, bind_address->ai_socktype, 
	bind_address->ai_protocol);
if (rsock < 0)
	{
	sryLog("%s: socket creation error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

int option = 0;
if (setsockopt(rsock, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, 
	sizeof option))
	{
	sryLog("%s: setsockopt error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

/*
At this point you may wish to augment socket behaviour with further calls to
setsockopt() and/or getsockopt().
*/

// bind socket to local address
if (bind(rsock, bind_address->ai_addr, bind_address->ai_addrlen))
	{
	sryLog("%s: bind error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}
freeaddrinfo(bind_address);

// willing to allow connections to the socket
if (listen(rsock, MaxPendingConnections) < 0)
	{
	sryLog("%s: listen error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

// got this far without mishap
return rsock;
}

/**********************************************************************
FUNCTION:	int initSsock(char *)

PURPOSE:	Create and initialize a socket to be used for sending.
			Used by a client (sender) in the client/server model. 

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int initSsock(char *hostName)
{
// surrPort is global
const std::string fn("initSsock");
char port[50];

// set the port number
sprintf(port, "%u", SurrPort);

// configure remote address
struct addrinfo hints;
memset(&hints, 0, sizeof hints);
hints.ai_socktype = SOCK_STREAM;
struct addrinfo *peer_address;
if (getaddrinfo(hostName, port, &hints, &peer_address))
	{
	sryLog("%s: getaddrinfo error-%s\n", fn.c_str(), strerror(errno));
	return -1;
    }

//printf("Remote address is: ");
char address_buffer[100];
char service_buffer[100];
getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
	address_buffer, sizeof address_buffer, service_buffer, 
	sizeof service_buffer, NI_NUMERICHOST);
//printf("%s %s\n", address_buffer, service_buffer);

// create socket
int ssock;
ssock = socket(peer_address->ai_family, peer_address->ai_socktype, 
	peer_address->ai_protocol);
if (ssock < 0)
	{
	sryLog("%s: socket creation error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

/*
At this point you may wish to augment socket behaviour with calls to 
setsockopt() and/or getsockopt().
*/

// try to connect to the other node's surrogate_S
if (connect(ssock, peer_address->ai_addr, peer_address->ai_addrlen))
	{
	sryLog("%s: connect error-%s\n", fn.c_str(), strerror(errno));
	sryLog("%s: port-%u\n", fn.c_str(), SurrPort);
	sryLog("%s: host-%s\n", fn.c_str(), hostName);
	return -1;
	}
freeaddrinfo(peer_address);

return ssock;
}

/**********************************************************************
FUNCTION:	int surRead(int, void *, unsigned)

PURPOSE:	Read a stream for a well-defined number of bytes.

RETURNS:	int
**********************************************************************/	

int surRead(int sock, void *ptr, unsigned nBytes)
{
int numRead, numLeft = nBytes;
char *ctr = (char *)ptr;

while (numLeft > 0)
	{
	if ((numRead = read(sock, (void *)ctr, numLeft)) < 0)
		return -1;
	numLeft -= numRead;
	for (int i = 0; i < numRead; ++i, ++ctr);
	}

return 0;
}

/**********************************************************************
FUNCTION:	int surWrite(int, void *, unsigned)

PURPOSE:	Write a stream for a well-defined number of bytes.

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int surWrite(int sock, void *ptr, unsigned nBytes)
{
return (write(sock, ptr, nBytes) == nBytes) ? 0 : -1;
}

/**********************************************************************
FUNCTION:	int setMsgBuffer(unsigned)

PURPOSE:	Set the global dynamic memory memory sizes. 

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int setMsgBuffer(unsigned nbytes)
{
// totMem, totMemSize, hdrMem, hdrMemSize, msgMem, msgMemSize are global
const std::string fn("setMsgMemory");
// need header + msg size
unsigned memSize = sizeof (SUR_MSG_HDR) + nbytes;

// check desired size against limit
if (memSize > MaxMsgMemSize)
	{
	sryLog("%s: desired message size %d bytes is too large\n",
													fn.c_str(), nbytes);
	return -1;
	}

// is there already enough memory allocated?
if (TotMemSize >= memSize)
	return 0;
	 
/*
delete is supposed to check for nullptr - let's make sure
as well, once released the original ptr is not made null
*/
if (TotMem != nullptr)
	{
	delete[] TotMem;
	TotMem = nullptr;
	HdrMem = nullptr;
	MsgMem = nullptr;
	TotMemSize = 0;
	HdrMemSize = 0;
	MsgMemSize = 0;
	}

// alloc message body memory
TotMem = new(std::nothrow) char[memSize];
if (TotMem == nullptr)
	{ 
	sryLog("%s: msg memory alloc error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

TotMemSize = memSize;
HdrMem = TotMem;
MsgMem = TotMem + sizeof (SUR_MSG_HDR);
HdrMemSize = sizeof (SUR_MSG_HDR);
MsgMemSize = nbytes;

return 0;
}

/**********************************************************************
FUNCTION:	void deadChildren(void)

PURPOSE:	This function deals with dead children; ie. surrogate_r()
			and surrogate_s().

RETURNS:	nothing
**********************************************************************/	

void deadChildren()
{
struct sigaction zom;

zom.sa_handler = SIG_IGN;
sigemptyset(&zom.sa_mask);
zom.sa_flags = SA_NOCLDWAIT;
sigaction(SIGCHLD, &zom, NULL);
}

/**********************************************************************
FUNCTION:	hndlClose(int)

PURPOSE:	Handle a SUR_CLOSE call. Send a close instruction to the 
			remote surrogate partner, either surrogate_r or surrogate_s.

RETURNS:	void
**********************************************************************/	

void hndlClose(int sock)
{
// hdrMem, hdrMemSize are global
const std::string fn("hndlClose");
SUR_MSG_HDR *hdr = (SUR_MSG_HDR *)HdrMem;

// set up close message
snprintf(hdr->token, sizeof hdr->token, "%d", SUR_CLOSE); 

// send the message via the socket to the other box
if (surWrite(sock, hdr, HdrMemSize) == -1)
	sryLog("%s: write error on close msg-%s\n", fn.c_str(), strerror(errno));
}
