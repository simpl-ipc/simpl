/*******************************************************************************
FILE:			trigger.cpp

DATE:			November 2022

DESCRIPTION:	This file sends a proxy to receiver.cpp.

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
string name, &n = name, host, &h = host;
SRY noo("trigger");	

if (argc != 4)
	{
	cout << "Incorect command line." << endl;
	exit(EXIT_FAILURE);
	}

name = argv[1];	
host = argv[2];

if ((receiverId = noo.Locate(h, n, 0, SIM_TCP)) == -1)
	{
	cout << "Can't locate receiver." << endl;
	exit(EXIT_FAILURE);
	}

limit = atoi(argv[3]);	
	
for (int i = 1; i <= limit; ++i)
	if (noo.Trigger(receiverId, 42) == -1)
		{
		cout << "Failed trigger." << endl;
		exit(EXIT_FAILURE);
		}

return 0;
}
