/******************************************************************************
FILE:			fractalMsg.h

DATE:			January 23, 2025

DESCRIPTION:	This file contains definitions and the message structure sent
				between fractals.py (sender) and fractals.cpp (receiver).

AUTHOR:			John Collins
******************************************************************************/

#pragma once

// definitions
#define MAX_COLOURS		16

// sry bits and pieces
typedef enum
	{
	QUIT = 0,
	CLEAR,
	PLAY,
	SAVE,
	DRAW
	} ACTION_TYPE;
		
typedef enum
	{
	MANDELBROT = 0,
	JULIA,
	TRIG,
	LEGENDRE,
	LAGUERRE,
	DRAGON,
	TCHEBYCHEV,
	BARNSLEY,
	SIERPINSKI,
	PHOENIX
	} PRIMARY_FRACTAL_TYPE;

typedef enum
	{
	COS = 0,
	SIN,
	COSH,
	SINH,
	EXP,
	NA
	} SECONDARY_FRACTAL_TYPE;

// message structure sent from sender to receiver
typedef struct
	{
	ACTION_TYPE actionType;
	PRIMARY_FRACTAL_TYPE primaryFractalType;
	SECONDARY_FRACTAL_TYPE secondaryFractalType;
	unsigned maxColumns;
	unsigned maxRows;
	unsigned maxIters;
	double divergence;
	double XMax;
	double YMax;
	double XMin;
	double YMin;
	double P;
	double Q;
	unsigned maxColours;
	unsigned colourIndex[MAX_COLOURS];
	char name[80];
	} FRACTAL_MSG;
