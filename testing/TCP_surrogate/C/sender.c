/******************************************************************************

FILE:			sender.c

DATE:			June 2016

DESCRIPTION:	This program sends and gets a reply of 4 KB message (1024 ints).

AUTHOR:			John Collins

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
const int memLimit = 1024;
int receiverId, in[memLimit], out[memLimit], limit;

/*
args example: sender receiverName localhost 10
sender sending to receiver on localhost 10 times
*/
if (argc != 4)
	{
	printf("Incorect command line\n");
	printf("Example: >sender receiverName localhost 10\n");
	exit(-1);
	}

if (openSRY("Sender") == -1)
	{
	printf("Couldn't start sry sender.\n");
	exit(-1);
	}

if ((receiverId = Locate(argv[2], argv[1], sizeof out, SIM_TCP)) == -1)
	{
	printf("Can't locate receiver.\n");
	exit(-1);
	}

limit = atoi(argv[3]);

// compose the message
for (int i = 0; i < memLimit; ++i)
	out[i] = i;

for (int i = 1; i <= limit; ++i)
	{
	printf("sending %u msg\n", i);
	if (Send(receiverId, out, sizeof out, in, sizeof in) == -1)
		{
		printf("Failed send.\n");
		exit(-1);
		}
	printf("got reply %u msg\n", i);
	}
//sleep(100);

closeSRY();

return 0;
}
