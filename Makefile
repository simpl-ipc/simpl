#*******************************************************************************
# FILE:		Makefile
#
# DATE:		February 6, 2025
#
# DESCRIPTION:	Top level make file for SIMPL. This make causes 2 separate
#		SIM libraries to be made, static and dynamic. The dynamic 
#		version of the SIM library is linked by default. Setting 
#		DYNAMIC=0 on the command line sets the linking to the 
#		static library.
#
# LINK OPTIONS:	make DYNAMIC=1 all	SIM dynamic library, default
#		make DYNAMIC=0 all	SIM static library
#
# AUTHOR:	John Collins
#*******************************************************************************

# default for C++ standard library version to be used if not on command line
ifndef STL
	STL = 20
endif

# dynamic or static library link?
ifndef DYNAMIC
	# dynamic library link by default
	DYNAMIC = 1
	# static library link by default
	#DYNAMIC = 0
endif

#=============================================================================
# default: dynamically linked user context library
#=============================================================================
all:
	@echo SIM Top Level Dynamic Build all starting
	@echo `date`
	@echo STL Version is $(STL)
	@echo

	make -C ./sim-lib dynamic STL=$(STL)
	make -C ./sim-lib static STL=$(STL)
	make -C ./surrogates/TCP_surrogate/src all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./surrogates/RS232_surrogate/src all DYNAMIC=$(DYNAMIC) \
								STL=$(STL)
	make -C ./python all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./benchmarks/src all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./example/src DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/C/local/src all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/C/loopback-TCP-network/src all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/CPP/local/src all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/CPP/loopback-TCP-network/src all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/python/C++ all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/TCP_surrogate/C all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./testing/TCP_surrogate/CPP all DYNAMIC=$(DYNAMIC) STL=$(STL)
	make -C ./utilities/src all DYNAMIC=$(DYNAMIC) STL=$(STL)

	@echo
	@echo Done SIM TOP LEVEL dynamic done
	@echo `date`

#=============================================================================
# to force a total rebuild
#=============================================================================
clean:
	@echo SIM Top Level clean starting
	@-rm -f ./bin/*
	@-rm -f ./scripts/*

	make -C ./sim-lib clean
	make -C ./surrogates/TCP_surrogate/src clean
	make -C ./surrogates/RS232_surrogate/src clean
	make -C ./python clean
	make -C ./benchmarks/src clean
	make -C ./example/src clean
	make -C ./testing/C/local/src clean
	make -C ./testing/C/loopback-TCP-network/src clean
	make -C ./testing/CPP/local/src clean
	make -C ./testing/CPP/loopback-TCP-network/src clean
	make -C ./testing/python/C++ clean
	make -C ./testing/TCP_surrogate/C clean
	make -C ./testing/TCP_surrogate/CPP clean
	make -C ./utilities/src clean

	@echo SIM TOP LEVEL clean done
	@echo `date`
