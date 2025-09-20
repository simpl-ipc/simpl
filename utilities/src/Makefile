#*******************************************************************************
# FILE:			Makefile
#
# DATE:			February 6, 2025
#
# DESCRIPTION:		This make file produces the SIM utility programs.
#
# AUTHOR:		John Collins
#*******************************************************************************

#====================================================================
# definitions
#====================================================================

OBJ_DIR = ./obj
BIN_DIR = ../bin
LIB_DIR_S = ../../static-lib
LIB_DIR_D = ../../dynamic-lib
SIM_BIN_DIR = ../../bin

#default for C++ standard library version to be used if not on command line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall
LDFLAGS = -L$(LIB_DIR) -lsimcpp

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
	$(BIN_DIR)/simShow \
	$(BIN_DIR)/simSlay \
	$(BIN_DIR)/simClean
	-cp $(BIN_DIR)/simShow $(SIM_BIN_DIR)/simShow
	-cp $(BIN_DIR)/simSlay $(SIM_BIN_DIR)/simSlay
	-cp $(BIN_DIR)/simClean $(SIM_BIN_DIR)/simClean
	@echo utilities all

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/simShow: $(OBJ_DIR)/simShow.o
	$(CXX) -o $@ $? $(LDFLAGS)	

$(BIN_DIR)/simSlay: $(OBJ_DIR)/simSlay.o
	$(CXX) -o $@ $? $(LDFLAGS)	

$(BIN_DIR)/simClean: $(OBJ_DIR)/simClean.o
	$(CXX) -o $@ $? $(LDFLAGS)

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/simShow.o: simShow.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/simSlay.o: simSlay.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/simClean.o: simClean.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/sim*
	@echo utilities clean
