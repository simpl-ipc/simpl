/******************************************************************************
FILE:			RS232_surrogate_R.cpp

DATE:			January 26, 2025

DESCRIPTION:	This program is run as a fork from RS232_surrogate.c

NOTES:			RS232_surrogate_R (parent) receives the following message:
				1. SUR_REQUEST_MSG

				RS232_surrogate_R (parent) replies the following message: 
				1. SUR_REQUEST_REPLY

AUTHOR:			John Collins
******************************************************************************/

// application header
#define _ALLOC extern
#include "RS232_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	void surrogate_R(void)

PURPOSE:	receives/replies messages from/to a Locate() call.
  
RETURNS:	nothing
**********************************************************************/	

void surrogate_R()
{
// Sender is global
const std::string &n = RS232_Surr_R;
pid_t pid;
SUR_REQUEST_MSG in;
SUR_REQUEST_REPLY out;

// deal with RS232_surrogate_r children ending ... no zombies
deadChildren();

// create an instance of SRY called 'nee' with receiver name 'n'
SRY nee(n);

while (true)
	{
	// create a RS232_surrogate_r for next time (keep one ahead)
	pid = fork();
	if (pid < 0) // fork failure
		sryLog("%s: fork error-%s\n", n.c_str(), strerror(errno));
	else if (pid == 0)
		surrogate_r(); // child
	
	// parent: message origin should be a remote Locate() call
	if (nee.Receive(&Sender, &in, sizeof in) == -1)
		{
		sryLog("%s: Receive error.\n", n.c_str());
		continue;
		}

	// check on the message token
	if (in.token != SUR_REQUEST)
		{
		sryLog("%s: Unknown message token=%d\n", n.c_str(), in.token);
		continue;
		}

	// set reply message - the pid of the RS232_surrogate_r child
	out.pid = pid;
	
	// reply to request
	if (nee.Reply(Sender, &out, sizeof out) == -1)
		{
		sryLog("%s: Reply error.\n", n.c_str());
		continue;
		}
	}
}
