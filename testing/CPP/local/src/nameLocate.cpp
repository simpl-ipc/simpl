/*******************************************************************************
FILE:			nameLocate.cpp

DATE:			February 3, 2025

DESCRIPTION:	This file tests sry name locating.
				Works with the nameAttach program in same file.
				ie. >nameAttach neee &
					>nameLocate neee

AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <sim.h>

using namespace std;

int main(int argc, char **argv)
{
int id;
string hname, pname;

if (argc != 3)
	{
	cout << "incorrect cmd line: nameLocate myName receiverName" << endl;
	exit(EXIT_FAILURE);
	}

SRY noo(argv[1]);

pname = argv[2];
	
if ((id = noo.Locate(hname, pname, 0, SIM_LOCAL)) == -1)
	{
	cout << "can't name locate: " << pname << endl;
	exit(EXIT_FAILURE);
	}

cout << "located receiver id " << id << endl;

return 0;
}
