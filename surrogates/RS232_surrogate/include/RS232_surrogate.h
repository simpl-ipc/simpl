/*******************************************************************************
FILE:			RS232_surrogate.h

DATE:			January 26, 2025

DESCRIPTION:	This file contains necessary headers, definitions and global 
				variables for the RS232 surrogate files.

AUTHOR:			John Collins
*******************************************************************************/

#pragma once

// standard headers
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

// sim headers
#include <sim.h>
#include <surrMsgs.h>

// for dynamic memory allocations
typedef enum
	{
	IN,
	OUT
	} MSG_TYPE;

// function prototypes within RS232_surrogate.c
int initialize(int, char **);

// function prototypes within RS232_surrogate_R.c
void surrogate_R(void);
//pid_t forkSurrogate(void);
//int sendSurrogateMsg(int, pid_t);

// function prototypes within RS232_surrogate_S.c
void surrogate_S(void);

// function prototypes within RS232_surrogate_r.c
void surrogate_r(void);
int hndlRemoteNameLocate(SRY&);
int hndlMsg(SRY&, int *);
int sendKeepAliveMsg(SRY&, int *);
void hndlClose(SRY&);
int hndlProxy(SRY&, int);
void replyFailure(SRY&, char *);

// function prototypes within RS232_surrogate_s.c
void surrogate_s(void);
int hndlMessage(SRY&, int, int *, int *);
int hndlReply(SRY&, int);
void errorReply(SRY&);

// function prototypes within surrogateUtils.c
int checkMemory(int, int);
void deadChildren(void);

// global constants -default values
const time_t NamelocTimeoutDefault = 60;// (sec) fail time on nameloc request 
const time_t KaTimeoutDefault = 10;		// (sec) 0 implies no keep alive
const time_t KaResponseTimeoutDefault = 3;	// acknowledge wait time (sec)

// global constants - initialized
const int KaFailLimit = 3;				// allowed keep alive failures
const time_t ResponseTimeout = 10;		// acknowledge wait time (sec)
const int MaxSurrogates = 50;			// limit on the forked surrogates

// globals
_ALLOC void *Sender;
_ALLOC int SurRpid;
_ALLOC int SurSpid;
_ALLOC int SerialWriter;

_ALLOC time_t NamelocTimeout;
_ALLOC time_t KaTimeout;

_ALLOC time_t KaResponseTimeout;

#ifdef _RS232_SURROGATE_PRIMARY
	_ALLOC char *InMsgArea = nullptr;	 // used for incoming messages
	_ALLOC int InMsgSize = 0;
	_ALLOC char *OutMsgArea = nullptr;	// used for outgoing messages
	_ALLOC int OutMsgSize = 0;
#else
	_ALLOC char *InMsgArea;
	_ALLOC int InMsgSize;
	_ALLOC char *OutMsgArea;
	_ALLOC int OutMsgSize;
#endif
