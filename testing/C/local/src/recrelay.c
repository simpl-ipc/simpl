/******************************************************************************
FILE:			recrelay.c

DATE:			January 24, 2025

DESCRIPTION:	This program receives a 4 KB message (1024 ints), sends it 
				on to receiver.c which reverses the number order in the 
				message and replies it back. It then replies this reply back 
				to the original sender, sender.c. So, it sits in between
				sender.c and receiver.c.
				
				sender -------> recrelay ------> receiver
                  ^				                    |
				  |                                 |
                  -----------------------------------
				  
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
int receiverId;
void *senderId;
int msgSize, in[memLimit], out[memLimit];
unsigned long cnt = 0;

if (argc != 3)
	{
	printf("incorrect cmd line: recrelay myName receiverName\n");
	exit(EXIT_FAILURE);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry receiver\n");
	exit(EXIT_FAILURE);
	}

if ((receiverId = Locate("", argv[2], sizeof out, SIM_LOCAL)) == -1)
	{
	printf("Can't locate receiver %s\n", argv[2]);
	exit(EXIT_FAILURE);
	}

while (true)
	{
	msgSize = Receive(&senderId, in, sizeof in);
	if (msgSize == -1)
		{
		printf("Failed receive\n");
		exit(EXIT_FAILURE);
		}

	if (Relay(senderId, receiverId) == -1)
		{
		printf("Failed relay\n");
		exit(EXIT_FAILURE);
		}

	printf("received/relayed msg %lu\n", ++cnt);
	}

if (closeSRY() == -1)
	{
	printf("Failed to close\n");
	exit(EXIT_FAILURE);	
	}

return 0;
}
