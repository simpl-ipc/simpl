/*******************************************************************************
FILE:			senrec.cpp

DATE:			February 4, 2025

DESCRIPTION:	This program receives a 4 KB message (1024 ints), sends it 
				on to receiver.c which reverses the number order in the 
				message and replies it back. It then replies this reply back 
				to the original sender, sender.c. So, it sits in between
				sender.c and receiver.c.
				
				sender <-------> senrec <------> receiver

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
int receiverId, msgSize, in[memLimit], out[memLimit];
void *senderId;
string myname, rname, hname;
unsigned long cnt = 0;

if (argc != 3)
	{
	cout << "incorrect cmd line: senrec myName receiverName" << endl;
	exit(EXIT_FAILURE);
	}
	
myname = argv[1];
rname = argv[2];

SRY nee(myname);

if ((receiverId = nee.Locate(hname, rname, sizeof out, SIM_LOCAL)) == -1)
	{
	cout << "Can't locate receiver " << rname << endl;
	exit(EXIT_FAILURE);
	}

while (true)
	{
	msgSize = nee.Receive(&senderId, in, sizeof in);
	if (msgSize == -1)
		{
		cout << "Failed receive" << endl;
		exit(EXIT_FAILURE);
		}

	if (nee.Send(receiverId, in, sizeof in, out, sizeof out) == -1)
		{
		cout << "Failed send" << endl;
		exit(EXIT_FAILURE);
		}

	msgSize = nee.Reply(senderId, out, sizeof out);
	if (msgSize == -1)
		{
		cout << "Failed reply" << endl;
		exit(EXIT_FAILURE);
		}

	cout << "received/sent/replied msg " << ++cnt << endl;
	}

return 0;
}
