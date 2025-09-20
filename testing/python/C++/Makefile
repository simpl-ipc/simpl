#******************************************************************************
# FILE:		Makefile
#
# DATE:		February 4, 2025
#
# DESCRIPTION:	Make file for SIM Python C++ testing.
#
# AUTHOR:	John Collins
#******************************************************************************

#====================================================================
# Definitions
#====================================================================
OBJ_DIR = ./obj
LIB_DIR_S = ../../../static-lib
LIB_DIR_D = ../../../dynamic-lib
INC_DIR = ../../../include
BIN_DIR = ../bin
PY_DIR = ../Py
PYMOD_DIR = ../../../pymodules

SENDER_OBJ = \
	$(OBJ_DIR)/sender.o

RECEIVER_OBJ = \
	$(OBJ_DIR)/receiver.o

# default for C++ standard library version to be used if not on command line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -lsimcpp

# static or dynamic library link
ifeq ($(DYNAMIC), 1)
	# dynamic
	LDFLAGS += -L$(LIB_DIR_D) -Wl,-R$(LIB_DIR_D)
else
	# static
	LDFLAGS += -L$(LIB_DIR_S)
endif

#=====================================================================
# Default targets
#=====================================================================
all: \
	title \
	$(BIN_DIR)/sender \
	$(BIN_DIR)/receiver
	@echo python test all
	@echo `date`

title:
	@echo python python/test build starting

#========================
# Linking
#========================
$(BIN_DIR)/sender: $(SENDER_OBJ)
	$(CXX) $? $(LDFLAGS) -o $@

$(BIN_DIR)/receiver: $(RECEIVER_OBJ)
	$(CXX) $? $(LDFLAGS) -o $@

#========================
# Compiling
#========================
$(OBJ_DIR)/receiver.o: receiver.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/sender.o: sender.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#===================
#  Clean up
#===================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/sender
	@-rm -f $(BIN_DIR)/receiver
	@echo python test clean
	@echo `date`
