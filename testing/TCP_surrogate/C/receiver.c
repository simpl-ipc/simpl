/******************************************************************************

FILE:			receiver.c

DATE:			June 2016

DESCRIPTION:	This program receives a 4 KB message (1024 ints), reverses the 
				number order in the message and replies it back. It is meant to 
				work with sender.c. It also works with trigger.c in receiving a 
				proxy.

AUTHOR:			John Collins

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sim.h>

int main(int argc, char **argv)
{
const int memLimit = 1024;
int msgSize, proxy;
void *senderId;
int buf[memLimit];
unsigned long cnt = 0;

if (argc != 2)
	{
	printf("Incorect command line.\n");
	exit(EXIT_FAILURE);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("Unable to start sry receiver.\n");
	exit(EXIT_FAILURE);
	}

while (1)
	{
	msgSize = Receive(&senderId, buf, sizeof buf);
	if (msgSize == -1)
		{
		printf("Failed receive.\n");
		exit(EXIT_FAILURE);
		}

	if (msgSize < -1)
		{
		proxy = returnProxy(msgSize);
		printf("trigger proxy=%d.\n", proxy);
		continue;
		}

	printf("sender's reply buffer size=%d\n", getSenderRplySize(senderId));

	for (int i = 0; i < memLimit; ++i)
		buf[i] = memLimit - 1 - i;

	msgSize = Reply(senderId, buf, sizeof buf);
	if (msgSize == -1)
		{
		printf("Failed reply.\n");
		exit(EXIT_FAILURE);
		}

	printf("received/replied msg %lu\n", ++cnt);
	}

closeSRY();

return 0;
}
