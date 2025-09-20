/*******************************************************************************
FILE:			RS232_surrogate.cpp

DATE:			January 26, 2025

DESCRIPTION:	This file forks into surroagte_R and surrogate_S.

AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _SURROGATE_RS232_PRIMARY
#define _ALLOC
#include "RS232_surrogate.h"
#undef _ALLOC
#undef _SURROGATE_RS232_PRIMARY

int main(int argc, char **argv)
{
pid_t childPid;
const std::string fn("RS232_surrogate");

// allow for command line overrides
initialize(argc, argv);

// fork off into surrogate receiver and sender parent processes 
childPid = fork();
if (childPid < 0) // failure
	sryLog("%s: cannot fork-%s\n", fn.c_str(), strerror(errno));
else if (childPid == 0) // child
	surrogate_R();
else // parent
	surrogate_S();

return 0;
}

/**********************************************************************
FUNCTION:	initialize(int argc, char **argv)

PURPOSE:	read command line parameters

RETURNS:	success:	0
			failure:	-1
**********************************************************************/	

int initialize(int argc, char **argv)
{
int opt, var;

// namelocTimeout is a global variable measured in seconds
// this is the maximum time allowed to negotiate a remote name locate call
NamelocTimeout = NamelocTimeoutDefault;

// kaTimeout is a global variable measured in seconds
// a value of zero turns off the keep alive functionality
KaTimeout = KaTimeoutDefault;

// process command line arguments (if any)
while ((opt = getopt(argc, argv, "k:n:")) != -1)
	{
	switch (opt)
		{
		case 'k': // keep alive time out
			if ((var = atoi(optarg)) >= 1)
				KaTimeout = var;
			break;

		case 'n': // maximum time to negotiate a remote name locate call
			if ((var = atoi(optarg)) >= 10)
				NamelocTimeout = (time_t)var;
			break;
		}
	}

return 0;
}
