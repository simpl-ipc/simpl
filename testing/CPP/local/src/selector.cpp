/*******************************************************************************
FILE:			selector.cpp

DATE:			February 4, 2025

DESCRIPTION:	This file tests the select and receive. It runs with sender.c.

AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
const int memLimit = 1024;	
int msgSize, proxy, buf[memLimit];
unsigned int cnt = 0;
void *senderId;
string pname;
int fd;
fd_set watchset, inset; 

if (argc != 2)
	{
	cout << "incorrect cmd line: selector selectorName" << endl;
	exit(EXIT_FAILURE);
	}

pname = argv[1];

SRY neewum(pname);

fd = neewum.rfd();
FD_ZERO(&watchset);
FD_SET(fd, &watchset);

while (true)
	{
	inset = watchset;
	if ( select(fd + 1, &inset, NULL, NULL, NULL) > 0 )
		{
		if (FD_ISSET(fd, &inset))
			{
			msgSize = neewum.Receive(&senderId, buf, sizeof buf);
			if (msgSize == -1)
				{
				cout << "Failed receive" << endl;
				exit(EXIT_FAILURE);
				}

			if (msgSize < -1)
				{
				proxy = neewum.returnProxy(msgSize);
				cout << "trigger proxy=" << proxy << endl;
				continue;
				}

			cout << "Got message " << ++cnt << endl;

			msgSize = neewum.Reply(senderId, nullptr, 0);
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
