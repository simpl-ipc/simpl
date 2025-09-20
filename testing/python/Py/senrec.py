"""
FILE:			senrecr.py

DATE:			February 4, 2025

DESCRIPTION:	This program receives a message, deconstructs the message using the 					long method and then prints the message contents to the screen. It 						then constructs a message using the short method which it sends to a 					receiver. Upon receiving a reply it then deconstructs the reply using 					the short method and prints it to the screen. Finally, it constructs a 				reply message using the long method which it then replies to the 						original sender. The message contents are binary in nature.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

# import required modules
import sys
import sim

import testMsg as msg

# set this sender/receiver's name
rName = "SENREC_P"

# set the destination receiver's name
dName = "RECEIVER_P"

# create an instance of a sim receiver
ping = sim.SRY(rName, 1024);

# name locate the receiver program
receiverId = ping.locate("", dName, 1024, ping.SIM_LOCAL)
if receiverId == -1:
	print(sName, ": name locate error")
	sys.exit(-1)

while True:
	# receive a message
	messageSize, senderId = ping.receive()
	if messageSize == -1:
		print(rName, ": receive error")
		sys.exit(-1)

	# print received message
	msg.deconstructMsg_L(ping, rName, ping.BIN)
	
	# build send and reply message
	msg.constructMsg_S(ping, 2, ping.BIN)

	# send the message onward
	retVal = ping.send(receiverId)
	if retVal == -1:
		print(rName, ": send error")
		sys.exit(-1)

	# print replied message
	msg.deconstructMsg_S(ping, rName, ping.BIN)

	# build send and reply message
	msg.constructMsg_L(ping, 4, ping.BIN)

	# reply to sending program
	retVal = ping.reply(senderId)
	if retVal == -1:
		print(rName, ": reply error")
		sys.exit(-1)
