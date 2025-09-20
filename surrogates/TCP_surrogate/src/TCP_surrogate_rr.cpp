/*******************************************************************************
FILE:			TCP_surrogate_rr.cpp

DATE:			January 23, 2025

DESCRIPTION:	This program is run as a fork from surrogate_R.c

AUTHOR:			John Collins

NOTES:			receives the following messages/proxies:
				1. SUR_NAME_LOCATE (from local sender)
				2. SUR_SEND (from local sender)
				3. SUR_PROXY (from local sender)
				4. PROXY_SHUTDOWN (from local sender)
				5. SUR_ALIVE_REPLY (from remote surrogate_S)

				sends the following messages/proxies:
				1. SUR_NAME_LOCATE (to remote surrogate_S and surrogate_s)
				2. SUR_SEND (to remote surrogate_s)
				3. SUR_PROXY (to remote surrogate_s)
				4. SUR_CLOSE (to remote surrogate_s)
				5. SUR_ALIVE (to remote surrogate_S)
				6. reply messages back to the local sender.
*******************************************************************************/

// application header
#define _ALLOC extern
#include "TCP_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	surrogate_r(void)

PURPOSE:	Talk via tcp to the surrogate_S	process on another host
			to perform a remote name_locate() of a receiver process on that
			host. If such a process exists, then become a surrogate
			receiver and tcp sender to a remote surrogate tcp receiver
			and sender on that other host thus acting as one side of a
			conduit between a local sender and a remote receiver.

RETURNS:	nothing, it is a forked process and never returns.
**********************************************************************/	

void surrogate_r()
{
// KaTimeout, KaCtr, Sender are global
std::string n = TCP_Surr_r, &p = n;
int fds[2], maxFd, ret, sock;
fd_set watchset, inset;
struct timeval tv, *timeoutPtr;

// set the SIM name of this forked program
p += std::to_string(getpid());

// close connections to parent's (TCP_surrogate_R's) SIM
closeSRYchild();

// create a new instance of SRY
SRY nee(p);
SRY &noo = nee;

// a remote name locate message will be the first thing to happen
if ((sock = hndlRemoteNameLocate(noo)) == -1)
	{
	sryLog("%s: name locate error.\n", p.c_str());
	exit(EXIT_FAILURE);
	}

// for local sry messages 
fds[0] = nee.rfd();

// for remote surrogate messages; sock set in hndlRemoteNameLocate() above 
fds[1] = sock;

// set select parameters
FD_ZERO(&watchset);
FD_SET(fds[0], &watchset);
FD_SET(fds[1], &watchset);
maxFd = (fds[0] > fds[1]) ? fds[0] + 1 : fds[1] + 1;

while (true)
	{
	inset = watchset;
	tv.tv_sec = ChkTimeout;
	tv.tv_usec = 0;
	timeoutPtr = &tv;

	// let select be the trigger on the file descriptors/timer
	ret = select(maxFd, &inset, NULL, NULL, timeoutPtr);
	if (ret > 0) // local or remote message coming in
		{
		if (FD_ISSET(fds[0], &inset))
			{
			// this is a local simpl Send message
			if (hndlLocalMsg(sock, noo) == -1)
				{
				sryLog("%s: hndlLocalMsg error.\n", p.c_str());
				exit(EXIT_FAILURE);
				}
			}
		else if (FD_ISSET(fds[1], &inset))
			{
			// send reply or keep alive reply from the remote surrogate 
			if (hndlRemoteMsg(sock, noo) == -1)
				{
				sryLog("%s: hndlRemoteMsg error.\n", p.c_str());
				exit(EXIT_FAILURE);
				}
			}
		else 
			{
			// unknown fd error
			sryLog("%s: unknown fd on select-%s.\n", p.c_str(),strerror(errno));
			}
		}
	else if (ret == 0) // timer
		{
		// check on the local sender
		if (nee.chkSender(Sender) == false)
			{
			// sender has gone and somehow left this surrogate behind
			// one posibility is that the sender was SIGKILL'd
			// send a close message to the remote surrogate partner
			hndlClose(sock);
			exit(EXIT_SUCCESS);
			}

		// keep alive on?
		if (KaFlag)
			{
			if (sendKeepAliveMsg(sock) == -1)
				{
				sryLog("%s: sendKeepAliveMsg error.\n", p.c_str());
				exit(EXIT_FAILURE);
				}
			}
		}
	else // select error
		{
		sryLog("%s: select error-%s.\n", p.c_str(), strerror(errno));
		}
	}
}

/**********************************************************************
FUNCTION:	hndlRemoteNameLocate(SRY&)

PURPOSE:	Handle a simpl SUR_NAME_LOCATE call.
			Should only be called once for each surrogate child.
			Doesn't use global message memory.

RETURNS:	int
**********************************************************************/	

int hndlRemoteNameLocate(SRY &c)
{
// Sender, KaPid are global
const std::string fn("hndlRemoteNameLocate");
fd_set watchset; 
struct timeval tv, *timeoutPtr;
int ret, sock;
SUR_NAME_LOCATE_MSG msg;
SUR_NAME_LOCATE_REPLY rep, failure;
const int mSize = sizeof (SUR_NAME_LOCATE_MSG);
const int ySize = sizeof (SUR_NAME_LOCATE_REPLY);
const int fail = -1;

// in case it is needed
snprintf(failure.result, sizeof failure.result, "%d", fail);

// receive name locate message from the local sender
if (c.Receive(&Sender, &msg, mSize) == -1)
	{
	sryLog("%s: receive error.\n", fn.c_str());
	return -1;
	}

// check the token
if (atoi(msg.hdr.token) != SUR_NAME_LOCATE)
	{
	sryLog("%s: incorrect message type.\n", fn.c_str());
	c.Reply(Sender, &failure, ySize);
	return -1;
	}

// make socket for sending remote name locate and local sender messages
if ((sock = initSsock(msg.hostName)) == -1)
	{
	sryLog("%s: message socket creation error.\n", fn.c_str());
	c.Reply(Sender, &failure, ySize);
	return -1;
	}

// ensure socket ownership
if (fcntl(sock, F_SETOWN, getpid()) == -1)
	{
	sryLog("%s: message socket ownership error-%s.\n", fn.c_str(), 
														strerror(errno));
	c.Reply(Sender, &failure, ySize);
	return -1;
	}

// send the name locate message to the other host
if (surWrite(sock, &msg, mSize) == -1)
	{
	sryLog("%s: write error on name locate msg-%s.\n", fn.c_str(), 
														strerror(errno));
	c.Reply(Sender, &failure, ySize);
	return -1;
	}

// block and wait for response 
FD_ZERO(&watchset);
FD_SET(sock, &watchset);

// set time out on wait for response
tv.tv_sec = NamelocTimeout;
tv.tv_usec = 0;
timeoutPtr = &tv;

ret = select(sock + 1, &watchset, NULL, NULL, timeoutPtr);
if (ret > 0) 
	{
	if (FD_ISSET(sock, &watchset))
		{
		// read the reply to the SUR_NAME_LOCATE (-1 fail !=-1 success)
		if (surRead(sock, (char *)&rep, ySize) == -1)
			{
			sryLog("%s: read error on name locate reply-%s.\n", fn.c_str(), 
					strerror(errno));
			c.Reply(Sender, &failure, ySize);
			return -1;
			}

		// reply the search results to the ipc sender
		if (c.Reply(Sender, &rep, ySize) == -1)
			{
			sryLog("%s: reply error.\n", fn.c_str());
			return -1;
			}

		// nobody home
		if (atoi(rep.result) == -1)
			return -1;

		// may use the pid for keep alive msg
		KaPid = atoi(rep.pid);
		}
	else
		{
		// unknown fd error
		sryLog("%s: unknown fd on select-%s.\n", fn.c_str(), strerror(errno));
		}
	}
else if (ret == 0) 
	{
	// select timer kick returns 0 ==> have not received a name locate reply
	// no error log; network/hostprocess simply not there or timed out
	c.Reply(Sender, &failure, ySize);
	return -1;
	}
else
	{
	// select error
	sryLog("%s: select error-%s.\n", fn.c_str(), strerror(errno));
	c.Reply(Sender, &failure, ySize);
	return -1;
	}

// check for adequate msg buffer
if (setMsgBuffer(atoi(msg.maxSize)) == -1)
	{ 
	sryLog("%s: memory allocation error.\n", fn.c_str());
	c.Reply(Sender, &failure, ySize);
	return -1;
	}

return sock;
}

/**********************************************************************
FUNCTION:	hndlLocalMsg(int, SRY&)

PURPOSE:	Handle simpl SUR_SEND call from a local sender intended
			for a remote receiver.

RETURNS:	int

**********************************************************************/	

int hndlLocalMsg(int sock, SRY &c)
{
// MsgMem, MsgMemSize, HdrMemSize, Sender are global
const std::string fn("hndlLocalMsg");
int msgSize, proxy;
SUR_MSG_HDR *hdr = (SUR_MSG_HDR *)HdrMem;

// receive message from the local sender
msgSize = c.Receive(&Sender, MsgMem, MsgMemSize);
if (msgSize == -1)
	{
	sryLog("%s: receive error.\n",fn.c_str());
	return -1;
	}

// is it a trigger proxy?
if (msgSize < -1)
	{
	// get the value of the proxy
	proxy = c.returnProxy(msgSize);
	
	// is it a close up shop proxy?
	if (proxy == PROXY_SHUTDOWN)
		{
		// send a close message to the remote surrogate partner
		hndlClose(sock);
		exit(EXIT_SUCCESS);
		}
	else
		{
		return hndlProxy(sock, proxy);
		}
	}

// build the message going to the other box	
snprintf(hdr->token, sizeof hdr->token, "%d", SUR_SEND);
snprintf(hdr->nbytes, sizeof hdr->nbytes, "%d", msgSize);
snprintf(hdr->ybytes, sizeof hdr->ybytes, "%d", c.getSenderRplySize(Sender));

// send the message via the socket to the other box
if (surWrite(sock, TotMem, HdrMemSize + msgSize) == -1)
	{
	sryLog("%s: write error on msg-%s.\n", fn.c_str(), strerror(errno));
	c.ReplyError(Sender);
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	hndlRemoteMsg(int, SRY&)

PURPOSE:	Handle incoming tcp/ip messages from the remote surrogate. 

RETURNS:	int
**********************************************************************/	

int hndlRemoteMsg(int sock, SRY &c)
{
// MsgMem, HdrMem, HdrMemSize, Sender, KaCtr are global
const std::string fn("hndlRemoteMsg");
int ybytes;
SUR_MSG_HDR *hdr = (SUR_MSG_HDR *)HdrMem;

// socket reply message header from other box
if (surRead(sock, hdr, HdrMemSize) == -1)
	{
	sryLog("%s: read error on msg header-%s.\n", fn.c_str(), strerror(errno));
	c.ReplyError(Sender);
	return -1;
	}			

// reaction based on the message token
switch (atoi(hdr->token))
	{
	case SUR_REPLY:
		// socket reply message data from other box
		ybytes = atoi(hdr->ybytes);

		if (surRead(sock, MsgMem, ybytes) == -1)
			{
			sryLog("%s: read error on msg data-%s.\n", fn.c_str(), 
															strerror(errno));
			c.ReplyError(Sender);	
			return -1;
			}

		// reply message to local ipc sender
		if (c.Reply(Sender, MsgMem, ybytes) == -1)
			{
			sryLog("%s: reply error-%s.\n", fn.c_str());
			return -1;
			}
		break;		  

	case SUR_CLOSE:
		exit(EXIT_SUCCESS);

	case SUR_ALIVE_REPLY:
		KaCtr = 0;
		break;

	case SUR_ERROR:
		c.ReplyError(Sender);	
		// surrogate sender always dies upon this error ==> we are not needed
		exit(EXIT_FAILURE);

	default:
		sryLog("%s: unknown message token.\n", fn.c_str());
		return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	sendKeepAliveMsg(int)

PURPOSE:	Send a keep alive message to the surrogate partner's parent.

RETURNS:	int
**********************************************************************/	

int sendKeepAliveMsg(int sock)
{
// TotMem, KaCtr, KaPid, KaFailLimit are global
const std::string fn("sendKeepAliveMsg");
SUR_KA_MSG *ka = (SUR_KA_MSG *)TotMem;
const int kaSize = sizeof (SUR_KA_MSG), nbytes = sizeof ka->kaPid;

// compose the keep alive message
snprintf(ka->hdr.token, sizeof ka->hdr.token, "%d", SUR_ALIVE);
snprintf(ka->hdr.nbytes, sizeof ka->hdr.nbytes, "%d", nbytes);
snprintf(ka->hdr.ybytes, sizeof ka->hdr.ybytes, "%d", nbytes);
snprintf(ka->kaPid, sizeof ka->kaPid, "%d", KaPid);

// send the keep alive message to the other surrogate's parent
if (surWrite(sock, ka, kaSize) == -1)
	{
	sryLog("%s: write error-%s.\n", fn.c_str(), strerror(errno));
	return -1;
	}

// increment the number of keep alive messages sent
KaCtr += 1;

// check the number of keep alive messages sent
if (KaCtr > KaFailLimit)
	{
	sryLog("%s: too many keep alive failures.\n", fn.c_str());
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	hndlProxy(int)

PURPOSE:	Handle sending a proxy to the remote surrogate_s.

RETURNS:	int
**********************************************************************/	

int hndlProxy(int sock, int proxy)
{
// TotMem is global
const std::string fn("hndlProxy");
SUR_PROXY_MSG *prox = (SUR_PROXY_MSG *)TotMem;
const int size = sizeof (SUR_PROXY_MSG), nbytes = sizeof prox->proxy;

// set up proxy message
snprintf(prox->hdr.token, sizeof prox->hdr.token, "%d", SUR_PROXY); 
snprintf(prox->hdr.nbytes, sizeof prox->hdr.nbytes, "%d", nbytes);
// prox->hdr.ybytes unnecessary since proxies have no reply
snprintf(prox->proxy, sizeof prox->proxy, "%d", proxy); 

// send the message via the socket to the other box
if (surWrite(sock, prox, size) == -1)
	{
	sryLog("%s: write error on proxy msg-%s.\n", fn.c_str(), strerror(errno));
	return -1;
	}

return 0;
}
