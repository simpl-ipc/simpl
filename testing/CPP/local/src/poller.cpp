/*******************************************************************************
FILE:			poller.cpp

DATE:			February 3, 2025

DESCRIPTION:	This file tests the poll and receive. It runs with sender and 							trigger.

AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/poll.h>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
const int memLimit = 1024;	
int msgSize, proxy, rv, buf[memLimit];
struct pollfd pfds[2];
char ch;
void *senderId;
unsigned int cnt = 0;
string pname;

if (argc != 2)
	{
	cout << "incorrect cmd line: poller pollerName" << endl;
	exit(EXIT_FAILURE);
	}

pname = argv[1];

SRY ping(pname);

pfds[0].fd = STDERR_FILENO; // stderr
pfds[0].events = POLLIN;
pfds[1].fd = ping.rfd();
pfds[1].events = POLLIN;

while (true)
	{
	if ( poll(pfds, 2, -1) > 0 )
		{
		if (pfds[0].revents && POLLIN)
			{
			rv = read(pfds[0].fd, &ch, 1);
			if (rv < 0)
				{
				cout << "Failed read" << endl;
				exit(EXIT_FAILURE);
				}
			}
		else if (pfds[1].revents && POLLIN)
			{
			msgSize = ping.Receive(&senderId, buf, sizeof buf);
			if (msgSize == -1)
				{
				cout << "Failed receive" << endl;
				exit(EXIT_FAILURE);
				}

			if (msgSize < -1)
				{
				proxy = ping.returnProxy(msgSize);
				cout << "trigger proxy=" << proxy << endl;
				continue;
				}

			cout << "Got message " << ++cnt << endl;

			msgSize = ping.Reply(senderId, nullptr, 0);
			if (msgSize == -1)
				{
				cout << "Failed reply" << endl;
				exit(EXIT_FAILURE);	
				}
			}
		}
	}

return 0;
}
