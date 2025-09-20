/******************************************************************************
FILE:			receiver.cpp

DATE:			February 4, 2025

DESCRIPTION:	This file tests the sry C++/Python compatibility.
				
AUTHOR:			John Collins
******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <sim.h>

// set make minimize padding alignment for 32/64-bit machines
typedef struct
	{
	short s;
	bool a;
	char b;
	short c;
	int d;
	long e;
	float f;
	float g;
	double h;
	char i[32];
	bool j[8];
	short k[8];
	int l[8];
	long m[8];
	float n[8];
	double o[8];
	} TESTER;

void printit(void);
char mem[1024];

using namespace std;

int main()
{
int n, i = 0;
void *sender;

SRY nee("RECEIVER_P");

while (true)
	{
	n = nee.Receive(&sender, mem, 1024);
	if (n == -1)
		{
		cout << "receiver: Receive error" << endl;
		exit(EXIT_FAILURE);
		}

	cout << "Receiver: received message " << ++i << " size=" << n << endl;
	cout << "tester size = " <<  sizeof(TESTER) << endl;
	printit();
	cout << "********************************************************" << endl;
	cout << endl;
	
	if (nee.Reply(sender, mem, n) == -1)
		{
		cout << "receiver: Reply error" << endl;
		exit(EXIT_FAILURE);
		}
	}

return 0;
}

void printit()
{
TESTER *out = (TESTER *)mem;

cout << "s=" << out->s << endl;
cout << "a=" << out->a << endl;
cout << "b=" << out->b << endl;
cout << "c=" << out->c << endl;
cout << "d=" << out->d << endl;
cout << "e=" << out->e << endl;
cout << "f=" << out->f << endl;
cout << "g=" << out->g << endl;
cout << "h=" << out->h << endl;
cout << "i=" << out->i << endl;

cout << "j=";
for (auto z : out->j)
	cout << " " << z;
cout << endl;

cout << "k=";
for (auto z : out->k)
	cout << " " << z;
cout << endl;

cout << "l=";
for (auto z : out->l)
	cout << " " << z;
cout << endl;

cout << "m=";
for (auto z : out->m)
	cout << " " << z;
cout << endl;

cout << "n=";
for (auto z : out->n)
	cout << " " << z;
cout << endl;

cout << "o=";
for (auto z : out->o)
	cout << " " << z;
cout << endl;
}
