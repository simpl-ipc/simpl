/***********************************************************************
FILE:			sender.cpp

DATE:			January 23, 2025

DESCRIPTION:	This file sends test messages to fractal.cpp

AUTHOR:			John Collins
***********************************************************************/

#include <cstring>
#include <iostream>

#include <sim.h>
#include <fractalMsg.h>

int main()
{
int receiverId;
const std::string renderer = "FractalReceiver", &receiver = renderer;
const std::string distant = "", &host = distant;
FRACTAL_MSG msg;
unsigned colourIndex1[] = {13,1,2,3,4,5,6,7,8,9,10,11,12,0,14,15};
//unsigned colourIndex2[] = {13,8,2,3,4,5,6,0,0,0,0,0,0,0,0,0};

/*
only currently operational message
*/
// mandelbrot
unsigned maxColours = 16;
msg.actionType = DRAW;
msg.primaryFractalType = MANDELBROT;
msg.secondaryFractalType = NA;
msg.maxColumns = 1000;
msg.maxRows = 650;
msg.maxIters = 512;
msg.divergence = 4;
msg.XMax = 1.2;
msg.YMax = 1.2;
msg.XMin = -2.0;
msg.YMin = -1.2;
msg.P = msg.XMin;
msg.Q = msg.YMax;
msg.maxColours = maxColours;
for (unsigned i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex1[i];
memset(msg.name, 0, 80);

/* julia
unsigned maxColours = 7;
msg.actionType = DRAW;
msg.primaryFractalType = ;
msg.secondaryFractalType = ;
msg.fractalType = JULIA;
msg.maxColumns = 1600;
msg.maxRows = 1000;
msg.maxIters = 96;
msg.divergence = 4;
msg.XMax = 1.8;
msg.YMax = 1.2;
msg.XMin = -1.8;
msg.YMin = -1.2;
msg.P = -0.743036;
msg.Q = 0.113467;
msg.maxColours = maxColours;
for (i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex2[i];
*/

/* cosine
unsigned maxColours = 16;
msg.actionType = DRAW;
msg.fractalType = COSINE;
msg.maxColumns = 1900;
msg.maxRows = 1000;
msg.maxIters = 64;
msg.divergence = 1000;
msg.XMax = 5.0;
msg.YMax = 8.0;
msg.XMin = -7.0;
msg.YMin = -8.0;
msg.P = 0;
msg.Q = 0;
msg.maxColours = maxColours;
for (i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex1[i];
*/

/* exponential
unsigned maxColours = 16;
msg.actionType = DRAW;
msg.fractalType = EXPONENTIAL;
msg.maxColumns = 1900;
msg.maxRows = 1000;
msg.maxIters = 64;
msg.divergence = 1000;
msg.XMax = 2.0;
msg.YMax = 1.0;
msg.XMin = 0.5;
msg.YMin = -1.0;
msg.P = 0;
msg.Q = 0;
msg.maxColours = maxColours;
for (i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex1[i];
*/

/* legendre
unsigned maxColours = 16;
msg.actionType = DRAW;
msg.fractalType = LEGENDRE;
msg.maxColumns = 1900;
msg.maxRows = 1000;
msg.maxIters = 64;
msg.divergence = 1000;
msg.XMax = 0.8;
msg.YMax = 0.4;
msg.XMin = -0.8;
msg.YMin = -0.4;
msg.P = 0;
msg.Q = 0;
msg.maxColours = maxColours;
for (i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex1[i];
*/

/* barnsley
unsigned maxColours = 16;
msg.actionType = DRAW;
msg.fractalType = BARNSLEY;
msg.maxColumns = 1900;
msg.maxRows = 1000;
msg.maxIters = 64;
msg.divergence = 4;
msg.XMax = 1.7;
msg.YMax = 1.03;
msg.XMin = -2.1;
msg.YMin = -1.03;
msg.P = 1;
msg.Q = 0;
msg.maxColours = maxColours;
for (i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex1[i];
*/

/* sierpinski
unsigned maxColours = 16;
msg.actionType = DRAW;
msg.fractalType = SIERPINSKI;
msg.maxColumns = 1300;
msg.maxRows = 1000;
msg.maxIters = 64;
msg.divergence = 64; // changes here cause great differences
msg.XMax = 1.3;
msg.YMax = 1.22;
msg.XMin = -0.5;
msg.YMin = -0.22;
msg.P = 0;
msg.Q = 0;
msg.maxColours = maxColours;
for (unsigned i = 0; i < maxColours; i++)
	msg.colourIndex[i] = colourIndex1[i];
*/

//clear window
//msg.actionType = CLEAR;

// receiver exit
//msg.actionType = QUIT;

SRY *sen = new SRY("exampleSender");

if ((receiverId = sen->Locate(host, receiver, sizeof msg, SIM_LOCAL)) == -1)
	{
	std::cout << "Can't locate receiver: " << receiver <<	std::endl;
	exit(EXIT_FAILURE);
	}

if (sen->Send(receiverId, &msg, sizeof msg, nullptr, 0) == -1)
	{
	std::cout << "Failed send" << std::endl;
	exit(EXIT_FAILURE);
	}

delete sen;

return 0;
}
