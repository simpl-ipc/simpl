"""
FILE:			structRec.py

DATE:			February 4, 2025

DESCRIPTION:	This program receives a message, deconstructs the message using the 					struct module and then prints the message contents to the screen. 						Next, it constructs a reply message using the struct module which it 					then replies to the sender. The message contents are binary in nature.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

# import required modules
import sys
import struct
import sim

# set this program's name
rName = "RECEIVER_P"
SINT = struct.calcsize("i")

nee = sim.SRY(rName, 1024);

# receive a message
messageSize, senderId = nee.receive()
if messageSize == -1:
	# error
	print(rname, ": receive error")
	sys.exit(-1)

# message
inMsg = nee.returnInMsgBuffer()
t = struct.unpack("i", inMsg[0:SINT])
print(t[0])
t = struct.unpack("i", inMsg[SINT:2 * SINT])
print(t[0])
t = struct.unpack("i", inMsg[2 * SINT:3 * SINT])
print(t[0])

# reply to sending program
outMsg = struct.pack("=i", 7)
nee.loadOutMsgBuffer(outMsg)
retVal = nee.reply(senderId)
if retVal == -1:
	print(rname, ": reply error")
	sys.exit(-1)
