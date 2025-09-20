/*******************************************************************************
FILE:			RS232_surrogate_rr.cpp

DATE:			January 26, 2025

DESCRIPTION:	This program runs as a fork from RS232_surrogate_R.cpp.

NOTES:			receives the following messages:
				1. SUR_NAME_LOCATE (from local sender)
				2. SUR_SEND (from local sender)
				3. SUR_PROXY (from local sender)
				4. SUR_CLOSE (from local sender)
				5. SUR_ALIVE_REPLY (from remote surrogate_s)

				sends the following messages:
				1. SUR_NAME_LOCATE (to remote surrogate_s)
				2. SUR_SEND (to remote surrogate_s)
				3. SUR_PROXY (to remote surrogate_s)
				4. SUR_CLOSE (to remote surrogate_s)
				5. SUR_ALIVE (to remote surrogate_s)
				6. reply messages back to the local sender.

AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _ALLOC extern
#include "RS232_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	surrogate_r(void)

PURPOSE:	Talk via serial to the rs232 reader process on another host
			to perform a remote Locate() of a receiver process on that
			host. If such a process exists, then become a surrogate
			receiver and serialsender to a remote surrogate serial receiver
			and sender on that other host thus acting as one side of a
			conduit between a local sender and a remote receiver.

RETURNS:	nothing, it is a forked process and never returns.
**********************************************************************/	

void surrogate_r()
{
// SerialWriter is global
// KaTimeout is global
// SurRpid is global
// Sender is global
std::string n = RS232_Surr_r, &p = n;
fd_set watchset, inset;
struct timeval tv, *timeoutPtr;
int ret, kaCounter = 0, fds[1];

// get the pid
SurRpid = getpid();

// set the SIM name of this forked program
p += std::to_string(SurRpid);

// detach child from parent's sim
closeSRYchild();

// create an instance of SRY called 'nee' with receiver name 'me'
SRY nee(p);
SRY &noo = nee;

// name locate the rs232 serial writer
SerialWriter = nee.Locate("", RS232_WRITER, 0, SIM_LOCAL);
if (SerialWriter == -1)
	{
	sryLog("%s: Locate on serial writer error.\n", p.c_str());
	exit(EXIT_FAILURE);
	}

// a remote name locate message will be the first thing to happen
if (hndlRemoteNameLocate(noo) == -1)
	{
	sryLog("%s: name locate error.\n", p.c_str());
	exit(EXIT_FAILURE);
	}

// for local sim messages 
fds[0] = nee.rfd();

// set select parameters
FD_ZERO(&watchset);
FD_SET(fds[0], &watchset);

// set timer values in case of keep alive
if (!KaTimeout)
	timeoutPtr = nullptr;
else
	{
	tv.tv_sec = KaTimeout;
	tv.tv_usec = 0;
	timeoutPtr = &tv;
	}

while (true)
	{
	// reset the fd set each time
	inset = watchset;

	// let select be the trigger on the file descriptor/timer
	ret = select(fds[0]+1, &inset, NULL, NULL, timeoutPtr);
	if (ret > 0) 
		{
		if (FD_ISSET(fds[0], &inset))
			{
			// this is a local simpl Send message
			// or send reply/keep alive reply from remote surrogate
			if (hndlMsg(noo, &kaCounter) == -1)
				{
				sryLog("%s: hndlSendMsg error\n", p.c_str());
				exit(EXIT_FAILURE);
				}
			}
		else 
			{
			// unknown fd error
			sryLog("%s: unknown fd on select-%s\n", p.c_str(), strerror(errno));
			}
		}
	else if (ret == 0) 
		{
		// a good opportunity to check on the local sender
		if (nee.chkSender(Sender) == false)
			{
			// sender has gone and somehow left this surrogate behind
			// one posibility is that the sender was SIGKILL'd
			// send a close message to the remote surrogate partner
			hndlClose(noo);
			exit(EXIT_SUCCESS);
			}	
			
		/*
		select timer kick returns 0 --> send a keep alive message to 
		surrogate partner
		*/
		if (sendKeepAliveMsg(noo, &kaCounter) == -1)
			{
			sryLog("%s: sendKeepAliveMsg error\n", p.c_str());
			exit(EXIT_FAILURE);
			}

		// check the number of keep alive messages sent
		if (kaCounter > KaFailLimit)
			{
			sryLog("%s: too many keep alive failures.\n", p.c_str());
			exit(EXIT_FAILURE);
			}

		// timer values must be reset each time as necessary
		tv.tv_sec = KaTimeout;
		tv.tv_usec = 0;
		timeoutPtr = &tv;
		}
	else
		{
		// select error
		sryLog("%s: select error-%s\n", p.c_str(), strerror(errno));
		}
	}
}
		
/**********************************************************************
FUNCTION:	hndlRemoteNameLocate(SRY&)

PURPOSE:	Handle a sim SUR_NAME_LOCATE call.
			Should only be called once for each surrogate child.

RETURNS:	int	
**********************************************************************/	

int hndlRemoteNameLocate(SRY &c)
{
// SurSpid is global
// SurRpid is global
// SerialWriter is global
// NamelocTimeout is global
// Sender is global
const std::string fn("hndlRemoteNameLocate_rs232");
int fds[1], nBytes, ret;
fd_set watchset; 
struct timeval tv, *timeoutPtr;
SUR_NAME_LOCATE_MSG lin, rout;
const static int outSize = sizeof(SUR_NAME_LOCATE_MSG);
SUR_NAME_LOCATE_REPLY rin, lout;
const static int inSize = sizeof(SUR_NAME_LOCATE_REPLY);
void *serialR;

// receive name locate message from the local sender
nBytes = c.Receive(&Sender, nullptr, 0);
if (nBytes == -1)
	{
	sryLog("%s: receive error.\n", fn.c_str());
	return -1;
	}

// make more certain that this is a name_locate message
if (nBytes != sizeof(SUR_NAME_LOCATE_MSG))
	{
	sryLog("%s: incorrect message size\n", fn.c_str());
	c.ReplyError(Sender);	
	return -1;
	} 

// get the name locate message from local sender's shmem
c.simRcopy(Sender, &lin, nBytes);

// check the token
if (atoi(lin.hdr.token) != SUR_NAME_LOCATE)
	{
	sryLog("%s: incorrect message type.\n", fn.c_str());
	c.ReplyError(Sender);
	return -1;
	}

/*
the remote host surrogate_s needs to know this pid so that when it sends 
messages to this program the local serial_r knows how to route it here. The
pid is used in the SIM name
*/
snprintf(lin.hdr.surPid, sizeof lin.hdr.surPid, "%d", SurRpid);
rout = lin;

// send the name locate message to the other box via the rs232_w program
if (c.Send(SerialWriter, &rout, outSize, nullptr, 0) == -1)
	{
	sryLog("%s: send error to serial writer on name locate msg-%s\n", 
																fn.c_str());
	c.ReplyError(Sender);
	return -1;
	}

// block and wait for response 
fds[0] = c.rfd();
FD_ZERO(&watchset);
FD_SET(fds[0], &watchset);

// set time out on wait for response
tv.tv_sec = NamelocTimeout;
tv.tv_usec = 0;
timeoutPtr = &tv;

ret = select(fds[0] + 1, &watchset, NULL, NULL, timeoutPtr);
if (ret > 0) 
	{
	if (FD_ISSET(fds[0], &watchset))
		{
		// read the reply to the SUR_NAME_LOCATE (-1 fail !=-1 success)
		// receive message from the local serial reader program
		if (c.Receive(&serialR, &rin, inSize) == -1)
			{
			sryLog("%s: receive error on name locate reply.\n", fn.c_str());
			c.ReplyError(Sender);
			return -1;
			}

		// an empty reply to the serial reader
		if (c.Reply(serialR, nullptr, 0) == -1)
			{
			sryLog("%s: reply error to serial reader.\n", fn.c_str());
			return -1;
			}

		// interpret replied message
		SurSpid = atoi(rin.result);
		strcpy(lout.result, rin.result); // important

		// reply the search results to the local sender
		if (c.Reply(Sender, &lout, sizeof(SUR_NAME_LOCATE_REPLY)) == -1)
			{
			sryLog("%s: reply error.\n", fn.c_str());
			return -1;
			}
		}
	else 
		{
		// unknown fd error
		sryLog("%s: unknown fd on select-%s\n", fn.c_str(), strerror(errno));
		}
	}
else if (ret == 0) 
	{
	// select timer kick returns 0 --> have not received a name locate reply
	// no error log; network/hostprocess simply not there
	c.ReplyError(Sender);
	return -1;
	}
else
	{
	// select error
	sryLog("%s: select error-%s\n", fn.c_str(), strerror(errno));
	c.ReplyError(Sender);
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	hndlMsg(SRY&, int *)

PURPOSE:	Handle sim SUR_SEND call from a local sender intended
			for a remote receiver.

RETURNS:	int	
**********************************************************************/	

int hndlMsg(SRY &c, int *kaCounter)
{
// InMsgArea is global
// InMsgSize is global
// OutMsgArea is global
// OutMsgSize is global
// SurSpid is global
const std::string fn("hndlMsg_rs232");
std::string senderName;
int nBytes, yBytes, maxBytes, proxyValue, token;
unsigned numBytes;
void *sender, *serialSender, *localSender = nullptr;
const static int hdrSize = sizeof(SUR_MSG_HDR);
SUR_MSG_HDR *in, *out;

// receive message from the local sender or the serial reader
nBytes = c.Receive(&sender, nullptr, 0);
if (nBytes == -1)
	{
	sryLog("%s: receive error.\n",fn.c_str());
	return -1;
	}

// is it a trigger proxy?
if (nBytes < -1)
	{
	// get the value of the proxy
	proxyValue = c.returnProxy(nBytes);
	
	// is it a close up shop proxy?
	if (proxyValue == PROXY_SHUTDOWN)
		{
		// send a close message to the remote surrogate partner
		hndlClose(c);
		exit(EXIT_SUCCESS);
		}
	else
		{
		return hndlProxy(c, proxyValue);
		}
	}

// get the sender information
c.getSenderName(sender, senderName);

if ( !strcmp(senderName.c_str(), RS232_READER) ) // msg sent from serial reader
	{
	// set the sender
	serialSender = sender;

	// do we need more memory for the incoming message?
	if (checkMemory(IN, nBytes) == -1)
		{
		sryLog("%s: in memory allocation error.\n", fn.c_str());
		return -1;
		}

	// get the message from serial reader's shmem
	c.simRcopy(serialSender, InMsgArea, nBytes);

	// null reply to the serial reader
	if (c.Reply(serialSender, NULL, 0) == -1)
		{
		sryLog("%s: reply to serial reader error.\n", fn.c_str());
		return -1;
		}

	in = (SUR_MSG_HDR *)InMsgArea;
	token = atoi(in->token);
	numBytes = atoi(in->nbytes);

	// reaction based on the message token
	switch (token)
		{
		case SUR_REPLY:
			// reply message from remote receiver to local ipc sender
			if (c.Reply(localSender, InMsgArea + hdrSize, numBytes) == -1)
				{
				sryLog("%s: reply error.\n", fn.c_str());
				return -1;
				}
			break;

		case SUR_ALIVE_REPLY:
			*kaCounter = 0;
			break;

		case SUR_ERROR:
			c.ReplyError(localSender);
			// surrogate sender dies upon this error ==> we are not needed
			exit(EXIT_FAILURE);

		default:
			sryLog("%s: unknown message token\n", fn.c_str());
			return(-1);
		}	
	}
else // from the local sender
	{
	// set the sender
	localSender = sender;

	// what is the size of the expected reply message?
	yBytes = c.getSenderRplySize(localSender);

	// which is larger, the sent or the replied message?
	maxBytes = (nBytes > yBytes) ? nBytes : yBytes;

	// check for adequate buffer size
	if (checkMemory(OUT, maxBytes) == -1)
		{ 
		sryLog("%s: memory allocation error\n", fn.c_str());
		c.ReplyError(sender);
		return -1;
		}

	// get the message from sender's shmem
	c.simRcopy(localSender, OutMsgArea + hdrSize, nBytes);

	// build the message going to the other box	
	out = (SUR_MSG_HDR *)OutMsgArea;
	snprintf(out->token, sizeof out->token, "%d", SUR_SEND);
	snprintf(out->nbytes, sizeof out->nbytes, "%d", nBytes);
	snprintf(out->ybytes, sizeof out->ybytes, "%d", yBytes);
	snprintf(out->surPid, sizeof out->nbytes, "%d", SurSpid);

	// send the message via serial to the other box
	if (c.Send(SerialWriter, OutMsgArea, hdrSize + nBytes, nullptr, 0) == -1)
		{
		sryLog("%s: send error on msg.\n", fn.c_str());
		c.ReplyError(localSender);	
		return -1;
		}
	}

return 0;
}

/**********************************************************************
FUNCTION:	sendKeepAliveMsg(SRY &c, int *)

PURPOSE:	Send a keep alive message to the other surrogate partner.

RETURNS:	int	
**********************************************************************/	

int sendKeepAliveMsg(SRY &c, int *kaCounter)
{
// SerialWriter is global
const std::string fn("sendKeepAliveMsg_rs232");
SUR_KA_MSG out;
const static int size = sizeof(SUR_KA_MSG);
	
// build keep alive message
snprintf(out.hdr.token, sizeof out.hdr.token, "%d", SUR_ALIVE);
snprintf(out.hdr.nbytes, sizeof out.hdr.nbytes, "%d", 0);
snprintf(out.hdr.surPid, sizeof out.hdr.surPid, "%d", SurSpid);

// send the keep alive message to the other surrogate's parent 
if (c.Send(SerialWriter, &out, size, nullptr, 0) == -1)
	{
	sryLog("%s: send error on keep alive msg.\n", fn.c_str());
	return -1;
	}

// increment the number of keep alive messages sent
*kaCounter += 1;

return 0;
}

/**********************************************************************
FUNCTION:	hndlClose(SRY&)

PURPOSE:	Handle simpl SUR_CLOSE call. This will cause this
			"receiving surrogate" to exit and send a close instruction
			to its remote "send surrogate" partner.

RETURNS:	void	
**********************************************************************/	

void hndlClose(SRY &c)
{
// SerialWriter is global
// SurSpid is global
const std::string fn("hndlClose_rs232");
SUR_MSG_HDR out;
const int size = sizeof(SUR_MSG_HDR);

// build close message
snprintf(out.token, sizeof out.token, "%d", SUR_CLOSE);
snprintf(out.nbytes, sizeof out.nbytes, "%d", 0);
snprintf(out.surPid, sizeof out.surPid, "%d", SurSpid);

// send a message to the rs232 serial writer
if (c.Send(SerialWriter, &out, size, nullptr, 0) == -1)
	sryLog("%s: send error on close msg.\n", fn.c_str());
}

/**********************************************************************
FUNCTION:	hndlProxy(SRY&, int)

PURPOSE:	Handle sending a proxy to the remote surrogate_s.

RETURNS:	int	
**********************************************************************/	

int hndlProxy(SRY &c, int proxyValue)
{
// SerialWriter is global
// SurSpid is global
const std::string fn("hndlProxy_rs232");
SUR_PROXY_MSG out;
const static int size = sizeof(SUR_PROXY_MSG);
	
// build proxy message
snprintf(out.hdr.token, sizeof out.hdr.token, "%d", SUR_PROXY);
snprintf(out.hdr.nbytes, sizeof out.hdr.nbytes, "%d", CHAR_BASE);
snprintf(out.hdr.surPid, sizeof out.hdr.surPid, "%d", SurSpid);
snprintf(out.proxy, sizeof out.proxy, "%d", proxyValue);

// send a message to the rs232 serial writer
if (c.Send(SerialWriter, &out, size, nullptr, 0) == -1)
	{
	sryLog("%s: send error on proxy msg.\n", fn.c_str());
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	replyFailure(SRY&, void *)

PURPOSE:	Failure reply function used by Receive surrogates to advise
			a local sender of communication errors resulting in a
			Send() failure.

RETURNS:	void
**********************************************************************/	

void replyFailure(SRY &c, void *caller)
{
static SUR_NAME_LOCATE_REPLY reply = {{0, 0}, -1,};

c.Reply(caller, &reply, sizeof(SUR_NAME_LOCATE_REPLY));
}
