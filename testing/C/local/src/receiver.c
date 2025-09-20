/******************************************************************************
FILE:			receiver.c

DATE:			January 24, 2025

DESCRIPTION:	This program receives a 4 KB message (1024 ints), reverses the
				number order in the message and replies it back. It is meant to
				work with sender.c. It also works with trigger.c in receiving a
				proxy.

AUTHOR:			John Collins
******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
const int memLimit = 1024;
int msgSize, proxy, in[memLimit], out[memLimit];
void *senderId;
unsigned cnt = 0;

if (argc != 2)
	{
	printf("incorrect cmd line: receiver receiverName\n");
	exit(EXIT_FAILURE);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry receiver\n");
	exit(EXIT_FAILURE);
	}

// create reply msg
for (int i = 0; i < memLimit; ++i)
	out[i] = memLimit - 1 - i;

while (true)
	{
	msgSize = Receive(&senderId, in, sizeof in);
	if (msgSize == -1)
		{
		printf("Failed receive\n");
		exit(EXIT_FAILURE);
		}

	// is it a proxy?
	if (msgSize < -1)
		{
		proxy = returnProxy(msgSize);
		printf("trigger proxy=%d\n", proxy);
		continue;
		}
/*
// print message contents for checking
for (int i = 0; i < memLimit; ++i)
		printf("in[%d]=%d\n", i, in[i]);
*/		
//	printf("sender's sry id=%ld\n", senderId);
//	printf("sender's reply buffer size=%d\n", maxRplySize(senderId));

/*
	// say we've been away doing other things, best to check
	sleep(60);
	if (chkSender(senderId) == -1)
		{
		printf("no more sender ... sender gone\n");
		continue;
		}
*/

/*		
	if (maxRplySize(senderId) < (signed)(memLimit * sizeof(int)))
		{
		printf("Sender's memory buffer too small for reply\n");
		ReplyError(senderId);
		continue;
		}
*/		
	msgSize = Reply(senderId, out, sizeof out);
	//msgSize = Reply(senderId, NULL, 0);
	if (msgSize == -1)
		{
		printf("Failed reply\n");
		exit(EXIT_FAILURE);
		}

/*	for (int i = 0; i < memLimit; ++i)
		printf("in[%d]=%d     out[%d]=%d\n", i, in[i], i, out[i]);*/
	printf("received/replied msg %u\n", ++cnt);
	}

if (closeSRY() == -1)
	{
	printf("Failed to close\n");
	exit(EXIT_FAILURE);
	}

return 0;
}
