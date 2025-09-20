#******************************************************************************
# FILE:		Makefile
#
# DATE:		February 6, 2025
#
# DESCRIPTION:	This make file produces the SIMPL user space static and
#		dynamic libraries in C and C++ used for SIMPL applications.
#
# AUTHOR:	John Collins
#******************************************************************************

#====================================================================
# definitions
#====================================================================

# @ command prefix suppresses echo
# - command prefix means ignore exit status

# default for C++ standard library version to be used if not on command line
ifndef STL
STL = 20
endif

# GNU compilers
CC = gcc
CPP = g++

# target directories
DYNAMIC_OBJ_DIR = ./dynamic_obj
STATIC_OBJ_DIR = ./static_obj
DYNAMIC_LIB_DIR = ../dynamic-lib
STATIC_LIB_DIR = ../static-lib
INC_DIR1 = ./include
INC_DIR2 = ../include

# library objects
DYNAMIC_LIB_C_OBJ = $(DYNAMIC_OBJ_DIR)/libsimc.so
STATIC_LIB_C_OBJ = $(STATIC_OBJ_DIR)/libsimc.o
DYNAMIC_LIB_CPP_OBJ = $(DYNAMIC_OBJ_DIR)/libsimcpp.so
STATIC_LIB_CPP_OBJ = $(STATIC_OBJ_DIR)/libsimcpp.o

# dynamic naming
SONAME_C = libsimc.so.1
SONAME_CPP = libsimcpp.so.1
VERSION = 1.0.0
LIBRARY_C = libsimc.so.$(VERSION)
LIBRARY_CPP = libsimcpp.so.$(VERSION)

# release flags
COMFLAGS = -Ofast
# debug flags
#COMFLAGS = -g
# profile flags
#COMFLAGS = -g -a -pg

# compiler flags
CFLAGS = $(COMFLAGS) -fPIC -c -Wall -Wextra -I$(INC_DIR1) -I$(INC_DIR2)
#CFLAGS = $(COMFLAGS) -fPIC -c -Wall -Wextra -std=c2x -pedantic -I$(INC_DIR) \
								-I$(INC_DIR2)
CPPFLAGS = $(COMFLAGS) -std=gnu++$(STL) -fPIC -c -Wall -Wextra -I$(INC_DIR1) \
								-I$(INC_DIR2)

# library flags
CLIBFLAGS = -shared -Wl,-soname,$(SONAME_C) -D_REENTRANT
CPPLIBFLAGS = -shared -Wl,-soname,$(SONAME_CPP) -D_REENTRANT

#=====================================================================
# Default target: dynamic libraries
#=====================================================================
dynamic: \
	title \
	$(DYNAMIC_OBJ_DIR) \
	$(DYNAMIC_LIB_DIR) \
	$(DYNAMIC_LIB_C_OBJ) \
	$(DYNAMIC_LIB_CPP_OBJ) \
	$(DYNAMIC_LIB_DIR)/$(LIBRARY_C) \
	$(DYNAMIC_LIB_DIR)/$(LIBRARY_CPP)
	@echo simc/simc++ library all dynamic

title:
	@echo simc/simc++ library starting

$(DYNAMIC_OBJ_DIR):
	mkdir ./dynamic_obj
	chmod a+w ./dynamic_obj

$(STATIC_OBJ_DIR):
	mkdir ./static_obj
	chmod a+w ./static_obj

#=====================================================================
# dynamic libraries
#=====================================================================
$(DYNAMIC_LIB_DIR)/$(LIBRARY_CPP): $(DYNAMIC_LIB_CPP_OBJ)
	$(CPP) $(CPPLIBFLAGS) -o $@ $< -lc
	ln -sf $@ $(DYNAMIC_LIB_DIR)/$(SONAME_CPP)
	ln -sf $(DYNAMIC_LIB_DIR)/$(SONAME_CPP) $(DYNAMIC_LIB_DIR)/libsimcpp.so

$(DYNAMIC_LIB_DIR)/$(LIBRARY_C): $(DYNAMIC_LIB_C_OBJ)
	$(CC) $(CLIBFLAGS) -o $@ $< -lc
	ln -sf $@ $(DYNAMIC_LIB_DIR)/$(SONAME_C)
	ln -sf $(DYNAMIC_LIB_DIR)/$(SONAME_C) $(DYNAMIC_LIB_DIR)/libsimc.so

#=====================================================================
# static libraries
#=====================================================================
$(STATIC_LIB_DIR)/libsimcpp.a: $(STATIC_LIB_CPP_OBJ)
	ar vrs $@ $<

$(STATIC_LIB_DIR)/libsimc.a: $(STATIC_LIB_C_OBJ)
	ar vrs $@ $<

#=====================================================================
# compiling
#=====================================================================
$(DYNAMIC_LIB_CPP_OBJ): libsim.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<

$(STATIC_LIB_CPP_OBJ): libsim.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<

$(DYNAMIC_LIB_C_OBJ): libsim.c
	$(CC) $(CFLAGS) -o $@ $<

$(STATIC_LIB_C_OBJ): libsim.c
	$(CC) $(CFLAGS) -o $@ $<

#=====================================================================
# static
#=====================================================================
static:  \
	title \
	$(STATIC_OBJ_DIR) \
	$(STATIC_LIB_DIR) \
	$(STATIC_LIB_C_OBJ) \
	$(STATIC_LIB_CPP_OBJ) \
	$(STATIC_LIB_DIR)/libsimc.a \
	$(STATIC_LIB_DIR)/libsimcpp.a
	@echo simc/simc++ library all static

#=====================================================================
#  cleanup
#=====================================================================
clean:
	@-rm -f $(DYNAMIC_OBJ_DIR)/*.so
	@-rm -f $(STATIC_OBJ_DIR)/*.o
	@-rm -f $(DYNAMIC_LIB_DIR)/*
	@-rm -f $(STATIC_LIB_DIR)/*
	@echo simc/simc++ library clean
