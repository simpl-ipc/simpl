/*******************************************************************************
FILE:			trigger.cpp

DATE:			February 4, 2025

DESCRIPTION:	This file sends a proxy to receiver.c.

AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
int receiverId, limit;
string hname, rname;

if (argc != 4)
	{
	cout << "incorrect cmd line: trigger triggerName receiverName #";
	cout << "how many times?" << endl;
	exit(EXIT_FAILURE);
	}

SRY noo(argv[1]);

rname = argv[2];
limit = atoi(argv[3]);

if ((receiverId = noo.Locate(hname, rname, 0, SIM_LOCAL)) == -1)
	{
	cout << "Can't locate receiver " << rname << endl;
	exit(EXIT_FAILURE);
	}

for (int i = 1; i <= limit; ++i)
	if (noo.Trigger(receiverId, i) == -1)
		{
		cout << "Failed trigger" << endl;
		exit(EXIT_FAILURE);
		}

return 0;
}
