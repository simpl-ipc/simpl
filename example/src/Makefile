#*******************************************************************************
# FILE:		Makefile
#
# Date:		February 4, 2025
#
# DESCRIPTION:	This make file produces fractal programs used as an example
#		of SIMPL C++ and Python communications.
#
# AUTHOR:	John Collins
#*******************************************************************************

#====================================================================
# definitions
#====================================================================
OBJ_DIR = ./obj
BIN_DIR = ../bin
INC_DIR1 = ../include
INC_DIR2 = ../../include
LIB_DIR_D = ../../dynamic-lib
LIB_DIR_S = ../../static-lib

# default for C++ standard library version to be used if not on command line
ifndef STL
STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall -I$(INC_DIR1) -I$(INC_DIR2)
LDFLAGS = -lsimcpp -lX11 -lm

# static or dynamic library link
ifeq ($(DYNAMIC), 1)
	# dynamic
	LDFLAGS += -L$(LIB_DIR_D) -Wl,-R$(LIB_DIR_D)
else
	# static
	LDFLAGS += -L$(LIB_DIR_S)
endif

#=====================================================================
# Default target
#=====================================================================
all: \
	$(BIN_DIR)/fractals \
	$(BIN_DIR)/sender
	@-cp fractals.py $(BIN_DIR)
	@-cp sender.py $(BIN_DIR)
	@echo fractals all

title:
	@echo fractals starting
	@echo $(LDFLAGS)

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/fractals: $(OBJ_DIR)/fractals.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/sender: $(OBJ_DIR)/sender.o
	$(CXX) -o $@ $? $(LDFLAGS)

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/fractals.o: fractals.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/sender.o: sender.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/*
	@echo fractals clean
