/*******************************************************************************
FILE:			sender.cpp

DATE:			February 4, 2025

DESCRIPTION:	This program sends and gets a reply of 4 KB message (1024 ints).

AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
const int memLimit = 1024;
int limit, receiverId, in[memLimit], out[memLimit];

if (argc != 4)
	{
	cout << "Incorrect command line: sender senderName receiverName #" << endl;
	cout << "how many times?" << endl;
	exit(EXIT_FAILURE);
	}

SRY noo(argv[1]);

if ((receiverId = noo.Locate("localhost", argv[2], sizeof out, SIM_TCP)) == -1)
	{
	cout << "Can't locate receiver" << endl;
	exit(EXIT_FAILURE);
	}

limit = atoi(argv[3]);

// compose the message
for (int i = 0; i < memLimit; ++i)
	out[i] = i;

for (int i = 0; i < limit; ++i)
	{
	cout << "sending " << i << " msg" << endl;
	if (noo.Send(receiverId, out, sizeof out, in, sizeof in) == -1)
		{
		cout << "Failed send" << endl;
		exit(EXIT_FAILURE);
		}
	usleep(100000);
	}
//sleep(100);

return 0;
}
