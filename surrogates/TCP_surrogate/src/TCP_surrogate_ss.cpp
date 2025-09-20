/*******************************************************************************
FILE:			TCP_surrogate_ss.cpp

DATE:			January 23 2025

DESCRIPTION:	This program runs as a fork from surrogate_S.cpp

AUTHOR:			John Collins

NOTES:			surrogate_s receives the following messages:
				1. SUR_SEND (from remote surrogate_r)
				2. SUR_PROXY (from remote surrogate_r)
				3. SUR_CLOSE (from remote surrogate_r)
				4. SUR_ALIVE (from surrogate_r)

				surrogate_s sends the following messages:
				1. SUR_ALIVE_REPLY (to remote surrogate_r)
				2. messages intended for the local receiver.
******************************************************************************/

// application header
#define _ALLOC extern
#include "TCP_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	void surrogate_s(int)

PURPOSE:	Handle a remote name locate request and act as a conduit
			by receiving tcp messages from a remote surrogate receiver
			and sending them on to the actual receiver.

RETURNS:	nothing, it is a forked process and never returns.	
**********************************************************************/	

void surrogate_s(int sock)
{
// KaTimeout, KaHostName, SurrPort, MsgMem are global
/*
note that the name locate msg received by the parent is in a copy of
MsgMem via the fork()
*/
std::string n = TCP_Surr_s, &p = n;
fd_set inset,  watchset; 
struct timeval tv, *timeoutPtr;
long id;
int retVal;
SUR_NAME_LOCATE_MSG *msg = (SUR_NAME_LOCATE_MSG *)MsgMem;
pid_t childPid = getpid();

// set the SIM name of this forked program
p += std::to_string(childPid);

// ensure socket ownership
if (fcntl(sock, F_SETOWN, childPid) == -1)
	{  
	sryLog("%s: socket ownership error-%s\n", p.c_str(), strerror(errno));
	nameLocateReply(sock, -1, 0);
	exit(EXIT_FAILURE);
	}

// intantiate a SRY object with reference
SRY nee(p);
SRY &noo = nee;

/*
does this process exist locally? id = -1 implies that there in such SIM name 
msg->sryName on localhost
*/
id = noo.Locate("", msg->sryName, atoi(msg->maxSize), SIM_LOCAL);

// remotely reply the results of the local Locate()
if (nameLocateReply(sock, id, childPid) == -1)
	{
	sryLog("%s: name locate reply error\n", p.c_str());
	exit(EXIT_FAILURE);
	}

// if the results of the local Locate() are -1, we are not needed
if (id == -1)
	exit(EXIT_SUCCESS);

// check for adequate msg buffer
if (setMsgBuffer(atoi(msg->maxSize)) == -1)
	{ 
	sryLog("%s: memory allocation error\n", p.c_str());
	exit(EXIT_FAILURE);	
	}

FD_ZERO(&watchset);
FD_SET(sock, &watchset);

// handle incoming messages destined for a local receiver
while (true)
	{
	inset = watchset;
	tv.tv_sec = ChkTimeout;
	tv.tv_usec = 0;
	timeoutPtr = &tv;

	// let select be the trigger on the file descriptor/timer
	retVal = select(sock + 1, &inset, NULL, NULL, timeoutPtr);
	if (retVal > 0) 
		{
		if (FD_ISSET(sock, &inset))
			{
			// a message from surrogate partner
			if (hndlMsg(sock, id, noo) == -1)
				{
				errorReply(sock);
				exit(EXIT_FAILURE);
				}
			}
		else 	// unknown fd error
			{
			sryLog("%s: fd error on select\n", p.c_str());
			}
		}
	else if (retVal == 0) // timer
		{
		// check on the local receiver
		// surrogate_r should also pick up on the ka failures (if active)
		if (noo.chkReceiver(msg->sryName, 0) == false)
			{
			// send a close message to the remote surrogate partner
			hndlClose(sock);
			exit(EXIT_SUCCESS);
			}
		}
	else	// select error
		{
		sryLog("%s: select error-%s\n", p.c_str(), strerror(errno));
		}
	}
}

/**********************************************************************
FUNCTION:	hndlMsg(int, long, SRY&)

PURPOSE:	Deal with an incoming message from the remote receiver
			surrogate.

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int hndlMsg(int sock, long id, SRY &n)
{
// HdrMem, HdrMemSize are global
const std::string fn("hndlMsg");
unsigned int nBytes, yBytes, maxBytes;
int replySize;
SUR_MSG_HDR *hdr = (SUR_MSG_HDR *)HdrMem;
SUR_PROXY_MSG *prox = (SUR_PROXY_MSG *)TotMem;

// read the header portion of the message
if (surRead(sock, HdrMem, HdrMemSize) == -1)
	{
	sryLog("%s: read error on header-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

// react on token value
switch (atoi(hdr->token))
	{
	case SUR_SEND:
		// set send and reply message sizes
		nBytes = atoi(hdr->nbytes);
		yBytes = atoi(hdr->ybytes);
		
		// what is the largest message; sent or replied?
		maxBytes = (nBytes > yBytes) ? nBytes : yBytes;

		// check for adequate buffer size
		if (maxBytes > MsgMemSize)
			{
			sryLog("%s: message memory area too small\n", fn.c_str());
			return -1;
			}	
			
		// read in the data portion of the message
		if (surRead(sock, MsgMem, nBytes) == -1)
			{
			sryLog("%s: read error on message-%s\n",
										fn.c_str(), strerror(errno));
			return -1;
			}

		// send message to local receiver process
		replySize = n.Send(id, MsgMem, nBytes, MsgMem, yBytes);
		if (replySize == -1)
			{
			sryLog("%s: send error.\n", fn.c_str());
			return -1;
			}

		// build the reply message
		snprintf(hdr->token, sizeof hdr->token, "%d", SUR_REPLY); 
		snprintf(hdr->ybytes, sizeof hdr->ybytes, "%d", replySize); 

		// send the reply msg
		if (surWrite(sock, TotMem,  HdrMemSize + replySize) == -1)
			{
			sryLog("%s: write error-%s\n", fn.c_str(), strerror(errno));
			return -1;
			}
		break;

	case SUR_PROXY:
		// read in the data portion of the message
		if (surRead(sock, MsgMem, atoi(hdr->nbytes)) == -1)
			{
			sryLog("%s: read error on proxy-%s\n", fn.c_str(), strerror(errno));
			return -1;
			}	

		// trigger proxy
		if (n.Trigger(id, atoi(prox->proxy)) == -1)
			{
			sryLog("%s: trigger error.\n", fn.c_str());
			return -1;
			}
		break;
		
	case SUR_CLOSE:
		exit(EXIT_SUCCESS);

	case SUR_ALIVE:
		if (hndlKeepAlive(sock) == -1)
			{
			sryLog("%s: keep alive error.\n", fn.c_str());
			return -1;
			}
		break;

	default:
		sryLog("surrogate_s: %s: token=%d\n", fn.c_str(), atoi(hdr->token));
		return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	int nameLocateReply(int, long, pid_t)

PURPOSE:	This function reports the results of a remote name_locate()
			to the calling receiver surrogate.

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int nameLocateReply(int sock, long result, pid_t pid)
{
// MsgMem is global, a header is not necessary for a reply
const std::string fn("nameLocateReply");
SUR_NAME_LOCATE_REPLY *out = (SUR_NAME_LOCATE_REPLY *)MsgMem;
const int repSize = sizeof (SUR_NAME_LOCATE_REPLY);

// build the reply message
snprintf(out->result, sizeof out->result, "%ld", result); 
snprintf(out->pid, sizeof out->pid, "%d", pid);

// send the reply msg
if (surWrite(sock, out, repSize) == -1)
	{
	sryLog("%s: write error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	int hndlKeepAlive(int)

PURPOSE:	handles a child alive message.
  
RETURNS:	success:	 0
			failure:	-1
**********************************************************************/

int hndlKeepAlive(int sock)
{
// TotMem is global
const std::string fn("hndlKeepAlive");
SUR_KA_MSG *i = (SUR_KA_MSG *)TotMem;
SUR_KA_REPLY_MSG *o = (SUR_KA_REPLY_MSG *)TotMem;
const int oSize = sizeof (SUR_KA_REPLY_MSG);
char dirname[50];

// read in the last bit
if (surRead(sock, i->kaPid, sizeof i->kaPid) == -1)
	{
	sryLog("%s: read error on kaPid-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

// this directory will exist if child still lives
sprintf(dirname, "/proc/%d", atoi(i->kaPid));

// compose the reply message, write over top
snprintf(o->hdr.token, sizeof o->hdr.token, "%d", SUR_ALIVE_REPLY);
snprintf(o->hdr.nbytes, sizeof o->hdr.nbytes, "%d", (int)sizeof o->result);
// out->hdr.ybytes is unnecessary, this is a reply
snprintf(o->result, sizeof o->result, "%d", access(dirname, F_OK));

// send reply back to surrogate partner
if (surWrite(sock, o, oSize) == -1)
	{
	sryLog("%s: write error-%s\n", fn.c_str(), strerror(errno));
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	errorReply(int)

PURPOSE:	Send an error reply back to the receiver surrogate
			indicating problems at this end.

RETURNS:	nothing
**********************************************************************/	

void errorReply(int sock)
{
// HdrMem, HdrMemSize are global
const std::string fn("errorReply");
SUR_MSG_HDR *hdr = (SUR_MSG_HDR *)HdrMem;

// concoct the error reply: token is the only important field
snprintf(hdr->token, sizeof hdr->token, "%d", SUR_ERROR); 

// send error reply 
if (surWrite(sock, hdr, HdrMemSize) == -1)
	sryLog("%s: write error-%s\n", fn.c_str(), strerror(errno));
}
