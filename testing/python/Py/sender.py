"""
FILE:			sender.py

DATE:			February 4, 2025

DESCRIPTION:	This program constructs a message using short method, then sends the 					message to a receiver, then deconstructs the reply message using the 					short method and then prints the message contents to the screen. 						It does this 100 times.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

# import required modules
import sys
import sim
import testMsg as msg

# set this program's name
sName = "SENDER_P"

# set the receiver program's name
rName = "SENREC_P"

# create an instance of a sry sender
nee = sim.SRY(sName, 1024);

# name locate the receiver program
receiverId = nee.locate("", rName, 1024, nee.SIM_LOCAL)
if receiverId == -1:
	print(sName, ": name locate error")
	sys.exit(-1)

for ctr in range(1, 101, 1):
	# build send message
	msg.constructMsg_S(nee, 1, nee.BIN)

	# send the message onward
	retVal = nee.send(receiverId)
	if retVal == -1:
		print(sName, ": send error")
		sys.exit(-1)

	# print replied message
	msg.deconstructMsg_S(nee, sName, nee.BIN)

	# print the loop number
	print("-------------" , " loop ", ctr, " -------------\n\n")
