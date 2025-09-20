#*******************************************************************************
# FILE:		Makefile
#
# DATE:		January 26, 2025
#
# DESCRIPTION:	This make file produces the RS232 surrogate programs.
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

RS232_RW_OBJ = \
	$(OBJ_DIR)/RS232_rw.o

RS232_SURROGATE_OBJ = \
	$(OBJ_DIR)/RS232_surrogate.o \
	$(OBJ_DIR)/RS232_surrogate_R.o \
	$(OBJ_DIR)/RS232_surrogate_S.o \
	$(OBJ_DIR)/RS232_surrogate_rr.o \
	$(OBJ_DIR)/RS232_surrogate_ss.o \
	$(OBJ_DIR)/RS232_surrogateUtils.o

# default for C++ standard library version to be used if not on command line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall -I$(INC_DIR1) -I$(INC_DIR2)
LDFLAGS = -lsimcpp

# static or dynamic library link
ifeq ($(DYNAMIC), 1)
	 #dynamic link
	LDFLAGS += -L$(LIB_DIR_D) -Wl,-R$(LIB_DIR_D)
else 
	# static library link
	LDFLAGS += -L$(LIB_DIR_S)
endif

#=====================================================================
# default target
#=====================================================================
all: \
	$(BIN_DIR)/RS232_rw \
	$(BIN_DIR)/RS232_surrogate
	-cp $(BIN_DIR)/RS232_rw $(SIM_BIN_DIR)/RS232_rw
	-cp $(BIN_DIR)/RS232_surrogate $(SIM_BIN_DIR)/RS232_surrogate
	-cp $(SCRIPTS_DIR)/* $(SIM_SCRIPTS_DIR)
	@echo RS232 all

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/RS232_rw: $(RS232_RW_OBJ)
	$(CXX) -o $@ $? $(LDFLAGS)	

$(BIN_DIR)/RS232_surrogate: $(RS232_SURROGATE_OBJ)
	$(CXX) -o $@ $? $(LDFLAGS)

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/RS232_rw.o: RS232_rw.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/RS232_surrogate.o: RS232_surrogate.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/RS232_surrogate_R.o: RS232_surrogate_R.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/RS232_surrogate_S.o: RS232_surrogate_S.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/RS232_surrogate_rr.o: RS232_surrogate_rr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/RS232_surrogate_ss.o: RS232_surrogate_ss.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/RS232_surrogateUtils.o: RS232_surrogateUtils.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/RS232_rw
	@-rm -f $(BIN_DIR)/RS232_surrogate
	@echo RS232 clean
