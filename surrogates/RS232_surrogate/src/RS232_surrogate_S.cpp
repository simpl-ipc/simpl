/*******************************************************************************
FILE:			RS232_surrogate_S.cpp

DATE:			January 26, 2025

DESCRIPTION:	This program runs as a fork from RS232_surrogate.cpp.

NOTES:			surrogate_S (parent) receives the following messages:
				1. SUR_NAME_LOCATE_MSG

AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _ALLOC extern
#include "RS232_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	surrogate_S()

PURPOSE:	Acts as a receiver for remote Locate() messages.

RETURNS:	void
**********************************************************************/	

void surrogate_S()
{
// InMsgArea is global
// SerialWriter is global
const std::string &n = TCP_Surr_S;
int token;
pid_t childPid;
void *sender;
SUR_MSG_HDR *hdr;

// deal with surrogate_s children ending
deadChildren();

// create an instance of SRY called 'nee' with receiver name 'n'
SRY nee(n);

// global message memory for parent, set to largest message encountered
if (checkMemory(IN, 1024) == -1)
	{
	sryLog("%s: check memory error-%s.\n", n.c_str(), strerror(errno));
	exit(EXIT_FAILURE);
	}

while (true)
	{
	// read the message, should be a SUR_NAME_LOCATE_MSG from rs232_r program
	if (nee.Receive(&sender, InMsgArea, sizeof(SUR_NAME_LOCATE_MSG)) == -1)
		{
		sryLog("%s: receive error.\n", n.c_str());
		exit(EXIT_FAILURE);
		}

	// an empty reply to the serial reader
	if (nee.Reply(sender, nullptr, 0) == -1)
		{
		sryLog("%s: reply error.\n");
		exit(EXIT_FAILURE);
		}

	// interpret message header
	hdr = (SUR_MSG_HDR *)InMsgArea;
	token = atoi(hdr->token);

	if (token == SUR_NAME_LOCATE)
		{
		// fork child (surrogate_s) to handle messaging
		childPid = fork();
		if (childPid < 0) // forking failure
			{
			sryLog("%s: fork error-%s.\n", n.c_str(), strerror(errno));
			exit(EXIT_FAILURE);
			}
		else if (childPid == 0) // child
			{
			// child will have original message in its copy of InMsgArea
			surrogate_s();
			}
		}
	else
		{
		sryLog("%s: unknown msg token=%d.\n", n.c_str(), token);
		}
	}
}
