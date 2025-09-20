/*******************************************************************************
FILE:			sender.cpp

DATE:			November 2022

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
string name, &n = name, host, &h = host;
SRY noo("sender");

if (argc != 4)
	{
	cout << "Incorect command line" << endl;
	exit(EXIT_FAILURE);
	}
	
name = argv[1];
host = argv[2];
	
if ((receiverId = noo.Locate(h, n, sizeof out, SIM_TCP)) == -1)
	{
	cout << "Can't locate receiver." << endl;
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
		cout << "Failed send." << endl;
		exit(EXIT_FAILURE);
		}
	//sleep(1);
	}
//sleep(100);

return 0;
}
