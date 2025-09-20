/******************************************************************************
FILE:			trigger.c

DATE:			February 3, 2025

DESCRIPTION:	This file sends a proxy to receiver.c.

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
	printf("Incorrect command line\n");
	printf("Example: >trigger triggerName receiverName 10\n");
	exit(-1);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("Couldn't start sry trigger\n");
	exit(-1);
	}

if ((receiverId = Locate("localhost", argv[2], 0, SIM_TCP)) == -1)
	{
	printf("Can't locate receiver\n");
	exit(-1);
	}

limit = atoi(argv[3]);	
	
for (int i = 1; i <= limit; ++i)
	if (Trigger(receiverId, i) == -1)
		{
		printf("Failed trigger\n");
		exit(-1);
		}

closeSRY();

return 0;
}
