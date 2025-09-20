/******************************************************************************
FILE:			sim_lib.h

DATE:			February 6, 2025

DESCRIPTION:	This file contains definitions used by the SIMPL
				library libsim.cpp and its link libsim.c.

AUTHOR:			John Collins
******************************************************************************/

#pragma once

// C system includes
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/stat.h>

// sim headers 
#include <sim.h>
#include <sim_array_defs.h>
#include <surrMsgs.h>

// global constants
static const char *DefaultFifoPath = "/var/tmp";
static const char *LogFile = "/var/tmp/sry.log";
static const int MaxLogSize = 102400; //100k

typedef struct
	{
    char whom[MAX_PROGRAM_NAME_LEN + 1];// SIM name
	pid_t pid;			// program pid
	int rfd;			// receive fifo fd
	int yfd;			// reply fifo fd
	int shmid;			// shared memory id
	void *shmPtr;		// pointer to shared memory location; sender id
	unsigned shmSize;	// size of shared memory
	} WHO_AM_I;

// must be kept atomic
typedef struct
	{
	int shmid;
	} FIFO_MSG;

typedef struct
	{
    char whom[MAX_PROGRAM_NAME_LEN + 1];
	pid_t pid;
	unsigned shmsize;
	unsigned nbytes;
	unsigned ybytes;
	char data;
	} FCMSG_REC;

// sry globals
WHO_AM_I SimParms = {"", -1, -1, -1, -1, (void *)NULL, 0};
int RemoteReceiverId[MAX_NUM_REMOTE_RECEIVERS];
void *BlockedSenderId[MAX_NUM_BLOCKED_SENDERS];
char SimFifoPath[MAX_FIFO_PATH_LEN + 1];
volatile int SimSenderShmid = -1;
bool PrintSimError = false;

// shared memory functions
int createShmem(unsigned);
int detachShmem(void);

// trigger fifo functions
int createFifos(void);
int detachFifos(void);
int deleteFifos(void);
int readFifoMsg(int, char *);
int getFifoName(const char *, char *);

// called and miscellaneous functions
bool sim_check(void);
void initSignalHandling(void);
void hndlSignals(int);
void exitFunc(void);
int saveSenderId(void *);
int removeSenderId(void *);
int getLocalHostName(char *);
pid_t chkNamePid(const char *);
bool chkStatus(const pid_t, const char *);

// include C++ bits C++ compiler
#ifdef __cplusplus
/*
C++ SRY class member function pointers

These allow a C++ library (SRY class methods) to be built from C functions 
which also provide the basis for a standalone C library.
*/

static int (*ReceivePtr)(void **, void *, unsigned) = Receive;
static int (*ReplyPtr)(void *, void *, unsigned) = Reply;
static int (*ReplyErrorPtr)(void *) = ReplyError;
static int (*returnProxyPtr)(int) = returnProxy;
static bool (*chkReceiverPtr)(const char *, pid_t) = chkReceiver;
static bool (*chkSenderPtr)(void *) = chkSender;
static int (*LocatePtr)(const char *, const char *, int, const int) = Locate;
static int (*SendPtr)(int, void *, unsigned, void *, unsigned) = Send;
static int (*PostMessagePtr)(int, void *, unsigned, unsigned) = PostMessage;
static int (*ReadReplyPtr)(void *) = ReadReply;
static int (*TriggerPtr)(int, int) = Trigger;
static int (*RelayPtr)(void *, int) = Relay;
static pid_t (*getSenderPidPtr)(void *) = getSenderPid;
static int (*getSenderShmemSizePtr)(void *) = getSenderShmemSize;
static int (*getSenderMsgSizePtr)(void *) = getSenderMsgSize;
static int (*getSenderRplySizePtr)(void *) = getSenderRplySize;
static int (*rfdPtr)(void) = rfd;
static int (*yfdPtr)(void) = yfd;
static void (*simRcopyPtr)(void *, void *, unsigned) = simRcopy;
static void (*simScopyPtr)(void *, unsigned) = simScopy;
static int (*closeSRYchildPtr)(void) = closeSRYchild;

// C++ global variable
static SRY *sryObj;
#endif
