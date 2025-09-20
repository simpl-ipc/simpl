#******************************************************************************
# FILE:		Makefile
#
# DATE:		January 23, 2025
#
# DESCRIPTION:	Top level make for SIM/python version 3 C extended library.
#
# AUTHOR:	John Collins
#******************************************************************************

PROJECT = SIM
TASK = python

# default for C++ standard library version to be used if not on comannd line
ifndef STL
	STL = 20
endif

# dynamic or static library link
ifndef DYNAMIC
	# dynamic library link by default
	DYNAMIC = 1
	# static library link by default
	#DYNAMIC = 0
endif

SRC_DIR = ./src
MOD_DIR = ./modules
PYMOD_DIR = ../pymodules

#===================================
# entry point for python make
#===================================
all:
	@echo "$(PROJECT) $(TASK) build all starting"
	@echo `date`
	make -C $(SRC_DIR) DYNAMIC=$(DYNAMIC) all
	@cp $(MOD_DIR)/* $(PYMOD_DIR)
	@echo "$(PROJECT) $(TASK) all done"

#=================================
# to force a total rebuild
#=================================
clean:
	@echo "$(PROJECT) $(TASK) clean starting"
	@echo `date`
	make -C $(SRC_DIR) clean
	@-rm -fr $(PYMOD_DIR)/*
	@echo "$(PROJECT) $(TASK) clean done"
