/******************************************************************************
FILE:			nameAttach.c

DATE:			January 24, 2025

DESCRIPTION:	This file tests the sim module's name attach ability. 
				
AUTHOR:			John Collins
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
int delay = 180; // default sleep time

printf("nameAttach starting\n");

// check command line
if ((argc < 2) || (argc > 3))
	{
	printf("incorrect cmd line: nameAttach receiverName (possible timeout)\n");
	exit(EXIT_FAILURE);
	}

// in case a delay has been added
if (argc == 3)
	delay = atoi(argv[2]);
	
// initialize sry for this process with receiver name argv[1]
if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry receiver\n");
	exit(EXIT_FAILURE);
	}

// check not allowed to have more than one Rtable entry
if (openSRY(argv[1]) == -1)
	printf("unable to initialize sry receiver twice\n");

// shouldn't be allowed to register another SRY name
if (openSRY("another name") == -1)
	printf("unable to initialize sry program with another name concurrently\n");

// sleep so that dumpTables/dmesg can be called to check the receiver table
// also can check /var/tmp/sry.log for sry error messages
printf("sleeping for %d seconds\n", delay);
sleep(delay);
printf("detaching name %s\n", argv[1]);

// remove this process' sry capabilities
if (closeSRY()  == -1)
	{
	printf("unable to close sry receiver\n");
	exit(EXIT_FAILURE);
	}

printf("nameAttach ended\n");

return 0;
}
