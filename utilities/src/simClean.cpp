/*******************************************************************************
FILE:			simClean.cpp

DATE:			February 11, 2025

DESCRIPTION:	This utility cleans up orphaned SIMPL fifos which can be left over
				from untrappable signals such as SIGKILL.

AUTHOR:			John Collins
*******************************************************************************/

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <filesystem>
#include <sys/resource.h>

using namespace std;
using namespace std::filesystem;

int main()
{
string fifoPath("/var/tmp"), fifoNameR, fifoNameY;
pid_t pid = 0;
char *p;

// get the fifo directory if exported variable exists
p = getenv("SIM_FIFO_PATH");
if (p != nullptr)
	fifoPath = p;

// check for access to the fifo path directory
if (exists(fifoPath) == false)
	{
	cerr << "Unable to obtain fifo path-" << strerror(errno) << endl;
	exit(EXIT_FAILURE);
	}

// set the name patterns of the fifos to be searched for
fifoNameR = fifoPath + "/R_";
fifoNameY = fifoPath + "/Y_";

// eg. looking for /var/tmp/R_noodle.12345
for (const auto& file : directory_iterator(fifoPath))
	{
	// ignore directory entry if not a fifo
	if (!is_fifo(file))
		continue;
	
	// convert iterator to a string for comparison purposes
	string s = file.path().string();

	// is the entry either the receive or the reply fifo? 
	if (!s.compare(0, fifoNameR.length(), fifoNameR, 0, fifoNameR.length()) ||
		!s.compare(0, fifoNameY.length(), fifoNameY, 0, fifoNameY.length()))
		{
		// get the pid extension from the entry
		int pos = s.find(".");
		string sub = s.substr(pos + 1);
		pid = stoi(sub);

		// check whether the process is running or not
		if (pid > 0)
			{
			// errno must be cleared prior to getpriority() call
			errno = 0;
			if ( (getpriority(PRIO_PROCESS, pid) == -1) && (errno == ESRCH) )
				// process is not running
				remove(s.c_str());
			}
		}
	}

return 0;
}
