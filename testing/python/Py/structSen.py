"""
FILE:			structSen.py

DATE:			February 4, 2025

DESCRIPTION:	This program constructs a message using the struct module, then sends 					the message to a receiver, then deconstructs the reply message using 					the struct module and prints the message contents to the screen.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

# import required modules
import sys
import struct
import sim

# initialize necessary variables
rName = "RECEIVER_P"
sName = "SENDER_P"

# create an instance of noo
noo = sim.SRY(sName, 1024)

# name locate the receiver program
receiverId = noo.locate("", rName, 1024, 0)
if receiverId == -1:
	print(sName, ": name locate error")
	sys.exit(-1)

# compose the message
outMsg = struct.pack("iii", 9, 99, 999)
noo.loadOutMsgBuffer(outMsg)

# send the message
retVal = noo.send(receiverId)
if retVal == -1:
	print(sName, ": send error")
	sys.exit(-1)

inMsg = noo.returnInMsgBuffer()
t = struct.unpack("=i", inMsg[0:struct.calcsize("i")])
print(t[0])
