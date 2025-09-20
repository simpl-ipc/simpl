"""
FILE:			packChrMsg.py

DATE:			February 4, 2025

DESCRIPTION:	This program tests the packing/unpacking methods attached to a
                class instance of sim. The packing mode is character. Both the shorter and longer versions are exercised with identical messages to serves as a comparator. It also serves as a good example of how to pack and unpack messages.

                Python 3 compatible only.

AUTHOR:			John Collins
"""

#import required modules
import sim
import testMsg

# create a sry instance
nee = sim.SRY("packer", 1024)

# pack an outgoing message
testMsg.constructMsg_S(nee, 1, nee.CHR)

# copy the outgoing msg buffer to the incoming msg buffer for comparison
nee.crossCopy()

# unpack and print out the message from the incoming message buffer
testMsg.deconstructMsg_S(nee, "packMsgChar", nee.CHR)

# rest the msg buffers
nee.resetInMsgBuffer(0)
nee.resetOutMsgBuffer()

# pack an outgoing message
testMsg.constructMsg_L(nee, 2, nee.CHR)

# copy the outgoing msg buffer to the incoming msg buffer for comparison
nee.crossCopy()

# unpack and print out the message from the incoming message buffer
testMsg.deconstructMsg_L(nee, "packMsgChar", nee.CHR)
