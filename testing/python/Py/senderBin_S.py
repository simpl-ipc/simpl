"""
FILE:			senderBin_S.py

DATE:			February 4, 2025

DESCRIPTION:	This program constructs a message using short method, then sends the 					message to a receiver, then deconstructs the reply message using the 					short method and then prints the message contents to the screen. The 					message contents are binary in nature.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

#import required modules
from array import array
import sys
import sim
import testMsg as msg

myName = "SENDER_P"
rName = "RECEIVER_P"

# create an instance of sry
nee = sim.SRY(myName, 1024)

# name locate receiver program
nid = nee.locate("", rName, 1024, nee.SIM_LOCAL)
if nid == -1:
	print(myName, ': name locate error')
	sys.exit(-1)

# build outgoing message
msg.constructMsg_S(nee, 1, nee.BIN)

# send the message to the receiver program
ret = nee.send(nid)
if ret == -1:
	print(myName, ': send error')
	sys.exit(-1)

# examine and display the reply message
msg.deconstructMsg_S(nee, myName, nee.BIN)
