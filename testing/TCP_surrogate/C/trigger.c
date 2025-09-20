/******************************************************************************

FILE:			trigger.c

DATE:			June 2016

DESCRIPTION:	This file sends a proxy to receiver.cpp.

AUTHOR:			John Collins

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sim.h>

int main(int argc, char **argv)
{
int receiverId, limit;

if (argc != 4)
	{
	printf("Incorect command line\n");
	printf("trigger receiverName hostname numberofmsgs");
	exit(-1);
	}

if (openSRY("Trigger") == -1)
	{
	printf("Couldn't start sry trigger.\n");
	exit(-1);
	}

if ((receiverId = Locate(argv[2], argv[1], 0, SIM_TCP)) == -1)
	{
	printf("Can't locate receiver.\n");
	exit(-1);
	}

limit = atoi(argv[3]);	
	
for (int i = 1; i <= limit; ++i)
	if (Trigger(receiverId, 42) == -1)
		{
		printf("Failed trigger.\n");
		exit(-1);
		}

closeSRY();

return 0;
}
