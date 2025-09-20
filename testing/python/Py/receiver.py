"""
FILE:			receiver.py

DATE:			February 4, 2025

DESCRIPTION:	This program receives a message, deconstructs the message using the 					short method and then prints the message contents to the screen. 						Next, it constructs a reply message using the long method which it 						then replies to the sender. The message contents are binary in nature.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

# import required modules
import sys
import sim
import testMsg as msg

# set this program's name
rName = "RECEIVER_P"

# create an instance of a sry receiver
neeewhaam = sim.SRY(rName, 1024);

while True:
	# receive a message
	messageSize, senderId = neeewhaam.receive()
	if messageSize == -1:
		print(rName, ": receive error")
		sys.exit(-1)

	# print received message
	msg.deconstructMsg_S(neeewhaam, rName, neeewhaam.BIN)
	
	# build reply message
	msg.constructMsg_L(neeewhaam, 3, neeewhaam.BIN)

	# reply to sending program
	retVal = neeewhaam.reply(senderId)
	if retVal == -1:
		print(rName, ": reply error")
		sys.exit(-1)
