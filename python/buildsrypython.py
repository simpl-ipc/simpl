"""
FILE:			buildsrypython.py

DATE:			January 23, 2025

DESCRIPTION:	Interactively build all Python 3 SIM bits and pieces.
				Calls top level Python make file.

AUTHOR:			John Collins
"""

import os, sys

# find whatever version 3.* of python to support the SIMPL extension library
numVers = 0
if os.path.isdir("/usr/include"):
	subDir = os.popen("ls -d1 /usr/include/python3.* 2>/dev/null")
	vers1 = subDir.readlines()
	numVers += len(vers1)
if os.path.isdir("/usr/local/include"):
	subDir = os.popen("ls -d1 /usr/local/include/python3.* 2>/dev/null")
	vers2 = subDir.readlines()
	numVers += len(vers2)

if numVers == 0:
	print("Cannot find any Python files to build the extended SIM library.")
	sys.exit()
else:
	vers = vers1 + vers2
	print("The following useable python version(s) were found:")
	useable = 0
	for i in range(numVers):
		if os.path.isfile(vers[i][0:len(vers[i])-1] + "/" + "Python.h"):
			print(vers[i][19:len(vers[i])-1])
			useable += 1
			
if useable == 0:
	print("There are no useable versions of Python needed for C extensions")
	sys.exit()
	
while True:
	ver = input("Choose a version for the SIM extended library> ")
	print("You have chosen python version %s" %ver)
	decision = input("Is this correct (yes/no/quit)? y/n/q> ")		
		
	if decision == 'y':
		break
	elif decision == 'n':
		continue
	else:
		sys.exit()

# make the extended SIM libraries/modules
here, me = os.path.split(os.path.abspath(__file__))
os.chdir(here)
cmd = "make" + " PYTHONVER=" + ver + " all"
os.system(cmd)
