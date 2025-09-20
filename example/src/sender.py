"""
FILE:			sender.py

DATE:			January 23, 2025

DESCRIPTION:	This program acts as a local test python sender to fractals.cpp
				(receiver).

AUTHOR:			John Collins
"""

import sim
from array import array
import sys

c = array("i", [13,1,2,3,4,5,6,7,8,9,10,11,12,0,14,15])

# create an instance of sry
nee = sim.SRY("exampleSender", 1024)

nee.packMsg(nee.BIN, "iiiiidddddddiI", 2, 0, 1600, 1000,512, 4, 
1.2, 1.2, -2.0, -1.2, -2.0, 1.2, 16, c)

# name locate receiver program
nid = nee.locate("", "FractalReceiver", 1024, nee.SIM_LOCAL)
if nid == -1:
	print(myName, ": name locate error")
	sys.exit()

# send the message to the receiver program
ret = nee.send(nid)
if ret == -1:
	print(myName, ": send error")
	sys.exit()
