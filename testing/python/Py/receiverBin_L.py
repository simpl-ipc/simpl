"""
FILE:			receiverBin_L.py

DATE:			February 4, 2025

DESCRIPTION:	This program receives a message, deconstructs the message using the 					long method and then prints the message contents to the screen. 						Next, it constructs a reply message using the long method which it 						then replies to the sender. The message contents are binary in nature.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

#import required modules
from sys import exit
import sim
import testMsg as msg

myName = "RECEIVER_P"

# create an instance of sry
nee = sim.SRY(myName, 1024)

while True:
	# receive a message
	messageSize, senderId = nee.receive()
	if messageSize == -1:
		print("receive error")
		exit(-1)

	# examine the message
	print("received msg size=", messageSize)
	msg.deconstructMsg_L(nee, myName, nee.BIN)
	
	# build reply message
	msg.constructMsg_L(nee, 1, nee.BIN)

	# reply to sending program
	retVal = nee.reply(senderId)
	if retVal == -1:
		print("reply error")
		exit(-1)
