/******************************************************************************
FILE:			selector.c

DATE:			January 24, 2025

DESCRIPTION:	This file tests the select and receive. It runs with sender.c.

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
int msgSize, proxy, fd, buf[memLimit];
void *senderId;
unsigned cnt = 0;

fd_set watchset, inset; 

if (argc != 2)
	{
	printf("incorrect cmd line: selector receiverName\n");
	exit(EXIT_FAILURE);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry receiver\n");
	exit(EXIT_FAILURE);
	}

fd = rfd();
FD_ZERO(&watchset);
FD_SET(fd, &watchset);

while (true)
	{
	inset = watchset;
	if ( select(fd + 1, &inset, NULL, NULL, NULL) > 0 )
		{
		if (FD_ISSET(fd, &inset))
			{
			msgSize = Receive(&senderId, buf, sizeof buf);
			if (msgSize == -1)
				{
				printf("Failed receive\n");
				exit(EXIT_FAILURE);
				}

			if (msgSize < -1)
				{
				proxy = returnProxy(msgSize);
				printf("trigger proxy=%d\n", proxy);
				continue;
				}

			printf("Got message %u\n", ++cnt);

			msgSize = Reply(senderId, NULL, 0);
			if (msgSize == -1)
				{
				printf("Failed reply\n");
				exit(EXIT_FAILURE);	
				}
			}
		}
	}

if (closeSRY() == -1)
	{
	printf("Failed to close\n");
	exit(EXIT_FAILURE);	
	}

return 0;
}
