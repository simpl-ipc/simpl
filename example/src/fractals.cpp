/*******************************************************************************
FILE:		fractals.cpp

DATE:		January 23, 2025

DESCRIPTION:The purpose of this program is to receive a message and display a 
			type of fractal graphically. Regardless of the action to be carried 
			out, the message is always of the same data structure rather than a 
			separate message for each action. This program uses X-windows for 
			the graphics and SIMPL for the interprocess message passing. The actions carried out are the calculating and display of various fractals, clearing the screen and saving the screen image to a bitmap file in the /var/tmp directory.

AUTHOR:		John Collins

NOTES:		The message structure etc. are defined in fractalMsg.h.
			Global variables and class definitions are defined in fractal.h.
*******************************************************************************/

// system headers
#include <complex>
#include <iostream>
#include <cstring>
#include <X11/Xlib.h>

// sry and application headers
#include <sim.h>
#include <fractals.h>
#include <fractalMsg.h>

int main()
{
// display is global
// window is global
// gc is global
// colourMap is global
// colourName[] is global
	
// X windows and local variables to main()
int screen;					// number of screen to place the window on
int depth;					// number of planes
unsigned maxDisplayWidth;	// width of the X display
unsigned maxDisplayHeight;	//height of the X display
unsigned winWidth;			// width for the new window
unsigned winHeight;			// height for the new window
unsigned winX;				// location of the window's top-left corner x-coord
unsigned winY;				// location of the window's top-left corner y-coord
unsigned winBorderWidth;	// width of window's border
Visual *visual;				// colour info
XSetWindowAttributes attr;	// for changing window attributes

// ipc messaging variables
void *senderId = nullptr;	// sender's sry id
int msgSize = -1;			// size of received message
char msgBuffer[1024];		// message buffer > actual message size
FRACTAL_MSG *msg = nullptr;	// message template size < message buffer size
const std::string me = "FractalReceiver";// sry receiver name

// instantiate a SRY object - name attach etc.
SRY comm(me);

// open the connection to the Xserver
display = XOpenDisplay(NULL);
if (display == NULL)
	{
    std::cout << "Cannot connect to X server" << std::endl;
    exit(EXIT_FAILURE);
	}

// get the number of the default screen for our X server
screen = XDefaultScreen(display);

// get the plane depth (24)
depth = XDefaultDepth(display, screen);

// get default colour map
colourMap = XDefaultColormap(display, screen);

// get the default graphics context
gc = XDefaultGC(display, screen);

// get the maximal width of the default screen of our X server in pixels
maxDisplayWidth = XDisplayWidth(display, screen);

// get the maximal height of the default screen of our X server in pixels
maxDisplayHeight = XDisplayHeight(display, screen);

// get the default visual type
visual = XDefaultVisual(display, screen);

// set a black background for the window
attr.background_pixel = XBlackPixel(display, screen);

// find the ID of the root window of the screen
window = XRootWindow(display, screen);

// set the window's width and height
winWidth = maxDisplayWidth;
winHeight = maxDisplayHeight;

// position of the window is top-left corner - (0,0)
winX = 0;
winY = 0;

// the window's border
winBorderWidth = 1;

// create the window as specified by prior calls visual etc.
window = XCreateWindow(
	display,
	window,
	winX,
	winY,
	winWidth,
	winHeight,
	winBorderWidth,
	depth,
	InputOutput,
	visual,
	CWBackPixel,
	&attr);

// realize window
XMapWindow(display, window);

// force and wait for Xwindows server to perform  
XSync(display, False);

// receive a message with instructions on which fractal to draw
while (true)
	{
	// line up message template on the message buffer
	msg = (FRACTAL_MSG *)msgBuffer;
	// initialize amount of buffer needed for message
	memset(msg, '\0', sizeof(FRACTAL_MSG));

	// get a message
	msgSize = comm.Receive(&senderId, msgBuffer, sizeof msgBuffer);
	if (msgSize == -1)
		{
		std::cout << "Failed receive" << std::endl;
		exit(EXIT_FAILURE);
		}

	// null reply - unblock Send()
	msgSize = comm.Reply(senderId, nullptr, 0);
	if (msgSize == -1)
		{
		std::cout << "Failed reply" << std::endl;
		exit(EXIT_FAILURE);	
		}

	// what to do next?
	if (msg->actionType == QUIT)
		{
		XDestroyWindow(display, window);// close the window
		XCloseDisplay(display);			// close connection to X server
		exit(EXIT_SUCCESS);				// end
		}
	else if (msg->actionType == CLEAR)
		{
		XClearWindow(display, window);	// clear window
		XFlush(display);
		}
	else if (msg->actionType == SAVE)
		{
		saveBMP(msg->name); // save the current display to a bitmap
		}
	else if (msg->actionType == PLAY)
		{
		XClearWindow(display, window);	// clear window
		XFlush(display);
		// this is where we stick in whatever we want to experiment with
		// create a Play object within the scope {}
		Play *frac = new Play(msg);
		frac->drawit();
		delete frac;
		}
	else if (msg->actionType == DRAW)
		{
		XClearWindow(display, window);	// clear window
		XFlush(display);
		
		// which fractal to generate?
		switch (msg->primaryFractalType)
			{
			case MANDELBROT:
				{
				// create a Mandelbrot object within the scope {}
				Mandelbrot *frac = new Mandelbrot(msg);
				frac->drawit();
				delete frac;
				break;
				}		

			case JULIA:
				{
				// create a Julia object within the scope {}
				Julia *frac = new Julia(msg);
				frac->drawit();
				delete frac;
				break;
				}

			case TRIG:
				{
				// create a Trig (cos/sin/cosh/sinh/exp) object within scope {}
				Trig *frac = new Trig(msg);
				frac->drawit();
				delete frac;
				break;		
				}

			case LEGENDRE:
				{
				// create a Legendre object within the scope {}
				Legendre *frac = new Legendre(msg);
				frac->drawit();
				delete frac;
				break;
				}

			case LAGUERRE:
				{
				// create a Laguerre object within the scope {}
				Laguerre *frac = new Laguerre(msg);
				frac->drawit();
				delete frac;
				break;
				}

			case DRAGON:
				{
				// create a Dragon object within the scope {}
				Dragon *frac = new Dragon(msg);
				frac->drawit();
				delete frac;
				break;
				}

			case TCHEBYCHEV:
				{
				// create a Tchebychev object within the scope {}
				Tchebychev *frac = new Tchebychev(msg);
				frac->drawit();
				delete frac;
				break;
				}

			case BARNSLEY:
				{
				// create a Barnsley object within the scope {}
				Barnsley *frac = new Barnsley(msg);
				frac->drawit();
				delete frac;
				break;
				}
				
			case SIERPINSKI:
				{
				// create a Sierpinski object within the scope {}
				Sierpinski *frac = new Sierpinski(msg);
				frac->drawit();
				delete frac;
				break;
				}

			case PHOENIX:
				{
				// create a Phoenix object within the scope {}
				Phoenix *frac = new Phoenix(msg);
				frac->drawit();
				delete frac;
				break;
				}

			default:
				std::cout << "unknown primary fractal type" << std::endl;	
				exit(EXIT_FAILURE);	
			}
		}	
	}

// close the window
XDestroyWindow(display, window);

// close connection to X server
XCloseDisplay(display);

return 0;
}

/*******************************************************************************
FUNCTION:	int Mandelbrot::drawit(void)

PURPOSE:	This method calculates and draws a Mandelbrot fractal based on the 
			parameters of the incoming message. The math is done using complex 		
			numbers since this is what is generally used in the math literature 	
			as well as simplifying the computer algorithm by reducing the 		
			number of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Mandelbrot :: drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// initialize needed math variables
deltaXZ.real(deltaX);
deltaXZ.imag(0.0);
deltaYZ.real(0.0);
deltaYZ.imag(deltaY);
C.real(m->P);
C.imag(m->Q);

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	{
	// set the colours based on the names
	if (!XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]))
		{
		std::cout<<"Failed to allocate colour: "<<m->colourIndex[i]<<std::endl;
		exit(EXIT_FAILURE);
		}
	}
	
// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values for each row
		Z.real(0.0);
		Z.imag(0.0);
		colour = 0;

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Z = Z * Z + C;
			colour++;
			}

		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour % m->maxColours].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		// next Q for each row in the column
		C -= deltaYZ;
		}

	// next P per column
	C += deltaXZ;
	// first Q in the column
	C.imag(m->YMax);
	}
	
return 0;
}	

/*******************************************************************************
FUNCTION:	int Julia::drawit(void)

PURPOSE:	This method calculates and draws a Julia fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the number 
			of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Julia::drawit(void)
{
// display, window, gc, colourMap, colourName are global
	
// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// initialize needed math variables
C.real(m->P);
C.imag(m->Q);

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		Z.real(m->XMin + col * deltaX);
		Z.imag(m->YMax - row * deltaY);
		colour = 0;

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Z = Z * Z + C;
			colour++;
			}

		// colour for next row value
		if (colour >= m->maxIters)
			colour = (unsigned)(anorm(Z) * 6.0) % m->maxColours + 1;
		else
			colour = 0;

		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Trig::drawit(void)

PURPOSE:	This method calculates and draws a Tig fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the required variables.
			
			The possible trig fractals are sin, cos, sinh, cosh, exp.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
			Z = tan(Z) + C, Z = tanh(Z) + C, Z = exp(Z) + C have been tried but 
			are uninteresting from a graphical picture point of view.
*******************************************************************************/

int Trig::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		colour = 0;
		Z.real(0.0);
		Z.imag(0.0);
		C.real(m->XMin + col * deltaX);
		C.imag(m->YMax - row * deltaY);

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			switch (m->secondaryFractalType)
				{
				case COS:
					Z = cos(Z) + C;
					break;
				case SIN:
					Z = sin(Z) + C;
					break;
				case COSH:
					Z = cosh(Z) + C;
					break;
				case SINH:
					Z = sinh(Z) + C;
					break;
				case EXP:
					Z = C * exp(Z);
					break;
				default:
					std::cout << "unknown secondary fractal type" << std::endl;	
					exit(EXIT_FAILURE);	
				}

			colour++;
			}
		
		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : colour % (m->maxColours - 1) + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Legendre::drawit(void)

PURPOSE:	This method calculates and draws a Legendre fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Legendre::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		colour = 0;
		Z.real(0.0);
		Z.imag(0.0);
		C.real(m->XMin + col * deltaX);
		C.imag(m->YMax - row * deltaY);

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Zsq = Z * Z;
			Z = 0.125 * (35.0 * Zsq * Zsq - 30.0 * Zsq + 3.0) + C;
			colour++;
			}
		
		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : colour % (m->maxColours - 1) + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Laguerre::drawit(void)

PURPOSE:	This method calculates and draws a Laguerre fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
number 
			of lines of code and the needed variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Laguerre::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		colour = 0;
		Z.real(0.0);
		Z.imag(0.0);
		C.real(m->XMin + col * deltaX);
		C.imag(m->YMax - row * deltaY);

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Zsq = Z * Z;
			Z = (Zsq*Zsq - 18.0*Zsq*Z + 72.0*Zsq - 96.0*Z + 24.0)/24.0 + C;
			colour++;
			}
		
		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : colour % (m->maxColours - 1) + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Dragon::drawit(void)

PURPOSE:	This method calculates and draws a Dragon fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Dragon::drawit(void)
{
// display, window, gc, colourMap, colourName are global
	
// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		Zn.real(m->XMin + col * deltaX);
		Zn.imag(m->YMax - row * deltaY);
		Z.real(0.0);
		Z.imag(0.0);
		C.real(m->P);
		C.imag(m->Q);
		colour = 0;
	
		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Z = C * Zn * (1.0 - Zn);
			Zn = Z;
			colour++;
			}
		
		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 :(int)(norm(Z) * 6.0) % 6 + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Tchebychev::drawit(void)

PURPOSE:	This method calculates and draws a Tchebychev fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Tchebychev::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		colour = 0;
		Z.real(0.654);
		Z.imag(0.0);
		C.real(m->XMin + col * deltaX);
		C.imag(m->YMax - row * deltaY);

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Zcu = Z * Z * Z;
			Z = C * (16.0 * Z * Z * Zcu - 20.0 * Zcu + 5.0 * Z);
			colour++;
			}
		
		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : colour % (m->maxColours - 1) + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Barnsley::drawit(void)

PURPOSE:	This method calculates and draws a Barnsley fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Barnsley::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;
C.real(m->P);

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		Z.real(m->XMin + col * deltaX);
		Z.imag(m->YMax - row * deltaY);
		colour = 0;

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			if (Z.real() >= 0)
				Z = Z * Z - 1.0;
			else
				Z = Z * Z - 1.0 + C.real() * Z.real();
			colour++;
			}

		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : colour % (m->maxColours - 1) + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Sierpinski::drawit(void)

PURPOSE:	This method calculates and draws a Sierpinski fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the reqired variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Sierpinski::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	C.real(m->XMin + col * deltaX);

	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		C.imag(m->YMax - row * deltaY);
		Z.real(C.real());
		Z.imag(C.imag());
		colour = 0;

		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z) < m->divergence))
			{
			Z *= 2;
			if (Z.imag() > 1.0)
				 Z.imag(Z.imag() - 1);
			else if (Z.real() > 1.0)
				Z.real(Z.real() -1);
			colour++;
			}
		
		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : colour % (m->maxColours - 1) + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Phoenix::drawit(void)

PURPOSE:	This method calculates and draws a Phoenix fractal based on the 
			parameters of the incoming message. The math is done using complex
			numbers since this is what is generally used in the math literature 
			as well as simplifying the computer algorithm by reducing the 
			number of lines of code and the required variables.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Phoenix::drawit(void)
{
// display, window, gc, colourMap, colourName are global

// deltas are the scaling of so many pixels per X and Y
deltaX = (m->XMax - m->XMin) / m->maxColumns;
deltaY = (m->YMax - m->YMin) / m->maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < m->maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[m->colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < m->maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < m->maxRows; row++)
		{
		// initialize starting values
		Z2.real(m->XMin + col * deltaX);
		Z2.imag(m->YMax - row * deltaY);
		Z1.real(Z2.imag());
		Z1.imag(Z2.real());
		Z0.real(0.0);
		Z0.imag(0.0);
		C.real(m->P);
		C.imag(m->Q);
		colour = 0;
	
		// do the math for pixel point (col, row)
		while ((colour < m->maxIters) && (norm(Z2) < m->divergence))
			{
			Z2 = Z1 * Z1 + C.real() + C.imag() * Z0;
			Z0 = Z1;
			Z1 = Z2;
			colour++;
			}

		// reset the colour?
		colour = (colour >= m->maxIters) ? 0 : ((int)(norm(Z2)*6.0))%6 + 1;
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	int Play::drawit(void)

PURPOSE:	This method is designed for in-house messing around and 
			experimenting. The only purpose of the gui program in this case is 
			to send a PLAY action to this program to activate this method. 
			Other than the leading token of the message, no other message 
			parameters are used. All necessary variables are defined locally 
			within this	method so that it centralizes the work and minimizes 
			the effect on the already functioning program elements.

RETURNS:	0

NOTES:		This class inherits public variables from a base class called Chaos.
*******************************************************************************/

int Play::drawit(void) // phoenix curve
{
// display, window, gc, colourMap, colourName are global

// set variables usually derived from the incoming message
double XMax=1.5, YMax=1.2, XMin=-1.5, YMin=-0.7;
unsigned maxColumns=1000, maxRows=650, maxColours=16, divergence=4, 
maxIters=64;
double P=0.56667, Q=-0.5;
// colours used in draw	
unsigned colourIndex[16] = {13,1,2,3,4,5,6,7,8,9,10,11,12,0,14,15};

// deltas are the scaling of so many pixels per X and Y
deltaX = (XMax - XMin) / maxColumns;
deltaY = (YMax - YMin) / maxRows;

// set the rgb's for the chosen colour scheme in colourName 
for (unsigned i = 0; i < maxColours; i++)
	// set the colours based on the names
	XAllocNamedColor(display, colourMap, colourName[colourIndex[i]],
				&eColour, &cColour[i]);

// loop through all columns
for (unsigned col = 0; col < maxColumns; col++)
	{
	// loop through all rows per column
	for (unsigned row = 0; row < maxRows; row++)
		{
		// initialize starting values
		Z2.real(XMin + col * deltaX);
		Z2.imag(YMax - row * deltaY);
		Z1.real(Z2.imag());
		Z1.imag(Z2.real());
		Z0.real(0.0);
		Z0.imag(0.0);
		C.real(P);
		C.imag(Q);
		colour = 0;
	
		// do the math for pixel point (col, row)
		while ((colour < maxIters) && (norm(Z2) < divergence))
			{
			Z2 = Z1 * Z1 + C.real() + C.imag() * Z0;
			Z0 = Z1;
			Z1 = Z2;
			colour++;
			}

		// reset the colour?
		colour = (colour >= maxIters) ? 0 : ((int)(norm(Z2)*6.0))%6 + 1;
		
		// set a drawing colour for this particular pixel
		XSetForeground(display, gc, cColour[colour].pixel);
		// draw this pixel at (col, row)
		XDrawPoint(display, window, gc, col, row);
		}
	}

return 0;
}

/*******************************************************************************
FUNCTION:	double anorm(std::complex<double> z)

PURPOSE:	calculates the anti-norm of a complex number

RETURNS:	double
*******************************************************************************/

inline double anorm(std::complex<double> z)
{
return z.real() * z.real() - z.imag() * z.imag();
}

/*******************************************************************************
FUNCTION:	int saveBMP(char *name)

PURPOSE:	Saves a fractal screen image to a bitmap file as 'name'.bmp in 
			/var/tmp.

RETURNS:	success: 0
			failure: -1
*******************************************************************************/

int saveBMP(char *name)
{
// display, window are global
XWindowAttributes gwa;
char filename[80];
FILE *fp;

// set file name for saved screen image
sprintf(filename, "/var/tmp/%s.bmp", name);

// fetch the attributes of the current display
XGetWindowAttributes(display, window, &gwa);

// set a pointer to the displayed image
XImage *Xi = XGetImage(
					display,
					window,
					0,
					0,
					gwa.width,
					gwa.height,
					AllPlanes, 
					ZPixmap);
if (!Xi)	
	{
	std::cout << "Failed on XGetImage" << std::endl;
	return -1;	
	}

// build required bitmap file header
BITMAPFILEHEADER bmpFileHeader;
memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
bmpFileHeader.bfType = 0x4D42;
bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
bmpFileHeader.bfReserved1 = 0;
bmpFileHeader.bfReserved2 = 0;
int biBitCount = 32;
int dwBmpSize = ((Xi->width * biBitCount + 31) / 32) * 4 * Xi->height;
bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ dwBmpSize;

// build required bitmap info header
BITMAPINFOHEADER bmpInfoHeader;
memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
bmpInfoHeader.biWidth = Xi->width;
bmpInfoHeader.biHeight = Xi->height;
bmpInfoHeader.biPlanes = 1;
bmpInfoHeader.biBitCount = biBitCount;
bmpInfoHeader.biSizeImage = 0;
bmpInfoHeader.biCompression = 0;
bmpInfoHeader.biXPelsPerMeter = 0;
bmpInfoHeader.biYPelsPerMeter = 0;
bmpInfoHeader.biClrUsed = 0;
bmpInfoHeader.biClrImportant = 0;

// open binary file for writing
fp = std::fopen(filename, "wb");
if (fp == NULL)
	{
	std::cout << "Could not open file for writing" << std::endl;
	return -1;
	}

std::fwrite(&bmpFileHeader, sizeof(bmpFileHeader), 1, fp);
std::fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fp);
std::fwrite(Xi->data, dwBmpSize, 1, fp);
std::fclose(fp);

return 0;
}
