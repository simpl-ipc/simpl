/*******************************************************************************
FILE:			sim.h

DATE:			January 23, 2025

DESCRIPTION:	This file contains the SRY class definition of sim with the
				function members contained in full in libsim.cpp. It should be 
				included in any application source code using SIMPL.

AUTHOR:			John Collins
*******************************************************************************/

#pragma once

#include <unistd.h> 	// for pid_t
#include <stdbool.h>	// true/false

#ifdef __cplusplus
#include <string>

// SRY class definition. 
/*
Note that the SRY class is designed to be a singleton class, one 
instantiation to a customer!
Also, it cannot (at this time) act as a base class.
*/

class SRY final // no base class
{
private:
	SRY(const SRY&) = delete;				// no copy
	SRY &operator=(const SRY&) = delete;	// no assignment

public:
	SRY(const char *);			// default constructor, C string type
	SRY(const std::string&);	// constructor, C++ string type

	int Receive(void **, void *, unsigned);
	int Reply(void *, void *, unsigned);
	int ReplyError(void *);
	int returnProxy(int);
	int Locate(const std::string&, const std::string&, int, const int);
	int Locate(const char *, const char *, int, const int);
	int Send(int, void *, unsigned, void *, unsigned);
	int PostMessage(int, void *, unsigned, unsigned);
	int ReadReply(void *);
	int Trigger(int, int);
	int Relay(void *, int);
	int getSenderName(void *, std::string&);
	pid_t getSenderPid(void *);
	int getSenderShmemSize(void *);
	int getSenderMsgSize(void *);
	int getSenderRplySize(void *);
	bool chkReceiver(const char *, pid_t);
	bool chkSender(void *);
	int rfd(void);
	int yfd(void);
	void simRcopy(void *, void *, unsigned);
	void simScopy(void *, unsigned);
	int closeSRYchild(void);

	~SRY();	// destructor	
};
#endif

// C functions
int openSRY(const char *);
int closeSRY(void);
int Receive(void **, void *, unsigned);
int Reply(void *, void *, unsigned);
int ReplyError(void *);
int returnProxy(int);
int Locate(const char *, const char *, int, const int);
int Send(int, void *, unsigned, void *, unsigned);
int PostMessage(int, void *, unsigned, unsigned);
int ReadReply(void *);
int Trigger(int, int);
int Relay(void *, int);
int getSenderName(void *, char *);
pid_t getSenderPid(void *);
int getSenderShmemSize(void *);
int getSenderMsgSize(void *);
int getSenderRplySize(void *);
bool chkReceiver(const char *, pid_t);
bool chkSender(void *);
int rfd(void);
int yfd(void);
void simRcopy(void *, void *, unsigned);
void simScopy(void *, unsigned);
int closeSRYchild(void);

// general functions
int sryLog(const char *, ...);

// supported communication protocols
typedef enum
	{
	SIM_LOCAL = 0,
	SIM_TCP,
	SIM_RS232,
	MAX_PROTOCOLS
	} SURR_PROTOCOLS;
