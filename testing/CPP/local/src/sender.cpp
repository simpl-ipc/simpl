/*******************************************************************************
FILE:			sender.cpp

DATE:			February 4, 2025

DESCRIPTION:	This file sends messages to receiver.c, poller.c, selector.c,
				and senrec.c.

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
int receiverId, limit, in[memLimit], out[memLimit];
string hname, rname;

if (argc != 4)
	{
	cout << "incorrect cmd line: sender senderName receiverName #";
	cout << "how many times?" << endl;
	exit(EXIT_FAILURE);
	}

SRY nee(argv[1]);

rname = argv[2];

if ((receiverId = nee.Locate(hname, rname, sizeof out, SIM_LOCAL)) == -1)
	{
	cout << "Can't locate receiver " << rname << endl;
	exit(EXIT_FAILURE);
	}

limit = atoi(argv[3]);

// compose the message
for (auto i : out)
	i = i;

for (int i= 0; i < limit; ++i)
	{
	if (nee.Send(receiverId, out, sizeof out, in, sizeof in) == -1)
		{
		cout << "Failed send" << endl;
		exit(EXIT_FAILURE);
		}
	usleep(10);
	}

return 0;
}
