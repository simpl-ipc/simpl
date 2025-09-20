/*******************************************************************************
FILE:			RS232_surrogate_ss.cpp

DATE:			January 26, 2025

DESCRIPTION:	This program runs as a fork from RS232_surrogate_S.cpp.

NOTES:			surrogate_s receives the following messages:
				1. SUR_SEND (from remote surrogate_r)
				2. SUR_PROXY (from remote surrogate_r)
				2. SUR_CLOSE (from remote surrogate_r)
				3. SUR_ALIVE (from remote surrogate_r)

				surrogate_s sends the following messages:
				1. SUR_ALIVE (to remote surrogate_r)
				2. messages intended for the local receiver.
				
AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _ALLOC extern
#include "RS232_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	surrogate_s(int)

PURPOSE:	Handle a remote name locate request and act as a conduit
			by receiving serial messages from a remote surrogate receiver
			and sending them on to the actual receiver.

RETURNS:	nothing, it is a forked process and never returns.	
**********************************************************************/	

void surrogate_s()
{
// KaTimeout is global
// KaHostName is global
// SerialWriter is global
// SurSpid is global
// SurRpid is global
std::string n = RS232_Surr_s, &p = n;
int fds[2], maxFd, retVal, ret, id, result, yBytes = 0, kaCounter = 0;
fd_set inset, watchset; 
struct timeval tv, *timeoutPtr;
SUR_NAME_LOCATE_MSG *surMsg = (SUR_NAME_LOCATE_MSG *)InMsgArea;

// we need the pid of the remote surrogate_r
SurRpid = atoi(surMsg->hdr.surPid);

// get the pid
SurSpid = getpid();

// set the SIM name of this forked program
p += std::to_string(SurSpid);

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

// perform the local name locate for the asked for process
id = nee.Locate("", surMsg->sryName, 0, SIM_LOCAL);

// set the result of the local name locate
result = (id == -1) ? -1 : SurSpid;

// reply the results of the local Locate()
SUR_NAME_LOCATE_REPLY reply;
const static int size = sizeof(SUR_NAME_LOCATE_REPLY);
snprintf(reply.result, sizeof reply.result, "%d", result);
snprintf(reply.pid, sizeof reply.pid, "%d", SurRpid);

// send a message to the rs232 serial writer
if (nee.Send(SerialWriter, &reply, size, nullptr, 0) == -1)
	{
	sryLog("%s: send error.\n", p.c_str());
	exit(EXIT_FAILURE);
	}

// if the results of the local name_locate() are -1, we are not needed
if (result == -1)
	exit(EXIT_SUCCESS);

// arm select
fds[0] = nee.rfd();
fds[1] = nee.yfd();
maxFd = (fds[0] > fds[1]) ? fds[0] : fds[1];
maxFd += 1;

FD_ZERO(&watchset);
FD_SET(fds[0], &watchset);
FD_SET(fds[1], &watchset);

// initialize timeout structure
if (!KaTimeout)
	{
	timeoutPtr = nullptr;
	}
else
	{
	tv.tv_sec = KaTimeout;
	tv.tv_usec = 0;
	timeoutPtr = &tv;
	}
 
// handle incoming messages destined for a local receiver
while (true)
	{
	// reset the fd set each time
	inset = watchset;

	// let select be the trigger on the file descriptors/timer
	retVal = select(maxFd, &inset, NULL, NULL, timeoutPtr);
	if (retVal > 0) 
		{
		if (FD_ISSET(fds[0], &inset))
			{
			// a message from surrogate partner
			ret = hndlMessage(noo, id, &kaCounter, &yBytes);
			if (ret == -1)
				{
				errorReply(noo);
				exit(EXIT_FAILURE);
				}
			}
		else if (FD_ISSET(fds[1], &inset))
			{
			// a reply from local receiver
			ret = hndlReply(noo, yBytes);
			if (ret == -1)
				{
				errorReply(noo);
				exit(EXIT_FAILURE);
				}
			}
		else 
			{
			// unknown fd error
			sryLog("%s: fd error on select\n", p.c_str());
			}
		}
	else if (retVal == 0) 
		{
		// a good opportunity to check on the local receiver
		if (nee.chkReceiver("", SurRpid) == false)
			{
			// could also send a close message to surrogate_r partner
			// surrogate_r should pick up on the ka failures
			exit(EXIT_SUCCESS);
			}
				
		// select timer has gone off check the kaCounter
		kaCounter += 1;
		if (kaCounter > KaFailLimit)
			{
			// we assume that our surrogate partner is no longer
			exit(EXIT_SUCCESS);
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
FUNCTION:	hndlMessage(SRY&, int, int *, int *)

PURPOSE:	Deal with an incoming messages from the receiver surrogate.

RETURNS:	int	
**********************************************************************/	

int hndlMessage(SRY &c, int fd, int *kaCounter, int *yBytes)
{
// InMsgArea is global
// OutMsgArea is global
// SerialWriter is global
const std::string fn("hndlMessage_rs232");
int nBytes, token, proxyValue, msgSize;
void *serialR;
SUR_MSG_HDR *hdr;
const static int hdrSize = sizeof(SUR_MSG_HDR);
SUR_KA_REPLY_MSG *out;
const static int outSize = sizeof(SUR_KA_REPLY_MSG);
SUR_PROXY_MSG *prox;

// receive message from the serial reader
msgSize = c.Receive(&serialR, nullptr, 0);
if (msgSize == -1)
	{
	sryLog("%s: receive error.\n", fn.c_str());
	return -1;
	}

// set to largest message encountered so far
if (checkMemory(IN, msgSize) == -1)
	{
	sryLog("%s: memory allocation error\n", fn.c_str());
	return -1;
	}

// get the message from sender's shmem
c.simRcopy(serialR, InMsgArea, msgSize);

// reply to serial reader
if (c.Reply(serialR, nullptr, 0) == -1)
	{
	sryLog("%s: reply to serial reader error.\n", fn.c_str());
	return -1;
	}

// interpret the message header
hdr = (SUR_MSG_HDR *)InMsgArea;
token = atoi(hdr->token);

// react on token value
switch (token)
	{
	case SUR_SEND:
		// set send and reply message sizes
		nBytes = atoi(hdr->nbytes);
		*yBytes = atoi(hdr->ybytes);
		
		// send message to local receiver process
		if (c.PostMessage(fd, InMsgArea + hdrSize, nBytes, *yBytes) == -1)
			{
			sryLog("%s: send error.\n", fn.c_str());
			return -1;
			}
		break;

	case SUR_PROXY:
		// extract the proxy value
		prox = (SUR_PROXY_MSG *)InMsgArea;
		proxyValue = atoi(prox->proxy);

		// trigger proxy
		if (c.Trigger(fd, proxyValue) == -1)
			{
			sryLog("%s: trigger error.\n", fn.c_str());
			return -1;
			}
		break;

	case SUR_ALIVE:
		*kaCounter = 0;

		// compose the reply message
		out = (SUR_KA_REPLY_MSG *)OutMsgArea;
		snprintf(out->hdr.token, sizeof out->hdr.token, "%d", SUR_ALIVE_REPLY);
		snprintf(out->hdr.nbytes, sizeof out->hdr.nbytes, "%d", 0);
		snprintf(out->hdr.surPid, sizeof out->hdr.surPid, "%d", SurRpid);

		// send reply back to surrogate partner
		if (c.Send(SerialWriter, out, outSize, nullptr, 0) == -1)
			{
			sryLog("%s: send error to serial writer.\n", fn.c_str());
			return -1;
			}
		break;
		
	case SUR_CLOSE:
		exit(EXIT_SUCCESS);

	default:
		sryLog("%s: unknown message token=%d.\n", fn.c_str(), token);
		return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	hndlReply(SRY&, int)

PURPOSE:	Deal with replies from the receiver process.

RETURNS:	int	
**********************************************************************/	

int hndlReply(SRY &c, int yBytes)
{
// OutMsgArea is global
// SerialWriter is global
// SurRpid is global
const std::string fn("hndlReply_rs232");
int replySize;
const static int hdrSize = sizeof(SUR_MSG_HDR);

// is there enough outgoing message memory?
if (checkMemory(OUT, yBytes) == -1)
	{ 
	sryLog("%s: memory allocation error.\n", fn.c_str());
	return -1;
	}

// get the reply message
replySize = c.ReadReply(OutMsgArea + hdrSize);
if (replySize == -1)
	{
	sryLog("%s: readReply error.\n", fn.c_str());
	return -1;
	}

// build the reply header
SUR_MSG_HDR *hdr = (SUR_MSG_HDR *)OutMsgArea;
snprintf(hdr->token, sizeof hdr->token, "%d", SUR_REPLY);
snprintf(hdr->nbytes, sizeof hdr->nbytes, "%d",replySize);
snprintf(hdr->surPid, sizeof hdr->surPid, "%d", SurRpid);

// send reply back to surrogate partner via the rs232 serial writer
if (c.Send(SerialWriter, OutMsgArea, hdrSize + replySize, nullptr, 0) == -1)
	{
	sryLog("%s: send error reply.\n", fn.c_str());
	return -1;
	}

return 0;
}
	
/**********************************************************************
FUNCTION:	errorReply(SRY&)

PURPOSE:	Send an error reply back to the receiver surrogate
			indicating problems at this end.

RETURNS:	void	
**********************************************************************/	

void errorReply(SRY &c)
{
// SerialWriter is global
const std::string fn("errorReply_rs232");

// concoct the error reply
SUR_MSG_HDR out;
const static int size = sizeof(SUR_MSG_HDR);
snprintf(out.token, sizeof out.token, "%d", SUR_ERROR);
snprintf(out.nbytes, sizeof out.nbytes, "%d", 0);
snprintf(out.surPid, sizeof out.surPid, "%d", SurRpid);

// send a message to the rs232 serial writer
if (c.Send(SerialWriter, &out, size, nullptr, 0) == -1)
	sryLog("%s: send error.\n", fn.c_str());
}
