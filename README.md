# SIMPL (Synchronous Interprocess Messaging Project for Linux)

simpl-4.0.0 is a major rewrite of simpl-3.3.8. It is different from 3.3.8 on many levels.

# Overview

- SIMPL is a software design paradigm promoting encapsulation of complex programs into simpler and more manageable units.
- The SIMPL toolbox contains library functions which allow well-defined message passing between these encapsulated elements.
- SIMPL can be used used locally and/or over a network of host machines.
- SIMPL modules are highly extensible and highly testable.
- SIMPL is a blocked message protocol.

# Introduction

Starting at the core level, the simpl ipc library which was written in C has been rewritten and cleaned up with a more modern C approach.
It has been found to be necessary to keep the C library for a number of reasons, particularly extending the C library functions to Python.

The C library has been wrapped to create a C++ library. Writing SIMPL programs in C++ is somewhat easier than writing in C since all of the original function name_attach() now called openSRY() and is called by the SRY class constructor when a SRY object is initiated. As well, the name_detach() now called closeSRY() is called by the class destructor. Extending the C and C++ with Boost, PyBind and SWIG have all met with failure for various reasons. Accordingly, the C library is manually wrapped for Python and JNI in the case of Java which is in development.

# Changes

The following is a list of changes made from simpl-3.3.8 to simpl-4.0.0 based on the directory entries under simpl-3.3.8 and simpl-4.0.0.

- `arduino-1.0.1/` - not yet ported.
- `benchmarks/` - sender and receiver programs rewritten from C in 3.3.8 to C++ in 4.0.0.
- `bin/` - repository for any executables in both 3.3.8 and 4.0.0.
- `docs/` - various readme's rewritten and relocated under applicable directories. For example, benchmark-explanation.txt found in ~/simpl-4.0.0/benchmarks.
- `fclogger/` - removed in 4.0.0 and made a library function called sryLog() which can print to screen and writes to a log file /var/tmp/sry.log. The fclogger can be a useful addition however but it depends on faithful sry given that it is a receiver.
- `include/` - contains 8 header files in 3.3.8 but only 3 in 4.0.0. Defines, enums etc. moved around.
- `java-0.2.0/` - not yet ported.
- `lib/` - contained the static and dynamic simpl C libraries in 3.3.8. This directory has been broken out to static-lib and dynamic-lib in 4.0.0. The static-lib contains the C and the C++ libraries for static linking. The dynamic-lib contains the C and the C++ libraries needed for dynamic linking at run time. for a C/C++ function synopsis in 4.0.0 check `/.../sim-lib/simlib-explanation.txt`. The same API applies to both the C functions and the C++ methods.
- `modules/` - changed to pymodules in 4.0.0.
- `python-3.0.1/` - changed to python in 4.0.0. Supports python 3.13.
- `scripts/` - still called scripts in 4.0.0. Now contains only startup and shutdown scripts for the TCP and RS232 surrogates.
- `simplipc/` - changed to sim-lib in 4.0.0. Instead of simnplipc.c and simplUtils.c it has all been placed and rewritten in a single file libsim.cpp with symbolic link libsim.c. Note the blurb above under lib.
- `simplUtils/` - changed to utilities in 4.0.0.
- `surrogates`/ - still called surrogates in 4.0.0. TCP/IP surrogate has been upgraded but not rigorously rested (only tested on a loopback). Note that the surrogates no longer need the protocol router and the C library name_locate() is now called Locate(). Locate() is a major rewrite of name_locate(). Locate() performs the functions of the protocol router thus saving an extra program having to be run in the background for network SIMPL. The tclSurrogate is still to be ported as necessary for arduino and also Java Ssimpl. RS232-surrogate is a work in progress.

- `tcl-1.5/` - not carried over to 4.0.0.
- `testing/` - still called testing in 4.0.0 but an entirely different set of test programs which are also still a work in progress.

# Additions

In 4.0.0 a new directory has been added called /.../simpl-4.0.0/example. The software contained provides an example of SIMPL communications between a Python sender and a C++ receiver.

The sender sends a message containing instructions for the receiver to draw a fractal image. See /.../simpl-4.0.0/example/example-explanation.txt.
