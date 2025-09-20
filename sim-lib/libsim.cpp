/*******************************************************************************
FILE:			libsim.cpp/libsim.c

DATE:			February 6, 2025

DESCRIPTION:	This file contains the function calls for the SIMPL library used
				by application programs. 

AUTHOR:			John Collins
*******************************************************************************/

// sim headers 
#include <sim_lib.h>

// include C++ bits C++ compiler
#ifdef __cplusplus
/*#############################################################################
							Start SRY Class Methods
##############################################################################*/

/**********************************************************************
FUNCTION:	SRY::SRY(const std::string&)

PURPOSE:	Constructor.
***********************************************************************/

SRY::SRY(const std::string &name)
{
// openSRY() performs the sry setup
if (openSRY(name.c_str()) == -1)
	{
	closeSRY();
	exit(EXIT_FAILURE);
	}
	
// use the global sry object pointer to store the sry object address
sryObj = this;
}

/**********************************************************************
FUNCTION:	SRY::SRY(const char *)

PURPOSE:	Constructor.
***********************************************************************/

SRY::SRY(const char *name)
{
// openSRY() performs the sry setup
if (openSRY(name) == -1)
	{
	closeSRY();
	exit(EXIT_FAILURE);
	}

// use the global sry object pointer to store the sry object address
sryObj = this;
}

/**********************************************************************
FUNCTION:	SRY::~SRY(void)

PURPOSE:	Destructor.
***********************************************************************/

SRY::~SRY()
{
closeSRY();
}

/**********************************************************************
FUNCTION:	int SRY::Receive(void  **, void *, unsigned)

PURPOSE:	This method receives messages/proxies from other processes.
			It is a blocking receive.

RETURNS:	success: number bytes in message >= 0 or proxy < -1
			failure: -1
***********************************************************************/

int SRY::Receive(void **id, void *iPtr, unsigned iSize)
{
return (*ReceivePtr)(id, iPtr, iSize);
}

/**********************************************************************
FUNCTION:	int SRY::Reply(void *, void *, unsigned)

PURPOSE:	This method replies to sender process messages.

RETURNS:	success: >= 0, reply message size
			failure: -1
***********************************************************************/

int SRY::Reply(void *id, void *oPtr, unsigned oSize)
{
return (*ReplyPtr)(id, oPtr, oSize);
}

/**********************************************************************
FUNCTION:	int SRY::ReplyError(void *)

PURPOSE:	This method replies an error condition to a sender.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int SRY::ReplyError(void *id)
{
return (*ReplyErrorPtr)(id);
}

/**********************************************************************
FUNCTION:	SRY::returnProxy(int)

PURPOSE:	Return the true value of a received proxy.

RETURNS:	The value of the proxy should be >= 1.
			(the value entered should be <= -2)
***********************************************************************/

int SRY::returnProxy(int value)
{
return (*returnProxyPtr)(value);
}

/**********************************************************************
FUNCTION:	int SRY::Locate(const std::string&, const std::string&, int,
const int)

PURPOSE:	Gets the sry id of a receiver process, locally or remotely.

RETURNS:	success: sry id > 0
			failure: -1
***********************************************************************/

int SRY::Locate(const std::string &hname, const std::string &pname, int 
												msgSize, const int protocol)
{
return (*LocatePtr)(hname.c_str(), pname.c_str(), msgSize, protocol);
}

/**********************************************************************
FUNCTION:	int SRY::Locate(const char *, const char *, int, const int)

PURPOSE:	Gets the sry id of a receiver process, local or remotely.

RETURNS:	success: sry id > 0
			failure: -1
***********************************************************************/

int SRY::Locate(const char *hname, const char *pname, int msgSize, 
const int protocol)
{
return (*LocatePtr)(hname, pname, msgSize, protocol);
}

/**********************************************************************
FUNCTION:	int SRY::Send(int, void *, unsigned, void *, unsigned)

PURPOSE:	This method sends messages to other receiver processes.
			It is a blocking send.

RETURNS:	success: reply msg size >= 0
			failure: -1
***********************************************************************/

int SRY::Send(int id, void *oPtr, unsigned oSize, void *iPtr, unsigned iSize)
{
return (*SendPtr)(id, oPtr, oSize, iPtr, iSize);
}

/**********************************************************************
FUNCTION:	int SRY::PostMessage(int, void *, unsigned, void *, unsigned)

PURPOSE:	This method sends messages to other receiver processes.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int SRY::PostMessage(int id, void *oPtr, unsigned oSize, unsigned iSize)
{
return (*PostMessagePtr)(id, oPtr, oSize, iSize);
}

/**********************************************************************
FUNCTION:	int SRY::ReadReply(void *)

PURPOSE:	This method reads reply messages from other receiver processes.

RETURNS:	success: reply msg size >= 0
			failure: -1
***********************************************************************/

int SRY::ReadReply(void *iPtr)
{
return (*ReadReplyPtr)(iPtr);
}

/**********************************************************************
FUNCTION:	int SRY::Trigger(int, int)

PURPOSE:	This method kicks or triggers a receiver.
			It is non-blocking; that is it doesn't require a reply.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int SRY::Trigger(int id, int proxy)
{
return (*TriggerPtr)(id, proxy);
}

/**********************************************************************
FUNCTION:	int SRY::Relay(void *, int)

PURPOSE:	This method relays a message to a receiver.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int SRY::Relay(void *sender, int fd)
{
return (*RelayPtr)(sender, fd);
}

/**********************************************************************
FUNCTION:	int SRY::getSenderName(void *, std::string&)

PURPOSE:	This method returns the sender sim name after a Send() call. 

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int SRY::getSenderName(void *sender, std::string& senderName)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;
int ret = 0;

if (sender == nullptr)
	ret = -1;
else
	senderName = msgPtr->whom;

return ret;
}

/**********************************************************************
FUNCTION:	int SRY::getSenderPid(void *)

PURPOSE:	This method returns the sender pid after a Send() call. 

RETURNS:	success: pid_t
			failure: -1
***********************************************************************/

int SRY::getSenderPid(void *id)
{
return (*getSenderPidPtr)(id);
}

/**********************************************************************
FUNCTION:	int SRY::getSenderShmemSize(void *)

PURPOSE:	This method returns the sender shmem size in bytes after a Send() 
			call.

RETURNS:	success: >= 0
			failure: -1
***********************************************************************/

int SRY::getSenderShmemSize(void *id)
{
return (*getSenderShmemSizePtr)(id);
}

/**********************************************************************
FUNCTION:	int SRY::getSenderMsgSize(void *)

PURPOSE:	This method returns the sender msg size in bytes after a Send() 
			call.

RETURNS:	success: >= 0
			failure: -1
***********************************************************************/

int SRY::getSenderMsgSize(void *id)
{
return (*getSenderMsgSizePtr)(id);
}

/**********************************************************************
FUNCTION:	int SRY::getSenderRplySize(void *)

PURPOSE:	This method returns the sender reply msg size in bytes after a 
			Send() call.

RETURNS:	success: >= 0
			failure: -1
***********************************************************************/

int SRY::getSenderRplySize(void *id)
{
return (*getSenderRplySizePtr)(id);
}

/**********************************************************************
FUNCTION:	bool SRY::chkReceiver(const char *)

PURPOSE:	This method checks that a local receiver is still sry name
			attached.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

bool SRY::chkReceiver(const char *name, pid_t pid)
{
return (*chkReceiverPtr)(name, pid);
}

/**********************************************************************
FUNCTION:	bool SRY::chkSender(void *)

PURPOSE:	This method checks that a local sender is still running.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

bool SRY::chkSender(void *id)
{
return (*chkSenderPtr)(id);
}

/**********************************************************************
FUNCTION:	int SRY::rfd(void)

PURPOSE:	Return the file descriptor to the receive fifo.

RETURNS:	success: a file descriptor > 2
			failure: -1
***********************************************************************/

int SRY::rfd(void)
{
return (*rfdPtr)();
}

/**********************************************************************
FUNCTION:	int SRY::yfd(void)

PURPOSE:	Return the file descriptor to the reply fifo.

RETURNS:	success: a file descriptor > 2
			failure: -1
***********************************************************************/

int SRY::yfd(void)
{
return (*yfdPtr)();
}

/**********************************************************************
FUNCTION:	void simRcopy(void *, void *, unsigned)

PURPOSE:	Copy unsigned nbytes from the sender's shmem pointer
			to the desired address void *dst 

			Follows a Receiver(&id, NULL, 0) call.

RETURNS:	nothing
***********************************************************************/

void SRY::simRcopy(void *sender, void *dst, unsigned nBytes)
{
return (*simRcopyPtr)(sender, dst, nBytes);
}

/**********************************************************************
FUNCTION:	void simScopy(void *, unsigned)

PURPOSE:	Copy unsigned nbytes from the sender's global shmem pointer
			to the desired address void * 

			Follows a Send(id, void *, NULL, outSize, inSize) call

RETURNS:	nothing
***********************************************************************/

void SRY::simScopy(void *dst, unsigned nBytes)
{
return (*simScopyPtr)(dst, nBytes);
}

/**********************************************************************
FUNCTION:	int SRY::closeSRYchild(void)

PURPOSE:	Close SRY fifos and shmem of a child process.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int SRY::closeSRYchild(void)
{
return (*closeSRYchildPtr)();
}
/*#############################################################################
							End SRY Class Methods
##############################################################################*/
#endif

/*#############################################################################
							Start SRY C/C++ Functions
##############################################################################*/

/**********************************************************************
FUNCTION:	int openSRY(const char *)

PURPOSE:	Initializes SIM module.

NOTE:		This function takes a string argument which serves as a unique 
			identifier for a SIM enabled process. It is necessary for a SIM 
			enabled process to have a unique name so that it can be located by 
			name.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int openSRY(const char *name)
{
// WHO_AM_I SimParms is global
// char *SimFifoPath is global
// int RemoteReceiverId[] is global
// void *BlockedSenderId[] is global
const char *fn = "openSRY";
char *p = NULL;
int len = 0;
pid_t pid = -1;

// is this process already SIM enabled? 
if (sim_check() == true)
	{
	sryLog("%s: SIM already active for this process.\n", fn);
	return -1;
	}

// needed for a number of purposes such as unique naming, checking ...
SimParms.pid = getpid();

// set up the SIM name by checking the length of the passed in name 
len = strlen(name);
if (len == 0) // SIM name too short
	{
	sryLog("%s: SIM name too short.\n", fn);
	return -1;
	}
else if (len <= (MAX_SIM_NAME_LEN)) // acceptable name for SIM
	{
	strcpy(SimParms.whom, name);
	}
else // passed in name too long
	{
	sryLog("%s: SIM name too long.\n", fn);
	return -1;
	}

/*
set the global SimFifoPath
set to Default_Fifo_Path if SIM_FIFO_PATH not defined
*/
p = getenv("SIM_FIFO_PATH");
if (p == NULL)
	strcpy(SimFifoPath, DefaultFifoPath);
else
	{
	if (strlen(p) > MAX_FIFO_PATH_LEN)
		{
		sryLog("%s: exported SIM fifo path name too long.\n", fn);
		return -1;
		}
	sprintf(SimFifoPath, "%s", p);
	}

// check for access to fifo path
if (access(SimFifoPath, F_OK) == -1)
	{
	sryLog("%s: No SIM fifo path defined.\n", fn);
	return -1;
	}

// check if the name is already in use
pid = chkNamePid(name);
if (pid != -1)
	{
	// pid is real; is the program still running?
	if (chkStatus(pid, name) == true)
		{
		sryLog("%s: SIM name is already in use.\n", fn);
		return -1;
		}
	}

// name, create and open the receive and reply fifos
if (createFifos() == -1)
	{
	sryLog("%s: Fifo creation error\n", fn);
	return -1;
	}

// shmem size = 0 indicates shmem has not yet been made
SimParms.shmSize = 0;

// add a signal handler
initSignalHandling();

//add the exit functionality
atexit(exitFunc);

// initialize table of possible surrogates
for (int i = 0; i < MAX_NUM_REMOTE_RECEIVERS; i++)
	RemoteReceiverId[i] = -1;

// initialize table of reply-blocked senders
for (int i = 0; i < MAX_NUM_BLOCKED_SENDERS; i++)
	BlockedSenderId[i] = (void *)NULL;

return 0;
}

/**********************************************************************
FUNCTION:	int closeSRY(void)

PURPOSE:	Removes SIM functionality upon process exit or SRY cancelled
			SRY instantiation.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int closeSRY()
{
//const char *fn = "closeSRY";
// WHO_AM_I SimParms is global 
// int RemoteReceiverId[] is global
// void *BlockedSenderId[] is global
	
// is this process SIM enabled? 
if (sim_check() == false)
	{
//	sryLog("%s: No SIM name attached.\n", fn);
	return -1;
	}

// release any reply-blocked senders
for (int i = 0; i < MAX_NUM_BLOCKED_SENDERS; i++)
	{
	if (BlockedSenderId[i] != (void *)NULL)
		{
		ReplyError(BlockedSenderId[i]);
		BlockedSenderId[i] = (void *)NULL;
		}		
	}

// remove any surrogates
for (int i = 0; i < MAX_NUM_REMOTE_RECEIVERS; i++)
	{
	if (RemoteReceiverId[i] != -1)
		{
		Trigger(RemoteReceiverId[i], PROXY_SHUTDOWN);
		RemoteReceiverId[i] = -1;
		}		
	}

// delete message shared memory segment
if (SimParms.shmSize)
	detachShmem();

// delete receive and reply fifos
deleteFifos();

// for checking purposes
SimParms.pid = -1;

return 0;
}

/**********************************************************************
FUNCTION:	int closeSRYchild(void)

PURPOSE:	Removes a parent's SIM functionality from a child process.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int closeSRYchild()
{
// WHO_AM_I SimParms is global 
const char *fn = "closeSRYchild";
	
// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: No SIM name attached.\n", fn);
	return -1;
	}

// detach message shared memory segment
if (SimParms.shmSize)
	detachShmem();

// detach from the receive and reply fifos
detachFifos();

// for checking purposes
SimParms.pid = -1;

return 0;
}

/**********************************************************************
FUNCTION:	int Send(int, void *, unsigned, void *, unsigned)

PURPOSE:	This function sends SIM messages to other processes.

RETURNS:	success: number of bytes from Reply >= 0
			failure: -1
***********************************************************************/

int Send(int fd, void *outBuffer, unsigned outBytes, void *inBuffer, unsigned 
inBytes)
{
const char *fn = "Send";
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
unsigned bufSize = 0;
FCMSG_REC *msgPtr = NULL;
// WHO_AM_I SimParms is global 

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active\n", fn);
	return -1;
	}

// check the veracity of the fd
if (fd < 3)
	{
	sryLog("%s: SIM id is out of range.\n", fn);
	return -1;
	}

// calculate the largest buffer size for this messaging
bufSize = (outBytes >= inBytes) ? outBytes : inBytes;

// build shmem as needed
if (SimParms.shmSize < (bufSize + (unsigned)sizeof(FCMSG_REC)))
	{
	// delete any past shmem
	if (SimParms.shmSize)
		detachShmem();
	
	// create new shmem	
	if (createShmem(bufSize) == -1)
		{
		sryLog("%s: Create shmem error\n", fn);
		return -1;
		}
	}

// copy the message into shmem to be read by the receiver
msgPtr = (FCMSG_REC *)SimParms.shmPtr;
strcpy(msgPtr->whom, SimParms.whom);
msgPtr->pid = SimParms.pid;
msgPtr->shmsize = bufSize;
msgPtr->nbytes = outBytes;
msgPtr->ybytes = inBytes;
if (outBuffer != NULL)
	memcpy((void *)&msgPtr->data, outBuffer, outBytes);

// line up the triggering message for the fifo
fifoMsg->shmid = SimParms.shmid;

/*
sender writes on receiver's fifo
receiver will read the fifo and get sender's shmem id (schmid)
*/
if (write(fd, fifoBuf, sizeof(FIFO_MSG)) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Unable to write to fifo -%s\n", fn, strerror(errno));
	return -1;
	}

// wait for the receiver to send fifo message to trigger the reply
if (readFifoMsg(SimParms.yfd, fifoBuf) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Fifo read error\n", fn);
	close(SimParms.yfd);
	SimParms.yfd = -1;
	return -1;
	}

// was there a problem in the send such as a call to ReplyError()?
if (fifoMsg->shmid == -1)
	{
	sryLog("%s: receive/reply problem ... could be a failed receiver.\n", fn);
	return -1;
	}

// copy the reply message
if (inBuffer != NULL)
	if (msgPtr->nbytes)
		memcpy(inBuffer, (void *)&msgPtr->data, msgPtr->nbytes);

// return the size of the reply message
return msgPtr->nbytes;
}

/**********************************************************************
FUNCTION:	int PostMesssage(int, void *, unsigned, unsigned)

PURPOSE:	This function sends SIM messages to other processes, however
			it does not return a reply. It is essentially the "top half"
			of a Send().

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int PostMessage(int fd, void *outBuffer, unsigned outBytes, unsigned inBytes)
{
const char *fn = "PostMessage";
char fifoBuf[sizeof(FIFO_MSG)];
unsigned bufSize = 0;
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
FCMSG_REC *msgPtr = NULL;
// WHO_AM_I SimParms is global 

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active\n", fn);
	return -1;
	}

// check the veracity of the fd
if (fd < 3)
	{
	sryLog("%s: SIM id is out of range.\n", fn);
	return -1;
	}

// calculate the largest buffer size for this messaging
bufSize = (outBytes >= inBytes) ? outBytes : inBytes;

// build shmem as needed
if (SimParms.shmSize < (bufSize + (unsigned)sizeof(FCMSG_REC)))
	{
	// delete any past shmem
	if (SimParms.shmSize)
		detachShmem();
	
	// create new shmem	
	if (createShmem(bufSize) == -1)
		{
		sryLog("%s: Create shmem error\n", fn);
		return -1;
		}
	}

// copy the message into shmem to be read by the receiver
msgPtr = (FCMSG_REC *)SimParms.shmPtr;
strcpy(msgPtr->whom, SimParms.whom);
msgPtr->pid = SimParms.pid;
msgPtr->shmsize = bufSize;
msgPtr->nbytes = outBytes;
msgPtr->ybytes = inBytes;
if (outBuffer != NULL)
	memcpy((void *)&msgPtr->data, outBuffer, outBytes);

// line up the triggering message for the fifo
fifoMsg->shmid = SimParms.shmid;

/*
sender writes on receiver's fifo
receiver will read the fifo and get sender's shmem id (schmid)
*/
if (write(fd, fifoBuf, sizeof(FIFO_MSG)) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Unable to write to fifo -%s\n", fn, strerror(errno));
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	int ReadReply(void *)

PURPOSE:	This function receives SIM reply messages from other processes.
			It is essentially the "bottom half" of a Send(). Follows a call
			to PostMessage().

RETURNS:	success: size of replied message >=0
			failure: -1
***********************************************************************/

int ReadReply(void *inBuffer)
{
const char *fn = "ReadReply";
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
FCMSG_REC *msgPtr = NULL;
// WHO_AM_I SimParms is global 

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active\n", fn);
	return -1;
	}

// wait for the receiver to send fifo message to trigger the reply
if (readFifoMsg(SimParms.yfd, fifoBuf) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Fifo read error\n", fn);
	close(SimParms.yfd);
	SimParms.yfd = -1;
	return -1;
	}

// was there a problem in the send such as a call to ReplyError()?
if (fifoMsg->shmid == -1)
	{
	sryLog("%s: receive/reply problem ... could be a failed receiver.\n", fn);
	return -1;
	}

// line up on the reply message
msgPtr = (FCMSG_REC *)SimParms.shmPtr;

// check the size of the sender's reply buffer
if (sizeof inBuffer < msgPtr->nbytes)
	{
	sryLog("%s: buffer too small for reply.\n", fn);
	return -1;
	}

// is there something to copy from the shmem?
if (msgPtr->nbytes)
	memcpy(inBuffer, (void *)&msgPtr->data, msgPtr->nbytes);

// return the size of the reply message
return msgPtr->nbytes;
}

/**********************************************************************
FUNCTION:	int Trigger(int, int)

PURPOSE:	This function sends a proxy to a receiver type process.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int Trigger(int fd, int proxy)
{
const char *fn = "Trigger";
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active.\n", fn);
	return -1;
	}

// check proxy value
if (proxy < 1)
	{
	sryLog("%s: Improper proxy value, should be > 0.\n", fn);
	return -1;
	}

// negative value marks a proxy
fifoMsg->shmid = -proxy;

if (write(fd, fifoBuf, sizeof(FIFO_MSG)) != sizeof(FIFO_MSG))
	{
	sryLog("%s: unable to write to fifo -%s\n", fn, strerror(errno));
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	int Receive(void **, void *, unsigned)

PURPOSE:	This function receives SIM messages from other processes.

RETURNS:	success: message size in bytes
			failure: -1
***********************************************************************/

int Receive(void **sender, void *inBuffer, unsigned maxBytes)
{
const char *fn = "Receive";
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
FCMSG_REC *msgRec = NULL;
// WHO_AM_I SimParms is global 
// int SimSenderShmid is global

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active.\n", fn);
	return -1;
	}

// reset the SimSenderShmid, needed by Relay()
SimSenderShmid = -1;

// wait on the fifo for a triggering message from a sending process
if (readFifoMsg(SimParms.rfd, fifoBuf) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Fifo read error.\n", fn);
	close(SimParms.rfd);
	SimParms.rfd = -1;
	return -1;
	} 

// set the global for sender's shmid for Relay to work
SimSenderShmid = fifoMsg->shmid;

// is the message a proxy?
if (fifoMsg->shmid < 0)
	return (-1 + fifoMsg->shmid); // -2 or less (shmid is already negative)

/*
Attach the sender's shmem to this process.
Known to fail if sender suddenly disappears. 
Saving this value allows the Reply() to use the same shmem.
*/
*sender = (void *)shmat(fifoMsg->shmid, (void *)0, 0);
if (*sender == (void *)-1)
	{
	sryLog("%s: shmid=%d cannot attach to shmem-%s\n", fn, fifoMsg->shmid, 
															strerror(errno));
	ReplyError(*sender);
	return -1;
	}

// line up on the message
msgRec = (FCMSG_REC *)*sender;

// copy the data out of the shmem or not?
if (inBuffer != NULL)
	{
	// is the message larger than the receiver's message buffer?
	if (msgRec->nbytes > maxBytes)
		{
		sryLog("%s: message size %d > buffer size %d\n", fn, msgRec->nbytes, 
																	maxBytes);
		ReplyError(*sender);
		return -1;
		}

	// copy the message 
	memcpy(inBuffer, (void *)&msgRec->data, msgRec->nbytes);
	}

// save this sender in case of failure before a reply can made
saveSenderId(*sender);

// return the size of the message
return msgRec->nbytes;
}

/**********************************************************************
FUNCTION:	int Reply(void *, void *, unsigned)

PURPOSE:	This function replies SIM messages to sender processes.

RETURNS:	success: number of reply bytes (nbytes) >= 0
			failure: -1
***********************************************************************/

int Reply(void *sender, void *outBuffer, unsigned nbytes)
{
const char *fn = "Reply";
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
FCMSG_REC *msgPtr = NULL;
char fifoName[256];
int fd = -1, ret = -1;
// WHO_AM_I SimParms is global 
// char *SimFifoPath is global
// int SimSenderShmid is global

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active.\n", fn);
	return -1;
	}

// set a pointer to the sender's shmem
msgPtr = (FCMSG_REC *)sender;

// set the sender's reply fifo path and name
sprintf(fifoName, "%s/Y_%s.%d", SimFifoPath, msgPtr->whom, msgPtr->pid);

// check that sender's reply buffer is large enough
if (nbytes > msgPtr->ybytes)
	{
	// set up fifo trigger message for error
	fifoMsg->shmid = -1;
	// set error
	sryLog("%s: Reply message too large.\n", fn);
	}
else
	{
	// set up fifo trigger message for success 
	fifoMsg->shmid = 0;
	// set the reply message header
	msgPtr->nbytes = nbytes;
	// copy the reply message into sender's shmem
	if (outBuffer != NULL)
		memcpy((void *)&msgPtr->data, outBuffer, nbytes);
	ret = nbytes;
	}

// detach sender's shmem from this process
shmdt(sender);

// open the sender's fifo
fd = open(fifoName, O_WRONLY);
if (fd == -1)
	{
	sryLog("%s: Unable to open fifo-%s.\n", fn, strerror(errno));
	return -1;
	}

// write the fifo message to unblock the sender
if (write(fd, fifoBuf, sizeof(FIFO_MSG)) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Unable to write to fifo.\n", fn);
	close(fd);
	return -1;
	}

// close the sender's fifo 
close(fd);

// reset the SimSenderShmid
SimSenderShmid = -1;

// remove this sender that was saved in case of a failure
removeSenderId(sender);

return ret;
}

/**********************************************************************
FUNCTION:	int ReplyError(void *)

PURPOSE:	This function replies an error condition to a reply-blocked
			sender.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int ReplyError(void *sender)
{
const char *fn = "ReplyError";
char fifoName[MAX_FIFO_PATH_LEN + MAX_PROGRAM_NAME_LEN + 10];
int fd = -1;
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
FCMSG_REC *msgPtr = NULL;
// WHO_AM_I SimParms is global 
// char *SimFifoPath is global

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active.\n", fn);
	return -1;
	}
	
// remove this sender from the reply-blocked sender table
removeSenderId(sender);

// line up on the fifo message
msgPtr = (FCMSG_REC *)sender;

// set the fifo path and name
sprintf(fifoName, "%s/Y_%s.%d", SimFifoPath, msgPtr->whom, msgPtr->pid);

// detach sender's shmem from this process
shmdt(sender);

// set up fifo message, -1 indicates an error condition
fifoMsg->shmid = -1;

// open the fifo
fd = open(fifoName, O_WRONLY);
if (fd == -1)
	{
	sryLog("%s: Fifo %s open failure - %s.\n", fn, fifoName, strerror(errno));
	return -1;
	}

// write the fifo trigger message
if (write(fd, fifoBuf, sizeof(FIFO_MSG)) != sizeof(FIFO_MSG))
	{
	sryLog("%s: Fifo write failure -%s\n", fn, strerror(errno));
	close(fd);
	return -1;
	}

// close the fifo 
close(fd);

// if we got this far the message has been sent and return success
return 0;
}

/**********************************************************************
FUNCTION:	int Relay(void *, int)

PURPOSE:	This function relays a SIM message to another process.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int Relay(void *sender, int fd)
{
const char *fn = "Relay";
char fifoBuf[sizeof(FIFO_MSG)];
FIFO_MSG *fifoMsg = (FIFO_MSG *)fifoBuf;
// WHO_AM_I SimParms is global 
// int sim_sender_shmid is global

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active.\n", fn);
	return -1;
	}
	
fifoMsg->shmid = SimSenderShmid;

// write the fifo message to unblock the sender
if (write(fd, fifoBuf, sizeof(FIFO_MSG)) != sizeof(FIFO_MSG))
	{
	sryLog("%s: unable to write to fifo\n", fn);
	close(fd);
	return -1;
	}
else
	{
	// remove this sender that was saved in case of failure
	removeSenderId(sender);

	// reset the _simpl_sender_shmid
	SimSenderShmid = -1;

	// detach sender's shmem from this process
	shmdt(sender);
	}

return 0;
}

/**********************************************************************
FUNCTION:	int returnProxy(int)

PURPOSE:	Return the true value of a received proxy.

RETURNS:	The true value of a proxy should be >= 1.
			(the value entered should be <= -2)
***********************************************************************/

int returnProxy(int value)
{
return (value > -2) ? -1 : abs(value + 1);
}

/**********************************************************************
FUNCTION:	int Locate(const char *, const char *, int, const int)

PURPOSE:	Main purpose is to decide whether to use local or remote
			name locating based on the type of host name (hName) string,
			if any.
			Returns the fd of the receive fifo of a sim receiver.

RETURNS:	success: >= 0
			failure: -1
***********************************************************************/

int Locate(const char *hostName, const char *processName, int msgSize,
		 												  const int protocol)
{
const char *fn = "Locate";
int rc = -1;
pid_t pid = -1;

// is this process SIM enabled? 
if (sim_check() == false)
	{
	sryLog("%s: SIM not active.\n", fn);
	return -1;
	}

// there must at least be a destination process name
if (strlen(processName) == 0)
	{
	sryLog("%s: Missing receiver's SIM name.\n", fn);
	return -1;
	}

/*
Local name locate: receiver treated as on same local host
*/

// no entry in host field indicates a process on local host
if (strlen(hostName) == 0)
	{
	char fifoName[128];

	// find the receiver's fifo
	rc = getFifoName(processName, fifoName);
	if (rc == -1)
		{	
		sryLog("%s: Unable to find receiver's fifo.\n", fn);
		return -1;
		}

// check if the receiver SIM name is in use
pid = chkNamePid(processName);
if (pid == -1)
	{
	sryLog("%s: receiver SIM name not in use.\n", fn);
	return -1;
	}

// pid is real; is the receiver program still running?
if (chkStatus(pid, processName) == false)
	{
	sryLog("%s: receiver not SIM enabled.\n", fn);
	return -1;
	}

// open the fifo for triggering message passing
rc = open(fifoName, O_WRONLY);
if (rc == -1)
	sryLog("%s: %s\n", fn, strerror(errno));
    // leave Locate, done with local needs
	return rc;
	}

/*
 Remote name locate: receiver treated as on remote host
*/

/*
host name indicates that process is not local or is a surrogate loopback test
using local host
*/
SUR_REQUEST_MSG lout;
SUR_REQUEST_REPLY lin;
SUR_NAME_LOCATE_MSG rout;
SUR_NAME_LOCATE_REPLY rin;
char surrogateParent[MAX_SIM_NAME_LEN + 1];
char surrogateChild[MAX_SIM_NAME_LEN + 1];
int wid = -1;
unsigned i = 0;
// int RemoteReceiverId[] is global

// choose the communication protocol (TCP/IP, RS232 etc.)
switch (protocol)
	{
	case SIM_RS232:
		sprintf(surrogateParent, "%s", RS232_Surr_R);
		break;

	case SIM_LOCAL:
	case SIM_TCP:
	default:
		sprintf(surrogateParent, "%s", TCP_Surr_R);
		break;
	}
	
// do a local name locate for *_surrogate_R
// Note: the * indicates the surrogate protocol such as TCP etc.
if ((wid = Locate("", surrogateParent, 0, protocol)) == -1)
	{
	sryLog("%s: surrogate R name locate failure.\n", fn);
	return -1;
	}

// send request to surrogate_R for a local *_surrogate_r child
lout.token = SUR_REQUEST;
if (Send(wid, &lout, sizeof lout, &lin, sizeof lin) == -1)
	{
	sryLog("%s: surrogate R send failure.\n", fn);
	return -1;
	}

// set the name of the *_surrogate_r program
switch (protocol)
	{
	case SIM_RS232:
		sprintf(surrogateChild, "%s%d", RS232_Surr_r, lin.pid);
		break;

	case SIM_LOCAL:
	case SIM_TCP:
	default:
		sprintf(surrogateChild, "%s%d", TCP_Surr_r, lin.pid);
		break;
	}

//printf("remote locate: local name=%s\n", surrogateChild);

// do a local name locate for *_surrogate_r_pid
if ((rc = Locate("", surrogateChild, 0, protocol)) == -1)
	{
	sryLog("%s: surrogate r name locate failure.\n", fn);
	return -1;
	}

//printf("remote locate: found local %s\n", surrogateChild);

// make char based msg to send to surrogate_r_pid
snprintf(rout.hdr.token, sizeof rout.hdr.token, "%d", SUR_NAME_LOCATE);
strcpy(rout.hostName, hostName);
strcpy(rout.sryName, processName);
snprintf(rout.maxSize, sizeof rout.maxSize, "%d", msgSize);

//printf("remote locate: begin send to %s\n", surrogateChild);

// send remote name locate msg to surrogate_r_pid
if (Send(rc, &rout, sizeof rout, &rin, sizeof rin) == -1)
	{
	sryLog("%s: r send failure.\n", fn);
	return -1;
	}

//printf("remote locate: finished send to %s\n", surrogateChild);

// convert result back to binary
if (atoi(rin.result) == -1)
	{
	sryLog("%s: remote locate failure.\n", fn);
	return -1;
	}

// add to the remote receiver offset table
for (i = 0; i < MAX_NUM_REMOTE_RECEIVERS; i++)
	{
	if (RemoteReceiverId[i] == -1)
		{
		RemoteReceiverId[i] = rc;
		break;
		}
	}

// the above loop didn't break ==> no room on table
if (i == MAX_NUM_REMOTE_RECEIVERS)
	{
	sryLog("%s: no room on the remote receiver table.\n", fn);
	return -1;
	}

return rc;
}

/**********************************************************************
FUNCTION:	bool chkSender(void *sender)

PURPOSE:	Checks on sender's existence. Called by a receiver between 
			Receive() and Reply().

RETURNS:	sender exists: true
			sender does not exist: false
***********************************************************************/

bool chkSender(void *sender)
{
FCMSG_REC *rec = (FCMSG_REC *)sender;

return chkStatus(rec->pid, rec->whom);
} 

/**********************************************************************
FUNCTION:	bool chkReceiver(const char *receiverSIMname, pid_t pid)

PURPOSE:	Can be used between multiple sends to the same receiver.

RETURNS:	receiver exists: true
			receiver does not exist: false
***********************************************************************/

bool chkReceiver(const char *name, pid_t pid)
{
bool ret = true;

if (!pid)
	pid = chkNamePid(name);

if (pid != -1)
	{
	// pid is real; is the program still running?
	if (chkStatus(pid, name) == false)
		{
		ret = false;
		}
	}

return ret;
}

/**********************************************************************
FUNCTION:	int getSenderName(void *, char *senderName)

PURPOSE:	Returns the local sender's sim name after a Send() call.

NOTE:		If a garbage pointer is passed in, a garbage value will be returned.
			As well, make certain that the pointer senderName that is passed
			in points to a character array that is large enough to accomodate 
			the the sim name or the memcpy will fail.

RETURNS:	sender exists: set sender's sim name and return 0 
			sender shmem ptr is NULL: -1
***********************************************************************/

int getSenderName(void *sender, char *senderName)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;
int ret = 0;

if (sender == NULL)
	ret = -1;
else
	memcpy(senderName, msgPtr->whom, MAX_PROGRAM_NAME_LEN);

return ret;
}

/**********************************************************************
FUNCTION:	pid_t getSenderPid(void *)

PURPOSE:	Returns the value of the local sender's pid after a Send() call.

NOTE:		If a garbage pointer is passed in, a garbage value will be returned.

RETURNS:	sender exists: pid_t
			sender shmem ptr is NULL: -1
***********************************************************************/

pid_t getSenderPid(void *sender)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;
pid_t ret;

if (sender == NULL)
	ret = -1;
else
	ret = msgPtr->pid;

return ret;
}

/**********************************************************************
FUNCTION:	int getSenderShmemSize(void *)

PURPOSE:	Returns the value of the local sender's shmem size in bytes after a 
			Send() call.

NOTE:		If a garbage pointer is passed in, a garbage value will be returned.

RETURNS:	sender exists: int
			sender shmem ptr is NULL: -1
***********************************************************************/

pid_t getSenderShmemSize(void *sender)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;
int ret = -1;

if (sender == NULL)
	ret = -1;
else
	ret = msgPtr->shmsize;

return ret;
}

/**********************************************************************
FUNCTION:	int getSenderMsgSize(void *)

PURPOSE:	Returns the value of the local sender's message size in bytes after 
			a Send() call.

NOTE:		If a garbage pointer is passed in, a garbage value will be returned.

RETURNS:	sender exists: int
			sender shmem ptr is NULL: -1
***********************************************************************/

int getSenderMsgSize(void *sender)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;
int ret;

if (sender == NULL)
	ret = -1;
else
	ret = msgPtr->nbytes;

return ret;
}

/**********************************************************************
FUNCTION:	int getSenderRplySize(void *)

PURPOSE:	Returns the value of the local sender's reply message size in bytes 
			after a Send() call.

NOTE:		If a garbage pointer is passed in, a garbage value will be returned.

RETURNS:	sender exists: int
			sender shmem ptr is NULL: -1
***********************************************************************/

int getSenderRplySize(void *sender)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;
int ret;

if (sender == NULL)
	ret = -1;
else
	ret = msgPtr->ybytes;

return ret;
}

/**********************************************************************
FUNCTION:	int rfd(void)

PURPOSE:	Return the file descriptor to the receive fifo

RETURNS:	success: a file descriptor > 2
			failure: -1
***********************************************************************/

int rfd(void)
{
// WHO_AM_I SimParms is global 
return SimParms.rfd;
}

/**********************************************************************
FUNCTION:	int yfd(void)

PURPOSE:	Return the file descriptor to the reply fifo

RETURNS:	success: a file descriptor > 2
			failure: -1
***********************************************************************/

int yfd(void)
{
// WHO_AM_I SimParms is global
return SimParms.yfd;
}

/**********************************************************************
FUNCTION:	void simRcopy(void *, void *, unsigned)

PURPOSE:	Copy unsigned nbytes from the sender's shmem pointer
			to the desired address void *dst 

			Follows a Receiver(&id, NULL, 0) call.

RETURNS:	nothing
***********************************************************************/

void simRcopy(void *sender, void *dst, unsigned nbytes)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)sender;

memcpy(dst, (void *)&msgPtr->data, nbytes);
} 
 
/**********************************************************************
FUNCTION:	void simScopy(void *, unsigned)

PURPOSE:	Copy unsigned nbytes from the sender's global shmem pointer
			to the desired address void * 

			Follows a Send(id, void *, NULL, outSize, inSize) call

RETURNS:	nothing
***********************************************************************/

void simScopy(void *dst, unsigned nbytes)
{
FCMSG_REC *msgPtr = (FCMSG_REC *)SimParms.shmPtr;

memcpy(dst, (void *)&msgPtr->data, nbytes);
}

/********************************************************************/
/****************** MESSAGE SHARED MEMORY FUNCTIONS *****************/
/********************************************************************/

/**********************************************************************
FUNCTION:	int createShmem(unsigned)

PURPOSE:	Create and attach shared memory used for a message passing
			buffer.

RETURNS:	success: 0
			failure: -1
**********************************************************************/

int createShmem(unsigned bufSize)
{
const char *fn = "createShmem";
unsigned memSize = 0;
int i = 0;
long pagesize = 0;
// WHO_AM_I SimParms is global

// calculate the shmem size based on the PAGE_SIZE
pagesize = sysconf(_SC_PAGESIZE);

// note that the loop limit is merely to prevent runaway
for (memSize = pagesize, i = 1; i <= 10000; i++, memSize += pagesize)
	if ( (bufSize + sizeof(FCMSG_REC)) <= memSize )
		break;

// create shmem for messages
SimParms.shmid = shmget(IPC_PRIVATE, memSize, 0777 | IPC_CREAT);
if (SimParms.shmid == -1)
	{
	sryLog("%s: Unable to create shmem-%s.\n", fn, strerror(errno));
	return -1;
	}

// attach the shmem for messages to this process
SimParms.shmPtr = (void *)shmat(SimParms.shmid, (void *)0, 0);
if ((void *)SimParms.shmPtr == (void *)-1)
	{
	sryLog("%s: Unable to attach shmem to process-%s\n",fn, 
															strerror(errno));
	return -1;
	}

// release shmem if and when owner dies by an untrappable signal
shmctl(SimParms.shmid, IPC_RMID, 0);

// this the maximum size for a message
SimParms.shmSize = memSize;

return 0;
}

/**********************************************************************
FUNCTION:	int detachShmem(void)

PURPOSE:	Detach shared memory used for a message passing	buffer.

RETURNS:	success: 0
			failure: -1
**********************************************************************/	

int detachShmem()
{
const char *fn = "detachShmem";
// WHO_AM_I SimParms is global

// detach shmem
if (shmdt(SimParms.shmPtr) == -1)
	{
	sryLog("%s: Unable to detach shmem-%s.\n", fn, strerror(errno));
	return -1;
	}

// this the maximum size for a message, used as a check
SimParms.shmSize = 0;

return 0;
}

/********************************************************************/
/************************* FIFO FUNCTIONS ***************************/
/********************************************************************/

/**********************************************************************
FUNCTION:	int createFifos(void)

PURPOSE:	Name, create and open receive and reply fifos.

RETURNS:	success (valid fifo fd): 0
			failure (no valid fifo): -1
***********************************************************************/

int createFifos()
{
const char *fn = "createFifos";
/*
more space than required, typically a path and name
such as "/var/tmp/simName.12345" is usual
*/
char rname[MAX_FIFO_PATH_LEN + MAX_PROGRAM_NAME_LEN + 10];
char yname[MAX_FIFO_PATH_LEN + MAX_PROGRAM_NAME_LEN + 10];
// WHO_AM_I SimParms is global; partly populated in openSRY()
// char *SimFifoPath is global; set in openSRY()

// receive fifo
sprintf(rname, "%s/R_%s.%d", SimFifoPath, SimParms.whom, SimParms.pid);
// reply fifo
sprintf(yname, "%s/Y_%s.%d", SimFifoPath, SimParms.whom, SimParms.pid);

// create receive fifo
if (mkfifo(rname, 0666) == -1)
	{
	sryLog("%s: Unable to create fifo %s-%s.\n", fn, rname, strerror(errno));
	return -1;
	}
// create reply fifo
if (mkfifo(yname, 0666) == -1)
	{
	sryLog("%s: Unable to create fifo %s-%s.\n", fn, yname, strerror(errno));
	return -1;
	}

// receive fifo; masks the mode 0666 with user's umask
if (chmod(rname, 0666) == -1)
	{
	sryLog("%s: Unable to chmod fifo %s-%s.\n", fn, rname, strerror(errno));
	return -1;
	}
// reply fifo; masks the mode 0666 with user's umask
if (chmod(yname, 0666) == -1)
	{
	sryLog("%s: Unable to chmod fifo %s-%s.\n", fn, yname, strerror(errno));
	return -1;
	}

// open receive fifo, close when not needed 
SimParms.rfd = open(rname, O_RDWR);
if (SimParms.rfd == -1)
	{
	sryLog("%s: Unable to open fifo %s-%s.\n", fn, rname, strerror(errno));
	return -1;
	}
// open reply fifo for use, close when not needed 
SimParms.yfd = open(yname, O_RDWR);
if (SimParms.yfd == -1)
	{
	sryLog("%s: Unable to open fifo %s-%s.\n", fn, yname, strerror(errno));
	return -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	int detachFifos(void)

PURPOSE:	Close receive and reply fifo file descriptors.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int detachFifos()
{
// WHO_AM_I SimParms is global; populated in openSRY()

// close the receive fifo
if (SimParms.rfd != -1)
	{
	close(SimParms.rfd);
	SimParms.rfd = -1;
	}

// close the reply fifo
if (SimParms.yfd != -1)
	{
	close(SimParms.yfd);
	SimParms.yfd = -1;
	}

return 0;
}

/**********************************************************************
FUNCTION:	int deleteFifos(void)

PURPOSE:	Remove receive and reply fifos.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int deleteFifos()
{
//const char *fn = "deleteFifos";
/*
more space than required, typically a path and name
such as "/var/tmp/simName.12345" is usual
*/
char rname[MAX_FIFO_PATH_LEN + MAX_PROGRAM_NAME_LEN + 10];
char yname[MAX_FIFO_PATH_LEN + MAX_PROGRAM_NAME_LEN + 10];
// WHO_AM_I SimParms is global; populated in openSRY()
// char *SimFifoPath is global; set early on in openSRY()

// receive fifo
sprintf(rname, "%s/R_%s.%d", SimFifoPath, SimParms.whom, SimParms.pid);
// reply fifo
sprintf(yname, "%s/Y_%s.%d", SimFifoPath, SimParms.whom, SimParms.pid);

// close Receive and replY fifos
detachFifos();

// check for presence of receive fifo
if (access(rname, F_OK) == 0)
	{
	// delete the receive fifo
	if (remove(rname) == -1)
		{
		/*
		sryLog("%s: Unable to remove fifo %s-%s.\n", fn, rname, 
														strerror(errno));
		return -1;
		*/
		}
	}		

// check for presence of reply fifo
if (access(yname, F_OK) == 0)
	{
	// delete the reply fifo
	if (remove(yname) == -1)
		{
		/*
		sryLog("%s: Unable to remove fifo %s-%s.\n", fn, yname, 
														strerror(errno));
		return -1;
		*/
		}
	}
	
return 0;
}

/**********************************************************************
FUNCTION:	int readFifoMsg(int, char *)

PURPOSE:	read any pending bytes from specified fifo fd.

RETURNS:	success: sizeof FIFO_MSG
			failure: != sizeof FIFO_MSG
**********************************************************************/	

int readFifoMsg(int fd, char *buf)
{
int numBytes = 0, bytesToGo = 0, rc = 0;
char *p = buf;

for (int i = 0; i < 10; i++)
	{
	bytesToGo = sizeof(FIFO_MSG) - numBytes;
	if (bytesToGo <= 0)
		break;
	rc = read(fd, p, bytesToGo);
	numBytes += rc;
	p += rc;
	}

return numBytes;
}

/**********************************************************************
FUNCTION:	int getFifoName(const char *, char *)

PURPOSE:	Find a receive fifo based on the sim name.

RETURNS:	success: 0 
			failure: -1
***********************************************************************/

int getFifoName(const char *simName, char *fifoName)
{
const char *fn = "getFifoName";
DIR *directory = NULL;
struct dirent *file = NULL;
int len = 0, ret = -1;
char entryName[128];
// char *SimFifoPath is global

// open the fifo directory
directory = opendir(SimFifoPath);
if (directory == NULL)
	{
	sryLog("%s: Unable to open fifo directory %s.\n", fn, SimFifoPath);
	return -1;
	}

// the fifo directory file name based on the SIM name passed in 
sprintf(entryName, "R_%s", simName); 
len = strlen(entryName);

// check for a match
while ((file = readdir(directory)) != NULL)
	{
	// check for a match
	if (file->d_name[len] == '.' && !memcmp(file->d_name, entryName, len))
		{
		// sim names match
		sprintf(fifoName, "%s/%s", SimFifoPath, file->d_name);
		ret = 0;
		break;
		}
	}

closedir(directory);

return ret;
}

/********************************************************************/
/************************* SIGNAL FUNCTIONS *************************/
/********************************************************************/

/**********************************************************************
FUNCTION:	initSignalHandling(void)

PURPOSE:	Set up any necessary signal handling. 

RETURNS:	nothing
***********************************************************************/

void initSignalHandling()
{
struct sigaction sigact;

// initialize the sigact structure
memset(&sigact, 0, sizeof sigact);

// set the signal handler
sigact.sa_handler = hndlSignals;

// the signals to trap
sigaction(SIGHUP, &sigact, NULL);
sigaction(SIGILL, &sigact, NULL);
sigaction(SIGABRT, &sigact, NULL);
sigaction(SIGINT, &sigact, NULL);
sigaction(SIGSEGV, &sigact, NULL);
sigaction(SIGTERM, &sigact, NULL);
sigaction(SIGPIPE, &sigact, NULL);
}

/**********************************************************************
FUNCTION:	void hndlSignals(int)

PURPOSE:	Called as a response to a trapped signal.

RETURNS:	nothing

NOTE:		Called by initSignalHandling().
***********************************************************************/

void hndlSignals(int signo)
{
// log the signal
//sryLog("received signal %d\n", signo);

// react to signal type
switch (signo)
	{
	case SIGPIPE:
		//do nothing
		break;

	default:
		// clean up SIM stuff
		closeSRY();

		// don't call atexit() because we have already run closeSRY()
		_exit(EXIT_SUCCESS);
	}
}

/********************************************************************/
/********************* MISCELLANEOUS FUNCTIONS **********************/
/********************************************************************/

/**********************************************************************
FUNCTION:	bool sim_check(void)

PURPOSE:	an indicator that the calling process is indeed registered
			as a SIM process.

RETURNS:	success: 0
			failure: -1
**********************************************************************/	

inline bool sim_check()
{
// WHO_AM_I SimParms is global
return (SimParms.pid == -1 ? false : true);
}

/**********************************************************************
FUNCTION:	void exitFunc(void)

PURPOSE:	Clean up when process exits. Called by atexit() and as the
			signal handler.

RETURNS:	nothing
***********************************************************************/

inline void exitFunc()
{
// separate function in order to conveniently allow added bits as necessary
closeSRY();
}

/**********************************************************************
FUNCTION:	int sryLog(const char *, ...)

PURPOSE:	sry errors/messages are recorded to a text file that is
			not	allowed to grow past a certain limit.

RETURNS:	int
**********************************************************************/

int sryLog(const char *format, ...)
{
const char *logFile = LogFile;
int fd = -1;
struct stat s;
char tstr[50], dstr[100];
va_list args;

// set time string
time_t tod = time(NULL);
strftime(tstr, sizeof tstr, "%D %T", gmtime(&tod));

// set comment string
va_start(args, format);
vsnprintf(dstr, sizeof dstr, format, args);
va_end(args);

// print to screen?
if (PrintSimError)
	printf("%s *** %s\n", tstr, dstr);

if (stat(logFile, &s) == -1) // file does not exist
	{
	fd = open(logFile, O_WRONLY | O_CREAT, 0644);
	}
else // file exists
	{
	if (s.st_size < MaxLogSize)
		fd = open(logFile, O_WRONLY | O_APPEND, 0644); // append
	else
		fd = open(logFile, O_WRONLY | O_TRUNC | O_CREAT, 0644); // truncate
	}

if (fd != -1)
	{
	if (write(fd, tstr, strlen(tstr)) == -1)
		return -1;
	if (write(fd, " *** ", 5) == -1)
		return -1;
	if (write(fd, dstr, strlen(dstr)) == -1)
		return -1;
	close(fd);
	}
else
	return -1;
	
return 0;
}

/**********************************************************************
FUNCTION:	int saveSenderId(void *)

PURPOSE:	This function adds a reply blocked sender to the global
			array.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int saveSenderId(void *sender)
{
// void *BlockedSenderId[] is global

// take the first available slot
for (int i = 0; i < MAX_NUM_BLOCKED_SENDERS; i++)
	{
	if (BlockedSenderId[i] == (void *)NULL)
		{
		BlockedSenderId[i] = sender;
		return 0;
		}
	}

return -1;
}

/**********************************************************************
FUNCTION:	int removeSenderId(void *)

PURPOSE:	This function removes a reply blocked sender from the global
			array.

RETURNS:	success: 0
			failure: -1
***********************************************************************/

int removeSenderId(void *sender)
{
// void *BlockedSenderId[] is global

// initialize table of reply-blocked senders
for (int i = 0; i < MAX_NUM_BLOCKED_SENDERS; i++)
	{
	if (BlockedSenderId[i] == sender)
		{
		BlockedSenderId[i] = (void *)NULL;
		return 0;
		}
	}

return -1;
}

/**********************************************************************
FUNCTION:	int getLocalHostName(char *)

PURPOSE:	Gets the local host's name either from a set environment
			variable or the hostname file.

RETURNS:	success: = 0
			failure: -1
***********************************************************************/

int getLocalHostName(char *name)
{
const char *fn = "getLocalHostName";
char *host = NULL;
int rc = 0,fd= -1, ch = 0;
const char *hostFile = "/etc/hostname", *environVar = "HOSTNAME";

// initialize potential local host name
memset(name, 0, MAX_HOST_NAME_LEN + 1);
// get the host name from an environment variable
host = getenv(environVar);
if (host == NULL)
	{
	// look up the hostname in the /etc/hostname file
	fd = open(hostFile, O_RDONLY);
	if (fd == -1)
		{
		rc = -1;
		sryLog("%s: No system host file.\n", fn);
		}
	else
		{
		ch = read(fd, name, MAX_HOST_NAME_LEN);
		if (ch <= 0)
			{
			rc = -1;
			sryLog("%s: No system hots name in host file.\n", fn);
			}

		// if present, get rid of the EOF
		if (name[ch-1] == 0xa)
			name[ch-1] = 0;
		close(fd);
		}
	}
else
	{
	if (strlen(host) > MAX_HOST_NAME_LEN)
		{
		rc = -1;
		sryLog("%s: System host name too long.\n", fn);
		}
	else
		strcpy(name, host);
	}

return rc;
}

/**********************************************************************
FUNCTION:	pid_t chkNamePid(const char *name)

PURPOSE:	return the SIM process' pid if it exists based on its SIM name

RETURNS:	receiver exists, pid >= 0
			receiver does not exist, pid = -1
 ***********************************************************************/

pid_t chkNamePid(const char *name)
{
const char *fn = "chkName";
DIR *directory = NULL;
struct dirent *file = NULL;
int len = 0;
pid_t pid = -1;
char entryName[128];
// char *SimFifoPath is global

// open the fifo directory
directory = opendir(SimFifoPath);
if (directory == NULL)
	{
	sryLog("%s: Unable to open fifo directory %s.\n", fn, SimFifoPath);
	return -1;
	}

// the length of the sim name passed in + 1
// form of fifo name is R_simName.12345
sprintf(entryName, "R_%s", name);
len = strlen(entryName);

// check fifo directory entries for a match
while ( (file = readdir(directory)) != NULL )
	{
	// check for a match
	if (file->d_name[len] == '.' && !memcmp(file->d_name, entryName, len))
		{
		// extract the pid; fifo name is of the form: R_name.12345
		pid = atoi(file->d_name + len + 1);
		break;
		}
	}

closedir(directory);

return pid;
}

/**********************************************************************
FUNCTION:	bool chkStatus(const pid_t pid, const char *name)

PURPOSE:	return the SIM process' status based on its process id

RETURNS:	SIM program exists, true
			SIM program does not exist, false
***********************************************************************/

bool chkStatus(const pid_t pid, const char *name)
{
//const char *fn = "chkStatus";
bool ret = true;
char fifoFile[400];
// char *SimFifoPath is global

if (pid > 0)
	{
	// errno must be cleared prior to getpriority() call
	errno = 0;
	if ( (getpriority(PRIO_PROCESS, pid) == -1) && (errno == ESRCH) )
		{
		// this program is not running so remove old fifos if they exist
		sprintf(fifoFile, "%s/R_%s.%d", SimFifoPath, name, pid);
		remove(fifoFile);
		sprintf(fifoFile, "%s/Y_%s.%d", SimFifoPath, name, pid);
		remove(fifoFile);
		// process doesn't exist'
		ret = false;
		}
	}

return ret;
}
/*#############################################################################
							End SRY C/C++ Functions
##############################################################################*/
