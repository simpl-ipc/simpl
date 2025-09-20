"""
FILE:			senderChr_S.py

DATE:			February 4, 2025

DESCRIPTION:	This program constructs a message using short method, then sends the 					message to a receiver, then deconstructs the reply message using the 					short method and then prints the message contents to the screen. The 					message contents are character based.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

#import required modules
import sim
from array import array
import sys

import testMsg as msg

rName = "RECEIVER_P"
myName = "SENDER_P"

# constructor for sry class object
nee = sim.SRY(myName, 1024)

# name locate receiver program
id = nee.locate("", rName, 1024, nee.SIM_LOCAL)
if id == -1:
	print(myName, " name locate error")
	sys.exit(-1)

# build outgoing message
msg.constructMsg_S(nee, 1, nee.CHR)

# send the message to the receiver program
ret = nee.send(id)
if ret == -1:
	print(myNAme, " send error")
	sys.exit(-1)

# examine the return message
print("received msg size=", ret)
msg.deconstructMsg_S(nee, myName, nee.CHR)

