/*******************************************************************************
FILE:			TCP_surrogate.cpp

DATE:			February 4, 2025

DESCRIPTION:	This is the TCP/IP surrogate source top file.

AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _SURROGATE_TCP_PRIMARY
#define _ALLOC
#include "TCP_surrogate.h"
#undef _ALLOC
#undef _SURROGATE_TCP_PRIMARY

int main(int argc, char **argv)
{
pid_t childPid;
const std::string fn("TCP_surrogate");

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
FUNCTION:	int initialize(int argc, char **argv)

PURPOSE:	Read and implement command line parameters.

RETURNS:	success:	 0
			failure:	-1
**********************************************************************/	

int initialize(int argc, char **argv)
{
int opt, var;

ChkTimeout = ChkTimeoutDefault;			// global; measured in seconds
KaFailLimit = KaFailLimitDefault;		// global; >= 1
MaxMsgMemSize = MaxMsgMemSizeDefault;	// global; message buffer size
NamelocTimeout = NamelocTimeoutDefault;	// global; nameloc timeout (s)
SurrPort = SurrPortDefault;				// global; TCP/IP port
KaFlag = true;							// global; defaulted to on
KaCtr = 0;								// global; keep alive attempts

// process command line arguments (if any)
while ((opt = getopt(argc, argv, "c:f:km:n:p:")) != -1)
	{
	switch (opt)
		{
		case 'c': // sender/receiver checking time out >= 10
			if ((var = atoi(optarg)) >= 10)
				ChkTimeout = (time_t)var;
			break;

		case 'f': // keep alive fail limit >= 1
			if ((var = atoi(optarg)) >= 1)
				KaFailLimit = (unsigned)var;
			break;

		case 'k': // keep alive flag
			KaFlag = false; // turn off, on by default
			break;

		case 'm': // maximum message memory size limit
			if ((var = atoi(optarg)) >= 1)
				MaxMsgMemSize = (unsigned)var;
			break;

		case 'n': // maximum time to negotiate a remote name locate call
			if ((var = atoi(optarg)) >= 10)
				NamelocTimeout = (time_t)var;
			break;

		case 'p': // communication port
			if ((var = atoi(optarg)) >= 1)
				SurrPort = (unsigned)var;
			break;
		}
	}

return 0;
}
