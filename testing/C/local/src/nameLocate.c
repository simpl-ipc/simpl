/******************************************************************************
FILE:			nameLocate.c

DATE:			January 24, 2025

DESCRIPTION:	This file tests sim name locating.

AUTHOR:			John Collins
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim.h>

int main(int argc, char **argv)
{
int id;

printf("nameLocate starting\n");

// check command line
if (argc != 3)
	{
	printf("incorrect cmd line: nameLocate senderName receiverName\n");
	exit(EXIT_FAILURE);
	}

// open sry for a pure sender
if (openSRY(argv[1]) == -1)
	{
	printf("unable to initialize sry sender\n");
	exit(EXIT_FAILURE);
	}

// locate the receiver
if ((id = Locate("", argv[2], 0, SIM_LOCAL)) == -1)
	{
	printf("can't name locate: %s", argv[1]);
	exit(EXIT_FAILURE);
	}

printf("located receiver id %d\n", id);

// close sry connection
if (closeSRY() == -1)
	{
	printf("can't close %s", argv[1]);
	exit(EXIT_FAILURE);
	}

printf("nameLocate ended\n");
return 0;
}
