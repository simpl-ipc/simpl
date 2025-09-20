/******************************************************************************
FILE:			sryLog.c

DATE:			January 24, 2025

DESCRIPTION:	This program tests the sryLog() function.

AUTHOR:			John Collins
******************************************************************************/

#include <sim.h>

int main()
{
int a = 1, b = 2, c = 3;

sryLog("this a test of the emergency broadcast system: %d %d %d", a, b, c);
	
return 0;
}
