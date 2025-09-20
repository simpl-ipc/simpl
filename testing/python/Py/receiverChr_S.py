"""
FILE:			receiverChr_S.py

DATE:			February 4, 2025

DESCRIPTION:	This program receives a message, deconstructs the message using the 					short method and then prints the message contents to the screen. 						Next, it constructs a reply message using the short method which it 					then replies to the sender. The message contents are character in 						nature.

                Python 3 compatible only.

AUTHOR:			John Collins
"""



#import required modules
import sim
from sys import exit
from select import select

import testMsg as msg

myName = "RECEIVER_P"

# constructor for sry class object
nee = sim.SRY("RECEIVER_P", 1024)

noo = nee.rfd()
readDesc = []
readDesc.append(noo)

while 1:
	readables, writables, exceptions = select(readDesc, [], [])
	for ping in readables:
		if ping == noo:
			# receive a message
			messageSize, senderId = nee.receive()
			if messageSize == -1:
				print("receive error")
				exit(-1)

		 	# examine the message
			print("received msg size=", messageSize)
			msg.deconstructMsg_S(nee, myName, nee.CHR)
	
			# build reply message
			msg.constructMsg_S(nee, 1, nee.CHR)

			# reply to sending program
			retVal = nee.reply(senderId)
			if retVal == -1:
				print("reply error")
				exit(-1)
