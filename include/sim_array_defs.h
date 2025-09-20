/******************************************************************************
FILE:			sim_array_defs.h

DATE:			January 23, 2025

DESCRIPTION:	This file contains definitions used by the SIMPL
				library libsim.cpp/libsim.c and various surrogates.

AUTHOR:			John Collins
******************************************************************************/

#pragma once

// various allowed name lengths
#define MAX_FIFO_PATH_LEN 			100 // bytes
#define MAX_HOST_NAME_LEN			100 // bytes
#define MAX_PROGRAM_NAME_LEN		100 // bytes
#define MAX_SIM_NAME_LEN			100 // bytes

// table sizes
#define	MAX_NUM_REMOTE_RECEIVERS	100
#define	MAX_NUM_BLOCKED_SENDERS		100

// TCP surrogate naming for the sim library (Locate) as well as the surrogates
#define TCP_Surr_R					"_TCP_surrogate_R"
#define TCP_Surr_S					"_TCP_surrogate_S"
#define TCP_Surr_r					"_TCP_surrogate_r"
#define TCP_Surr_s					"_TCP_surrogate_s"

// RS232 surrogate naming for the sim library (Locate) as well as the surrogates
#define RS232_Surr_R				"_RS232_surrogate_R"
#define RS232_Surr_S				"_RS232_surrogate_S"
#define RS232_Surr_r				"_RS232_surrogate_r"
#define RS232_Surr_s				"_RS232_surrogate_s"

#define RS232_READER 				"_rs232_reader_"
#define RS232_WRITER 				"_rs232_writer_"
