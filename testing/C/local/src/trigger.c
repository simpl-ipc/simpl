/******************************************************************************
FILE:			trigger.c

DATE:			January 24, 2025

DESCRIPTION:	This file sends a proxy to receiver.c.

AUTHOR:			John Collins
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
int receiverId, limit;

if (argc != 4)
	{
	printf("incorrect cmd line: trigger triggerName receiverName #");
	printf("how many times?\n");
	exit(EXIT_FAILURE);
	}

limit = atoi(argv[3]);

if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry sender\n");
	exit(EXIT_FAILURE);
	}

if ((receiverId = Locate("", argv[2], 0, SIM_LOCAL)) == -1)
	{
	printf("Can't locate receiver %s\n", argv[2]);
	exit(EXIT_FAILURE);
	}

for (int i = 1; i <= limit; ++i)
	if (Trigger(receiverId, i) == -1)
		{
		printf("Failed trigger\n");
		exit(EXIT_FAILURE);
		}

if (closeSRY() == -1)
	{
	printf("Failed to close\n");
	exit(EXIT_FAILURE);	
	}

return 0;
}
