/*******************************************************************************
FILE:			sender.cpp

DATE:			February 2, 2025

DESCRIPTION:	This sender benchmarks the send-receive-reply by sending
				1 KB messages round trip numPasses of times.

AUTHOR:			John Collins
*******************************************************************************/

#include <iostream>
#include <string>
#include <sys/time.h>
#include <sim.h>

using namespace std;

int main(void)
{
string sname("SENDER"), rname("RECEIVER"), host;
//string sname("SENDER"), rname("RECEIVER"), host = nullptr;
const int numPasses = 1000000, memLimit = 1024;
char in[memLimit], out[memLimit]; // no need to set a message
int receiverId;
time_t total;
struct timeval start, stop;

SRY noo(sname);

if ((receiverId = noo.Locate(host, rname, memLimit, SIM_LOCAL)) == -1)
	{
	cout << "Can't locate receiver " << rname << endl;
	exit(EXIT_FAILURE);
	}

gettimeofday(&start, NULL); 

for (int j = 0; j < numPasses; ++j)
	if (noo.Send(receiverId, out, sizeof out, in, sizeof in) == -1)
		{
		cout << "Failed send" << endl;
		exit(EXIT_FAILURE);
		}

gettimeofday(&stop, NULL); 

total = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);
cout << "sender: time taken=" << total / numPasses;
cout << " microseconds/KB/per pass" << endl;
 
return 0;
}
