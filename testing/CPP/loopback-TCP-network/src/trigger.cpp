/*******************************************************************************
FILE:			trigger.cpp

DATE:			February 4, 2025

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

if (argc != 4)
	{
	cout << "Incorect command line" << endl;
	exit(EXIT_FAILURE);
	}

SRY noo(argv[1]);

if ((receiverId = noo.Locate("localhost", argv[2], 0, SIM_TCP)) == -1)
	{
	cout << "Can't locate receiver" << endl;
	exit(EXIT_FAILURE);
	}

limit = atoi(argv[3]);	
	
for (int i = 1; i <= limit; ++i)
	if (noo.Trigger(receiverId, i) == -1)
		{
		cout << "Failed trigger" << endl;
		exit(EXIT_FAILURE);
		}

return 0;
}
