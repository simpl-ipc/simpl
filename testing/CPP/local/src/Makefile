#******************************************************************************
# FILE:		Makefile
#
# DATE:		February 4, 2025
#
# DESCRIPTION:	This make file produces the test programs for SIM.
#
# AUTHOR:	John Collins
#******************************************************************************

#====================================================================
# definitions
#====================================================================

OBJ_DIR = ../obj
LIB_DIR_S = ../../../../static-lib
LIB_DIR_D = ../../../../dynamic-lib
BIN_DIR = ../bin
INC_DIR = ../../../../include

# default for C++ standard library version to be used if not on comannd line
ifndef STL
	STL = 20
endif

CXX = g++
CXXFLAGS = -c -std=gnu++$(STL) -O3 -Wall -I$(INC_DIR)
LDFLAGS = -lsimcpp

# static or dynamic link
ifeq ($(DYNAMIC), 1)
	# dynamic
	LDFLAGS += -L$(LIB_DIR_D) -Wl,-R$(LIB_DIR_D)
else
	# static
	LDFLAGS += -L$(LIB_DIR_S)
endif

#=====================================================================
# command line hooks
#=====================================================================
all: \
	$(BIN_DIR)/multipleInstances \
	$(BIN_DIR)/srylog \
	$(BIN_DIR)/nameAttach \
	$(BIN_DIR)/nameLocate \
	$(BIN_DIR)/poller \
	$(BIN_DIR)/receiver \
	$(BIN_DIR)/recrelay \
	$(BIN_DIR)/selector \
	$(BIN_DIR)/sender \
	$(BIN_DIR)/bigSender \
	$(BIN_DIR)/senrec \
	$(BIN_DIR)/trigger
	@echo testing all

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/multipleInstances.o: multipleInstances.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/srylog.o: srylog.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/nameAttach.o: nameAttach.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/nameLocate.o: nameLocate.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/poller.o: poller.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/receiver.o: receiver.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/recrelay.o: recrelay.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/selector.o: selector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/sender.o: sender.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/bigSender.o: bigSender.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/senrec.o: senrec.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/trigger.o: trigger.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/multipleInstances: $(OBJ_DIR)/multipleInstances.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/srylog: $(OBJ_DIR)/srylog.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/nameAttach: $(OBJ_DIR)/nameAttach.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/nameLocate: $(OBJ_DIR)/nameLocate.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/poller: $(OBJ_DIR)/poller.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/receiver: $(OBJ_DIR)/receiver.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/recrelay: $(OBJ_DIR)/recrelay.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/selector: $(OBJ_DIR)/selector.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/sender: $(OBJ_DIR)/sender.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/bigSender: $(OBJ_DIR)/bigSender.o
	$(CXX) -o $@ $? $(LDFLAGS)

$(BIN_DIR)/senrec: $(OBJ_DIR)/senrec.o
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
