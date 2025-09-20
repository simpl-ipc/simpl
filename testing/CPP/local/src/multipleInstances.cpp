/*******************************************************************************
FILE:			multipleInstances.cpp

DATE:			February 4, 2025

DESCRIPTION:	This file tests the sry class' ability to fail on the 
				creation of multiple instances. 
				
AUTHOR:			John Collins
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <sim.h>

int main()
{
SRY nee1("simmer1");

// testing for ...
//SRY nee2("simmer1"); // second instance should fail (and does)
//SRY *nee3("simmer1") = new SRY; // won't compile
//SRY nee4("simmer2") = nee1; // fails to compile
sleep(60);

return 0;
}
