/*******************************************************************************
FILE:			RS232_surrogateUtils.cpp

DATE:			January 26, 2025

DESCRIPTION:	This file contains utility type functions.

AUTHOR:			John Collins
*******************************************************************************/

// application header
#define _ALLOC extern
#include "RS232_surrogate.h"
#undef _ALLOC

/**********************************************************************
FUNCTION:	int checkMemory(int, int)

PURPOSE:	Adjust the global dynamic memory for upward growth. 

RETURNS:	int	
**********************************************************************/	

int checkMemory(int msgType, int size)
{
// InMsgArea is global
// InMsgSize is global
// OuMsgArea is global
// OutMsgSize is global
const static char *fn = "checkMemory_rs232";

if (msgType == IN)
	{
	if (size > InMsgSize)
		{
		// realloc memory
		InMsgArea = (char *)realloc((void *)InMsgArea, size);
		if (!InMsgArea)
			{ 
			sryLog("%s: in message memory allocation error-%s.\n", 
													fn, strerror(errno));
			return -1;
			}

		// reset global memory size
		InMsgSize = size;
		}
	}
else
	{
	if (size > OutMsgSize)
		{
		// realloc memory
		OutMsgArea = (char *)realloc((void *)OutMsgArea, size);
		if (!OutMsgArea)
			{	 
			sryLog("%s: out message memory allocation error-%s\n",
				   									fn, strerror(errno));
			return -1;
			}

		// reset global memory size
		OutMsgSize = size;
		}
	}

return 0;
}

/**********************************************************************
FUNCTION:	void deadChildren(void)

PURPOSE:	This function deals with dead children; ie. RS232_surrogate_r()
			and RS232_surrogate_s().

RETURNS:	nothing
**********************************************************************/	

void deadChildren()
{
struct sigaction zom;

zom.sa_handler = SIG_IGN;
sigemptyset(&zom.sa_mask);
zom.sa_flags = SA_NOCLDWAIT;
sigaction(SIGCHLD, &zom, NULL);
}
