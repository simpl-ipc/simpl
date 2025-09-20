/*******************************************************************************
FILE:			surrMsgs.h

DATE:			January 23, 2025

DESCRIPTION:	This file contains surrogate message definitions.

AUTHOR:			John Collins
*******************************************************************************/

#pragma once

#include <sim_array_defs.h>

#define	PROXY_SHUTDOWN		0x7FFFFFF1
#define CHAR_BASE			12 // bytes

typedef enum 
	{
	SUR_NAME_LOCATE,
	SUR_SEND,
	SUR_REPLY,
	SUR_CLOSE,
	SUR_PROXY,
	SUR_ERROR,
	SUR_ALIVE,
	SUR_ALIVE_REPLY,
	SUR_REQUEST,
	MAX_SURR_TOKENS
	} SURR_MSG_TOKENS;

// locally sent to surrogate_R	
typedef struct // locally sent and received
	{
	int token;
	} SUR_REQUEST_MSG;

// locally replied from surrogate_R	
typedef struct
	{
	pid_t pid;
	} SUR_REQUEST_REPLY;

// for sent/received/replied messages
typedef struct
	{
	char token[CHAR_BASE];		
	char nbytes[CHAR_BASE];
	char ybytes[CHAR_BASE];
	char surPid[CHAR_BASE];
	} SUR_MSG_HDR;	

// locally sent to surrogate_r and forwarded to remote surrogate_S
typedef struct
	{
	SUR_MSG_HDR hdr;
	// host location of the remote receiver
	char hostName[MAX_HOST_NAME_LEN + 1];
	// remote receiver's sry name
	char sryName[MAX_SIM_NAME_LEN + 1];
	// largest message size sent and/or replied in bytes
	char maxSize[CHAR_BASE];
	} SUR_NAME_LOCATE_MSG;

// from remote surrogate_s to local surrogate_r and locally replied 	
typedef struct
	{
	char result[CHAR_BASE];
	char pid[CHAR_BASE];
	} SUR_NAME_LOCATE_REPLY;

typedef struct
	{
	SUR_MSG_HDR hdr;
	char proxy[CHAR_BASE];
	} SUR_PROXY_MSG;

typedef struct
	{
	SUR_MSG_HDR hdr;
	char kaPid[CHAR_BASE];
	} SUR_KA_MSG;

typedef struct
	{
	SUR_MSG_HDR hdr;
	char result[CHAR_BASE];
	} SUR_KA_REPLY_MSG;
