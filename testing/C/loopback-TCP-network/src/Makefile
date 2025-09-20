#******************************************************************************
# FILE:		Makefile
#
# DATE:		February 3, 2025
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

CC = gcc
CFLAGS = -c -O3 -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -lsimc

# static or dynamic library link
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
	$(BIN_DIR)/receiver \
	$(BIN_DIR)/sender \
	$(BIN_DIR)/trigger
	@echo testing all

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/receiver.o: receiver.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/sender.o: sender.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/trigger.o: trigger.c
	$(CC) $(CFLAGS) -o $@ $<

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/receiver: $(OBJ_DIR)/receiver.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/sender: $(OBJ_DIR)/sender.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/trigger: $(OBJ_DIR)/trigger.o
	$(CC) $? $(LDFLAGS) -o $@

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -f $(BIN_DIR)/*
	@echo testing clean
