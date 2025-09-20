/******************************************************************************
FILE:			sender.cpp

DATE:			February 4, 2025

DESCRIPTION:	This file tests the sry C++/Python compatibility.
				
AUTHOR:			John Collins
******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <string.h>
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

char mem1[1024];
char mem2[1024];

void buildMsg(void);
void printit(bool);

using namespace std;

int main()
{
int receiver, ret;
SRY noo("sender");

receiver = noo.Locate("", "RECEIVER_P", sizeof(TESTER), SIM_LOCAL);
if (receiver == -1)
	{
	cout << "cannot locate receiver" << endl;
	exit(EXIT_FAILURE);
	}

buildMsg();

cout << "C Sender: sent message  ********************************" << endl;
printit(true);
cout << "********************************************************" << endl;
cout << endl;

ret = noo.Send(receiver, mem1, sizeof(TESTER), mem2, 1024);
if (ret == -1)
	{
	cout << "Send error" << endl;
	exit(EXIT_FAILURE);
	}

cout << "C Sender: reply message ********************************" << endl;
printit(false);
cout << "********************************************************" << endl;
cout << endl;

return 0;
}

void buildMsg()
{
TESTER *ptr = (TESTER *)mem1;

ptr->s = 1;
ptr->a = true;
ptr->b = 'A';
ptr->c = 2;
ptr->d = 37;
ptr->e = 42424242;
ptr->f = 2.1718;
ptr->g = 3.1415;
ptr->h = 1.6023e-19;

memset(&(ptr->i), 0, 32);
memcpy(ptr->i, "We are the knights who say nee!!", 32);

ptr->j[0] = true;
ptr->j[1] = false;
ptr->j[2] = true;
ptr->j[3] = false;
ptr->j[4] = true;
ptr->j[5] = false;
ptr->j[6] = true;
ptr->j[7] = false;

ptr->k[0] = 0;
ptr->k[1] = 1;
ptr->k[2] = 2;
ptr->k[3] = 3;
ptr->k[4] = 4;
ptr->k[5] = 5;
ptr->k[6] = 6;
ptr->k[7] = 7;

ptr->l[0] = 0;
ptr->l[1] = 1;
ptr->l[2] = 2;
ptr->l[3] = 3;
ptr->l[4] = 4;
ptr->l[5] = 5;
ptr->l[6] = 6;
ptr->l[7] = 7;

ptr->m[0] = 0;
ptr->m[1] = 1;
ptr->m[2] = 2;
ptr->m[3] = 3;
ptr->m[4] = 4;
ptr->m[5] = 5;
ptr->m[6] = 6;
ptr->m[7] = 7;

ptr->n[0] = 1.1;
ptr->n[1] = 2.2;
ptr->n[2] = 3.3;
ptr->n[3] = 4.4;
ptr->n[4] = 5.5;
ptr->n[5] = 6.6;
ptr->n[6] = 7.7;
ptr->n[7] = 8.8;

ptr->o[0] = 1.1;
ptr->o[1] = 2.2;
ptr->o[2] = 3.3;
ptr->o[3] = 4.4;
ptr->o[4] = 5.5;
ptr->o[5] = 6.6;
ptr->o[6] = 7.7;
ptr->o[7] = 8.8;
}

void printit(bool direction)
{
TESTER *out = (direction) ? (TESTER *)mem1 : (TESTER *)mem2;

cout << "s=" << out->s << endl;
cout << "a=" << out->a << endl;
cout << "b=" << out->b << endl;
cout << "c=" << out->c << endl;
cout << "d=" << out->d << endl;
cout << "e=" << out->e << endl;
cout << "f=" << out->f << endl;
cout << "g=" << out->g << endl;
cout << "h=" << out->h << endl;
cout << "h=" << out->i << endl;

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
