#******************************************************************************
# FILE:		Makefile
#
# DATE:		January 23, 2025
#
# DESCRIPTION:	Make file for SIM Python version 3 C extended library.
#
# AUTHOR:	John Collins
#******************************************************************************

# default command line defs
ifndef PYTHONVER
	PYTHONVER = 3.13
endif

MOD_DIR = ../modules
PYMOD_DIR = ../../pymodules
OBJ_DIR = ./obj
INC_DIR1 = /usr/include/python$(PYTHONVER) # python stuff
INC_DIR2 = ../../include # sim stuff

MY_LIBRARY = simc.so
MY_OBJECT = simc.o
MY_SOURCE = simcmodule.c

ifeq ($(DYNAMIC), 1)
	# dynamic library
	LIB_DIR = ../../sim-lib/dynamic_obj
	SIM_OBJ = $(LIB_DIR)/libsimc.so
else
	# static library
	LIB_DIR = ../../sim-lib/static_obj
	SIM_OBJ = $(LIB_DIR)/libsimc.o
endif		

CC = gcc
CFLAGS = -c -fPIC -O3 -I$(INC_DIR1) -I$(INC_DIR2)
LIBFLAGS = -shared

description:
	@(\
	echo;\
	echo Python SIM build starting; \
	echo;\
	echo "** Note that this library works only for Python 3.* **";\
	echo;\
	echo "To change Python Version (for example):";\
	echo "make all PYTHONVER=$(PYTHONVER)";\
	echo;\
	echo "To find the Python versions available on your computer type:";\
	echo "sudo find / -name Python.h";\
	echo "and examine the path results of the search.";\
	echo)

#========================
# Default targets
#========================
all:\
	description \
	$(MOD_DIR)/$(MY_LIBRARY)
	@echo python sim all
	@echo `date`

#===================
# Library 
#===================
$(MOD_DIR)/$(MY_LIBRARY):  $(OBJ_DIR)/$(MY_OBJECT)
	$(CC) $(SIM_OBJ) $< $(LIBFLAGS) -o $@ 

#===================
# Compiles
#===================
$(OBJ_DIR)/$(MY_OBJECT): $(MY_SOURCE)
	$(CC) $(CFLAGS) -o $@ $<

#===================
#  Clean
#===================
clean:
	@-rm -f $(OBJ_DIR)/*.o
	@-rm -fr $(MOD_DIR)/$(MY_LIBRARY)
	@echo python sim clean 
	@echo `date`
