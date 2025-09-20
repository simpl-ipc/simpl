#*******************************************************************************
#
# FILE:		Makefile
#
# DATE:		November 2022
#
# DESCRIPTION:	This make file produces the test programs for SIM.
#
# AUTHOR:	John Collins
#
#*******************************************************************************

#====================================================================
# definitions
#====================================================================

OBJ_DIR = ./obj
LIB_DIR_S = ../../../static-lib
LIB_DIR_D = ../../../dynamic-lib
BIN_DIR = ./bin
INC_DIR = ../../../include

# default for C++ standard library version to be used if not on comannd line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall -I$(INC_DIR)
LDFLAGS = -lsimcpp

# static or dynamic library link
ifeq ($(DYNAMIC), 1)
	# dynamic
	LDFLAGS += -L$(LIB_DIR_D) -Wl,-R$(LIB_DIR_D)
else
	#static
	LDFLAGS += -L$(LIB_DIR_S)
endif

#=====================================================================
# command line hooks
#=====================================================================
all: \
	$(BIN_DIR)/receiver \
	$(BIN_DIR)/sender \
	$(BIN_DIR)/trigger
	@echo testing all

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/receiver.o: receiver.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/sender.o: sender.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/trigger.o: trigger.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/receiver: $(OBJ_DIR)/receiver.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/sender: $(OBJ_DIR)/sender.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/trigger: $(OBJ_DIR)/trigger.o
	$(CXX) -o $@ $? $(LDFLAGS)

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/*
	@echo testing clean
