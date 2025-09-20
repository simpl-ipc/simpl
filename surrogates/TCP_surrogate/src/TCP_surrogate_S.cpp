/*******************************************************************************
FILE:			TCP_surrogate_S.cpp

DATE:			January 23, 2025

DESCRIPTION:	This program runs as a fork from TCP_surrogate.cpp.

AUTHOR:			John Collins

NOTES:			surrogate_S (parent) receives the following messages:
				1. SUR_NAME_LOCATE_MSG
*******************************************************************************/

// application header
#define _ALLOC extern
#include "TCP_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	void surrogate_S(void)

PURPOSE:	Acts as a receiver for remote Locate() messages.

RETURNS:	nothing
**********************************************************************/	

void surrogate_S()
{
// MsgMem is global
const std::string &n = TCP_Surr_S;
fd_set watchset, inset; 
int sock0, sock1;
pid_t pid;
SUR_NAME_LOCATE_MSG *ptr;
socklen_t sock_len = sizeof (struct sockaddr_in);
struct sockaddr_in rserver;

// deal with surrogate_s children ending
deadChildren();

// allocate memory for the largest expected message
if (setMsgBuffer(sizeof (SUR_NAME_LOCATE_MSG)))
	{
	sryLog("%s: Unable to allocate dynamic message buffer.\n", n.c_str());
	exit(EXIT_FAILURE);
	}

// must be after msgMem is set above
ptr = (SUR_NAME_LOCATE_MSG *)MsgMem;

// make socket for receiving remote SUR_NAME_LOCATE/SUR_ALIVE queries
sock0 = initRsock(&rserver);
if (sock0 == -1)
	{
	sryLog("%s: Socket creation error on port=%u.\n", n.c_str(), SurrPort);
	exit(EXIT_FAILURE);
	}

// arm select for TCP connections
FD_ZERO(&watchset);
FD_SET(sock0, &watchset);

while (true)
	{
	// reset inset every time
	inset = watchset;

	if (select(sock0 + 1, &inset, NULL, NULL, NULL) < 0)
		{
		sryLog("%s: select error-%s.\n", n.c_str(), strerror(errno));
		continue;
		}	

	// socket message from an external surrogate: accept this connection
	sock1 = accept(sock0, (struct sockaddr *)&rserver, &sock_len);
	if (sock1 < 0)
		{
		sryLog("%s: accept error-%s.\n", n.c_str(), strerror(errno));
		exit(EXIT_FAILURE);
		}

	// read the message header
	if (surRead(sock1, ptr, sizeof (SUR_NAME_LOCATE_MSG)) == -1)
		{
		sryLog("%s: read error on header-%s.\n", n.c_str(), strerror(errno));
		exit(EXIT_FAILURE);
		}

	if (atoi(ptr->hdr.token) == SUR_NAME_LOCATE)
		{
		// fork child (surrogate) to handle messaging
		pid = fork();
		if (pid < 0) // fork failure
			sryLog("%s: fork error-%s\n", n.c_str(), strerror(errno));
		else if (pid == 0) 
			surrogate_s(sock1);	// child
		}
	else
		sryLog("%s: unknown msg token.\n", n.c_str());

	// parent does not need this socket any longer
	close(sock1);
	}
}
