/*******************************************************************************
FILE:			RS232.h

DATE:			January 26, 2025

DESCRIPTION:	This file contains necessary headers, definitions
				and global variables for the rs232_rw.c etc. files.

AUTHOR:			John Collins

NOTES:			Make certain to chmod a+rw /dev/ttyS* !!!
*******************************************************************************/

#pragma once

// standard headers
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>

/*
note the values used in /usr/include/bits/termios.h
which is called by /usr/include/termios.h
*/

// sim headers
#include <sim.h>
#include <surrMsgs.h>

// definitions
#define OPEN_FLAGS			O_RDWR | O_NOCTTY
#define BAUD_RATE			B9600
#define CONTROL_MODE_FLAGS	BAUD_RATE | CRTSCTS | CS8 | CLOCAL | CREAD
#define INPUT_MODE_FLAGS	IGNPAR
#define OUTPUT_MODE_FLAGS	0
#define LOCAL_MODE_FLAGS	0

//#define RS232_READER 		"_rs232_reader_"
//#define RS232_WRITER 		"_rs232_writer_"

// function prototypes
int initialize(int, char **);
void rs232_r(void);
void rs232_w(void);
int readSerial(int, int, char *);
int upMemory(int);

// global constants
const char *SerialDeviceDefault = "/dev/ttyS0";
const int IntWidthDefault = 8; // could be 2/4/8 for 16/32/64 bit archs.
const bool VerboseDefault = false;

// global variables; set in initialize()
char SerialDevice[50];
int IntWidth;
bool Verbose;
char *MemArea = nullptr;
int MemSize = 0;
