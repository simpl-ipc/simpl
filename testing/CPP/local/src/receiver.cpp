/*******************************************************************************
FILE:			receiver.cpp

DATE:			February 3, 2025

DESCRIPTION:	This program receives a 4 KB message (1024 ints), reverses the 
				number order in the message and replies it back. It is meant to 
				work with sender and trigger.

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
void *senderId;
unsigned int cnt = 0;
string pname;

if (argc != 2)
	{
	cout << "incorrect cmd line: receiver receiverName" << endl;
	exit(EXIT_FAILURE);
	}

pname = argv[1];

SRY nee(pname);

while (true)
	{
	msgSize = nee.Receive(&senderId, buf, sizeof buf);
	if (msgSize == -1)
		{
		cout << "Failed receive" << endl;
		exit(EXIT_FAILURE);
		}

	if (msgSize < -1)
		{
		proxy = nee.returnProxy(msgSize);
		cout << "trigger proxy=" << proxy << endl;
		continue;
		}

	for (auto i : buf)
		i = memLimit - 1 - i;

	msgSize = nee.Reply(senderId, buf, sizeof buf);
	if (msgSize == -1)
		{
		cout << "Failed reply" << endl;
		exit(EXIT_FAILURE);	
		}

	cout << "received/replied msg " << ++cnt << endl;
	}

return 0;
}
