/*******************************************************************************
FILE:			TCP_surrogate.h

DATE:			January 23, 2025

DESCRIPTION:	This file contains necessary headers, definitions and global 
				variables for the TCP surrogate files.

AUTHOR:			John Collins
*******************************************************************************/

#pragma once

// standard headers
#include <cstdlib>
#include <string>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// sim headers
#include <sim.h>
#include <surrMsgs.h>

// function prototypes within TCP_surrogate.c
int initialize(int, char **);

// function prototypes within TCP_surrogate_R.c
void surrogate_R(void);

// function prototypes in TCP_surrogate_rr.c
void surrogate_r(void);
int hndlRemoteNameLocate(SRY&);
int hndlLocalMsg(int, SRY&);
int hndlRemoteMsg(int, SRY&);
int sendKeepAliveMsg(int);
int hndlProxy(int, int);

// function prototypes within TCP_surrogate_S.c
void surrogate_S(void);

// function prototypes within TCP_surrogate_ss.c
void surrogate_s(int);
int nameLocateReply(int, long, pid_t);
int hndlMsg(int, long, SRY&);
int hndlKeepAlive(int);
void errorReply(int);

// function prototypes within TCP_surrogateUtils.c
int initRsock(struct sockaddr_in *);
int initSsock(char *);
int surRead(int, void *, unsigned);
int surWrite(int, void *, unsigned);
void deadChildren(void);
int adjustMemory(int);
int setMsgBuffer(unsigned);
void hndlClose(int);

// global constants - default values
const unsigned SurrPortDefault = 50001;		// TCP/IP port
const time_t NamelocTimeoutDefault = 60;	// fail time on nameloc request (s) 
const time_t ChkTimeoutDefault = 60;		// period of check (s)
const unsigned KaFailLimitDefault = 3;		// allowed keep alive failures
const unsigned MaxMsgMemSizeDefault = 8192;	// 8 kbytes; due to kernel part

// other global constants
const int MaxPendingConnections = 25;		// socket listen backlog

// global variables which are set at TCP_surrogate start up
_ALLOC unsigned SurrPort;
_ALLOC time_t NamelocTimeout;
_ALLOC time_t ChkTimeout;
_ALLOC unsigned KaFailLimit;
_ALLOC unsigned MaxMsgMemSize;
_ALLOC bool KaFlag;

// other global variables
_ALLOC unsigned KaCtr;
_ALLOC pid_t KaPid;
_ALLOC void *Sender;

#ifdef _TCP_SURROGATE_PRIMARY
	_ALLOC char *TotMem = nullptr;
	_ALLOC unsigned TotMemSize = 0;
	_ALLOC char *HdrMem = nullptr;
	_ALLOC unsigned HdrMemSize = 0;
	_ALLOC char *MsgMem = nullptr;
	_ALLOC unsigned MsgMemSize = 0;
#else
	_ALLOC char *TotMem;
	_ALLOC unsigned TotMemSize;
	_ALLOC char *HdrMem;
	_ALLOC unsigned HdrMemSize;
	_ALLOC char *MsgMem;
	_ALLOC unsigned MsgMemSize;
#endif
