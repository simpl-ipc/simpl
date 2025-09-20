/******************************************************************************
FILE:			recrelay.cpp

DATE:			January 24, 2025

DESCRIPTION:	This program receives a 4 KB message (1024 ints), sends it 
				on to receiver.cpp which reverses the number order in the
				message and replies it back. It then replies this reply back 
				to the original sender, sender.c. So, it sits in between
				sender.cpp and receiver.cpp.
				
				sender -------> recrelay ------> receiver
                  ^				                    |
				  |                                 |
                  -----------------------------------
				  
AUTHOR:			John Collins
******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
const int memLimit = 1024;	
void *senderId;
int receiverId, msgSize, in[memLimit], out[memLimit];
unsigned long cnt = 0;
string pname;

if (argc != 3)
	{
	printf("incorrect cmd line: recrelay myName receiverName\n");
	exit(EXIT_FAILURE);
	}

pname = argv[1];

SRY noo(pname);

if ((receiverId = noo.Locate("", argv[2], sizeof out, SIM_LOCAL)) == -1)
	{
	printf("Can't locate receiver %s\n", argv[2]);
	exit(EXIT_FAILURE);
	}

while (true)
	{
	msgSize = noo.Receive(&senderId, in, sizeof in);
	if (msgSize == -1)
		{
		printf("Failed receive\n");
		exit(EXIT_FAILURE);
		}

	if (noo.Relay(senderId, receiverId) == -1)
		{
		printf("Failed relay\n");
		exit(EXIT_FAILURE);
		}

	printf("received/relayed msg %lu\n", ++cnt);
	}

return 0;
}
