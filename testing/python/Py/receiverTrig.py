"""
FILE:			receiverTrig.py

DATE:			February 4, 2025

DESCRIPTION:	This program receives a proxy and prints its value to the screen.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

#import required modules
import sim
import sys
import testMsg as msg

myName = "RECEIVER_P"

# constructor for sry class object
nee = sim.SRY(myName, 1024)

print("receiverFd =", nee.rfd())

while 1:
	# receive a message
	messageSize, senderId = nee.receive()
	if messageSize == -1:
		# error
		print(myName, " receive error")
		sys.exit(-1)

	if messageSize < -1:
		# trigger
		print("proxy =", nee.returnProxy(messageSize))
		
