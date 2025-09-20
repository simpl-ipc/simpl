#*******************************************************************************
# FILE:		Makefile
#
# DATE:		January 23, 2025
#
# DESCRIPTION:	This make file produces the TCP/IP surrogate program.
#
# AUTHOR:	John Collins
#*******************************************************************************

#====================================================================
# definitions
#====================================================================
OBJ_DIR = ./obj
BIN_DIR = ../bin
LIB_DIR_S = ../../../static-lib
LIB_DIR_D = ../../../dynamic-lib
INC_DIR1 = ../include
INC_DIR2 = ../../../include
SIM_BIN_DIR = ../../../bin
SCRIPTS_DIR = ../scripts
SIM_SCRIPTS_DIR = ../../../scripts

SURROGATE_OBJ =	\
	$(OBJ_DIR)/TCP_surrogate.o \
	$(OBJ_DIR)/TCP_surrogate_R.o \
	$(OBJ_DIR)/TCP_surrogate_S.o \
	$(OBJ_DIR)/TCP_surrogate_rr.o \
	$(OBJ_DIR)/TCP_surrogate_ss.o \
	$(OBJ_DIR)/TCP_surrogateUtils.o

# default for C++ standard library version to be used if not on command line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall -I$(INC_DIR1) -I$(INC_DIR2)
LDFLAGS = -lsimcpp 

# static or dynamic library link
ifeq ($(DYNAMIC), 1)
	 # dynamic library link
	LDFLAGS += -L$(LIB_DIR_D) -Wl,-R$(LIB_DIR_D)
else 
	# static library link
	LDFLAGS += -L$(LIB_DIR_S)
endif

#=====================================================================
# default target
#=====================================================================
all: \
	$(BIN_DIR)/TCP_surrogate
	-cp $(BIN_DIR)/TCP_surrogate $(SIM_BIN_DIR)/TCP_surrogate
	-cp $(SCRIPTS_DIR)/* $(SIM_SCRIPTS_DIR)
	@echo TCP_Surrogate all

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/TCP_surrogate: $(SURROGATE_OBJ)
	$(CXX) -o $@ $? $(LDFLAGS)
	
#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/TCP_surrogate.o: TCP_surrogate.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/TCP_surrogate_R.o: TCP_surrogate_R.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/TCP_surrogate_rr.o: TCP_surrogate_rr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/TCP_surrogate_S.o: TCP_surrogate_S.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/TCP_surrogate_ss.o: TCP_surrogate_ss.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/TCP_surrogateUtils.o: TCP_surrogateUtils.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/TCP_surrogate
	@echo TCP_Surrogate clean
