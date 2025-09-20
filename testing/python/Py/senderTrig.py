"""
FILE:			senderTrig.py

DATE:			February 4, 2025

DESCRIPTION:	This program sends a proxy valued at 42 to a receiver.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

#import required modules
import sim
import sys

import testMsg as msg

# set this program's name
myName = "SENDER_P"

# set the receiver program's name
rName = "RECEIVER_P"

# constructor for sry class object
nee = sim.SRY(myName, 1024)

# name locate receiver program
id = nee.locate("", rName, 1024, nee.SIM_LOCAL)
if id == -1:
	print(myName, " name locate error")
	sys.exit(-1)

# send the message to the receiver program
ret = nee.trigger(id, 42)
if ret == -1:
	print(myName, " send error")
	sys.exit(-1)


