#*******************************************************************************
#
# FILE:		Makefile
#
# DATE:		November 2022
#
# DESCRIPTION:	This make file produces the TCP/IP rec/sen programs.
#
# AUTHOR:	John Collins
#
#*******************************************************************************

#====================================================================
# definitions
#====================================================================
OBJ_DIR = ./obj
BIN_DIR = ./bin

REC_OBJ = $(OBJ_DIR)/recTCP.o
SEN_OBJ = $(OBJ_DIR)/senTCP.o

# default for C++ standard library version to be used if not on command line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall

#=====================================================================
# Default target
#=====================================================================
all: \
	$(BIN_DIR)/recTCP \
	$(BIN_DIR)/senTCP
	@echo socket_sim all

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/recTCP: $(REC_OBJ)
	$(CXX) -o $@ $(REC_OBJ)

$(BIN_DIR)/senTCP: $(SEN_OBJ)
	$(CXX) -o $@ $(SEN_OBJ)
	
#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/recTCP.o: recTCP.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/senTCP.o: senTCP.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/recTCP
	@-rm -f $(BIN_DIR)/senTCP	
	@echo socket-sim clean

