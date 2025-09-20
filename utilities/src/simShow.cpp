/*******************************************************************************
FILE:			simShow.cpp

DATE:			February 2, 2025

DESCRIPTION:	This utility displays a list of running SIMPL programs.

AUTHOR:			John Collins
*******************************************************************************/

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

int main()
{
string fifoPath("/var/tmp"), fifoNameR;
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

// use the names of the receive fifos (without pid extension)
fifoNameR = fifoPath + "/R_";

// print a header for the list of running sim processes
cout << "SIM Name:Pid" << endl;
cout << "============" << endl;

// eg. looking for /var/tmp/R_noodle.12345
for (const auto& file : directory_iterator(fifoPath))
	{
	// ignore directory entry if not a fifo
	if (!is_fifo(file))
		continue;
	
	// convert iterator to a string for comparison purposes
	string s = file.path().string();

	// is the entry a receive fifo? 
	if (!s.compare(0, fifoNameR.length(), fifoNameR, 0, fifoNameR.length()))
		{
		// sub1 of the form nee.12345
		string sub1 = s.substr(fifoNameR.length());
		// chop the .12345 to get the sim name for display purposes
		int pos = sub1.find(".");
		string sub2 = sub1.substr(0, pos);
		// get the pid extension from the entry
		string sub3 = sub1.substr(pos + 1);
		cout << sub2 << ":" << sub3 << endl;
		}
	}

return 0;
}
