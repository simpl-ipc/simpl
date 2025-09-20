/******************************************************************************
FILE:			sender.c

DATE:			January 24, 2025

DESCRIPTION:	This program sends messages to receiver.c, poller.c, selector.c,
				and senrec.c.

AUTHOR:			John Collins
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
const int memLimit = 1024;
int receiverId;
int in[memLimit],out[memLimit],limit;

if (argc != 4)
	{
	printf("incorrect cmd line: sender senderName receiverName #\n");
	printf("how many times?\n");
	exit(EXIT_FAILURE);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry sender\n");
	exit(EXIT_FAILURE);
	}

receiverId = Locate("", argv[2], sizeof out, SIM_LOCAL);
if (receiverId == -1)
	{
	printf("Can't locate receiver %s\n", argv[2]);
	exit(EXIT_FAILURE);
	}

//printf("receiver's sry id=%ld\n", receiverId);
//printf("receiver's buffer size=%d\n", maxRecvSize(receiverId));

limit = atoi(argv[3]);

// compose the message: numbers 1-1024
for (int i = 0; i < memLimit; ++i)
	out[i] = i;

// say we've been away doing other things, best to check
//sleep(60);	
if (chkReceiver(argv[2], 0) == false)
	{
	printf("no more receiver ... receiver gone\n");
	exit(EXIT_FAILURE);
	}

for (int i= 0; i < limit; ++i)
	{
	if (Send(receiverId, &out, sizeof out, &in, sizeof in) == -1)
		{

		printf("Failed send\n");
		exit(EXIT_FAILURE);
		}

	// do something with reply
	/*
	for (int j = 0; j < memLimit; ++j)
		printf("in[%d]=%d\n", j, in[j]);
	*/
	usleep(10);
	}

if (closeSRY() == -1)
	{
	printf("Failed to close\n");
	exit(EXIT_FAILURE);	
	}

return 0;
}
