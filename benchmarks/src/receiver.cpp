/*******************************************************************************
FILE:			receiver.cpp

DATE:			February 2, 2025

DESCRIPTION:	This is the complementary benchmarking receiver to the
				benchmarking sender for SIMPL.

AUTHOR:			John Collins
*******************************************************************************/

#include <iostream>
#include <string>
#include <sim.h>

using namespace std;

int main(void)
{
string name("RECEIVER");
void *senderId;
const int memLimit = 1024;
char buf[memLimit];

// SRY instantiation
SRY nee(name);

while (true)
	{
	// receive test message
	if (nee.Receive(&senderId, buf, sizeof buf) == -1)
		{
		cout << "Failed receive" << endl;
		exit(EXIT_FAILURE);
		}

	// reply test message
	if (nee.Reply(senderId, buf, sizeof buf) == -1)
		{
		cout << "Failed reply" << endl;
		exit(EXIT_FAILURE);	
        }
	}

return 0;
}
