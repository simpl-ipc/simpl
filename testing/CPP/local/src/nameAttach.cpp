/*******************************************************************************
FILE:			nameAttach.cpp

DATE:			February 3, 2025

DESCRIPTION:	This file tests the sry module's name attach ability. 
				
AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
const int delay = 180;
string name;

if (argc != 2)
	{
	cout << "incorrect cmd line: nameAttach receiverName" << endl;
	exit(EXIT_FAILURE);
	}

name = argv[1];

SRY nee(name);

// sleep so that /var/log can be inspected
cout << "sleeping for " << delay << " seconds" << endl;
sleep(delay);
cout << "detaching name " << argv[1] << endl;
// check /var/log for sim fifo's
// also, you can kill this process while sleeping and again check for removal
// SIM for this process becomes disabled when process ends

return 0;
}
