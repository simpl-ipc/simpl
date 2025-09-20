/*******************************************************************************
FILE:			simSlay.cpp

DATE:			February 2, 2025

DESCRIPTION:	This utility slays a SIMPL program based on its SIMPL name.

AUTHOR:			John Collins
*******************************************************************************/

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

int main(int argc, char **argv)
{
string fifoPath("/var/tmp"), fifoNameR, fifoNameY;
pid_t pid = 0;
char *p;

// check command line
if (argc != 2)
	{
	cerr << "Incorrect command line." << endl;
	cerr << "Use: simSlay simName." << endl;
	exit(EXIT_FAILURE);
	}

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

// set the names of the fifos to be searched for (without pid extension)
fifoNameR = fifoPath + "/R_" + argv[1] + ".";
fifoNameY = fifoPath + "/Y_" + argv[1] + ".";

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
	
		// complete the fifo names for removal purposes
		fifoNameR += sub;
		fifoNameY += sub;
		break;
		}
	}

// found a sim fifo by that name? (then pid will be nonzero)
if (pid)
	{
	// terminate this process
	kill(pid, SIGTERM);
	// remove receive fifo if still present; shouldn't be after kill(SIGTERM)
	remove(fifoNameR.c_str());
	// remove reply fifo if still present; shouldn't be after kill(SIGTERM)
	remove(fifoNameY.c_str());
	}

return 0;
}
