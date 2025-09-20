/******************************************************************************
FILE:			fractals.h

DATE:			January 23, 2025

DESCRIPTION:	This file contains the Chaos class definitions and colour
				palette used by fractals.cpp.

AUTHOR:			John Collins
******************************************************************************/

#pragma once

#include <fractalMsg.h>

// function prototypes
inline double anorm(std::complex<double>);
int saveBMP(char *);

#pragma pack (1)
typedef struct
{
short bfType;
int bfSize;
short bfReserved1;
short bfReserved2;
int bfOffBits;
} BITMAPFILEHEADER;

#pragma pack (1)
typedef struct
{
int biSize;
int biWidth;
int biHeight;
short biPlanes;
short biBitCount;
int biCompression;
int biSizeImage;
int biXPelsPerMeter;
int biYPelsPerMeter;
int biClrUsed;
int biClrImportant;
} BITMAPINFOHEADER;

// global variables
Display *display = nullptr;	// pointer to X Display structure
Window window;				// newly created window
GC gc;						// graphics context
Colormap colourMap;			// the default colour map
const char *colourName[MAX_COLOURS] =
	{"white",
	"violet",
	"blue",
	"green",
	"brown",
	"yellow",
	"orange",
	"navy",
	"cyan",
	"sky blue",
	"maroon",
	"sea green",
	"grey",
	"black",
	"azure",
	"dark orange"}; // palette of 16 chosen colours

// fractal renderer base class
class Chaos
{
public:
	// incoming ipc message
	FRACTAL_MSG *m;

	// variables common to all sub-classes
	double deltaX, deltaY;

	// colour variables
	unsigned colour;
	XColor cColour[MAX_COLOURS];// closest colour - actual setting used 
	XColor eColour; 			// exact colour: dummy var setting draw colour
};

// fractal renderer classes
class Mandelbrot : Chaos
{
private:
	// variables used in calulations
	std::complex<double> deltaXZ, deltaYZ;
	std::complex<double> Z, C;
	
public:
	Mandelbrot(FRACTAL_MSG *p){m=p;}// constructor
	int drawit(void);				// fractal drawing method
};

class Julia : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, C;

public:
	Julia(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);			// fractal drawing method
};

class Trig : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, C;

public:
	Trig(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);			// fractal drawing method
};

class Legendre : Chaos
{
private:
	// variables used in calulations
	private:
	// variables used in calulations
	std::complex<double> Z, C, Zsq;
	
public:
	Legendre(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);				// fractal drawing method
};

class Laguerre : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, C, Zsq;
	
public:
	Laguerre(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);				// fractal drawing method
};

class Dragon : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, Zn, C;
	
public:
	Dragon(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);				// fractal drawing method
};

class Tchebychev : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, C, Zcu;
	
public:
	Tchebychev(FRACTAL_MSG *p){m=p;}// constructor
	int drawit(void);				// fractal drawing method
};

class Barnsley : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, C;

public:
	Barnsley(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);				// fractal drawing method
};

class Sierpinski : Chaos
{
private:
	// variables used in calulations
	std::complex<double> Z, C;

public:
	Sierpinski(FRACTAL_MSG *p){m=p;}// constructor
	int drawit(void);				// fractal drawing method
};

class Phoenix : Chaos
{
private:
	// variables used in calulations	
	std::complex<double> Z0, Z1, Z2, C;

public:
	Phoenix(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);				// fractal drawing method
};

class Play : Chaos
{
private:
	// variables used in calulations	
	std::complex<double> Z0, Z1, Z2, C;
	
public:
	Play(FRACTAL_MSG *p){m=p;}	// constructor
	int drawit(void);			// fractal drawing method
};
