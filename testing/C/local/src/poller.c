/******************************************************************************
FILE:			poller.c

DATE:			January 24, 2025

DESCRIPTION:	This file tests the poll and receive. It runs with sender.c.

AUTHOR:			John Collins
******************************************************************************/

#include <errno.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
const int memLimit = 1024;
int msgSize, buf[memLimit], ret;
void *senderId;
struct pollfd pfds[2];
char ch;
unsigned cnt = 0;

if (argc != 2)
	{
	printf("incorrect cmd line: poller pollerName\n");
	exit(EXIT_FAILURE);
	}

if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry poller\n");
	exit(EXIT_FAILURE);
	}

pfds[0].fd = STDERR_FILENO; // stderr
/*
Originally used stdin instead which works fine if this program is run in the foregoround.
If run in the background stdin continually reports hits of 0. Since pfds[0] is really just a placeholder for this program in order to test SIM with poll(), it doesn't matter.
*/
pfds[0].events = POLLIN;
pfds[1].fd = rfd();
pfds[1].events = POLLIN;

while (true)
	{
	ret = poll(pfds, 2, -1);

	if (ret > 0)
		{
		if (pfds[0].revents & POLLIN)
			{
			if (read(pfds[0].fd, &ch, 1) == -1)
				{
				printf("Failed read\n");
				exit(EXIT_FAILURE);
				}
			}
		else if (pfds[1].revents & POLLIN)
			{
			msgSize = Receive(&senderId, buf, sizeof buf);
			if (msgSize == -1)
				{
				printf("Failed receive\n");
				exit(EXIT_FAILURE);
				}

			if (msgSize < -1)
				{
				printf("trigger proxy=%d\n", returnProxy(msgSize));
				continue;
				}

			msgSize = Reply(senderId, NULL, 0);
			if (msgSize == -1)
				{
				printf("Failed reply\n");
				exit(EXIT_FAILURE);	
				}

			printf("Received and Replied message %u\n", ++cnt);
			}
		}
	else if (ret == -1)
		{
		printf("poll error: %s\n", strerror(errno));
		break;
		}
	}

if (closeSRY() == -1)
	{
	printf("Failed to close\n");
	exit(EXIT_FAILURE);
	}

return 0;
}
