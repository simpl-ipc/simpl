#******************************************************************************
# FILE:		Makefile
#
# DATE:		February 2, 2025
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
# default target
#=====================================================================
all:\
	$(BIN_DIR)/srylog \
	$(BIN_DIR)/nameAttach \
	$(BIN_DIR)/nameLocate \
	$(BIN_DIR)/poller \
	$(BIN_DIR)/receiver \
	$(BIN_DIR)/selector \
	$(BIN_DIR)/sender \
	$(BIN_DIR)/senrec \
	$(BIN_DIR)/recrelay \
	$(BIN_DIR)/trigger
	@echo testing all

#=====================================================================
# compiling
#=====================================================================
$(OBJ_DIR)/srylog.o: srylog.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/nameAttach.o: nameAttach.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/nameLocate.o: nameLocate.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/poller.o: poller.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/receiver.o: receiver.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/selector.o: selector.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/sender.o: sender.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/senrec.o: senrec.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/recrelay.o: recrelay.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/trigger.o: trigger.c
	$(CC) $(CFLAGS) -o $@ $<

#=====================================================================
# linking
#=====================================================================
$(BIN_DIR)/srylog: $(OBJ_DIR)/srylog.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/nameAttach: $(OBJ_DIR)/nameAttach.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/nameLocate: $(OBJ_DIR)/nameLocate.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/poller: $(OBJ_DIR)/poller.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/receiver: $(OBJ_DIR)/receiver.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/selector: $(OBJ_DIR)/selector.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/sender: $(OBJ_DIR)/sender.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/senrec: $(OBJ_DIR)/senrec.o
	$(CC) $? $(LDFLAGS) -o $@

$(BIN_DIR)/recrelay: $(OBJ_DIR)/recrelay.o
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
