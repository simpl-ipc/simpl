"""
FILE:			TestMsg.py

DATE:			January 23, 2025

DESCRIPTION:	Performs the construction and deconstruction (writing and reading)
				of a canned message used for testing the packing/unpacking methods
				for SIMPL messages. Every available variable type is used and so this also provides an example of how to write and read a message is performed.

AUTHOR:			John Collins
"""
# import required modules
from array import array
import sim

def constructMsg_L(noo, msgNum, mode):
	# create arrays of all supported types for an outgoing message
	a = array("b", [True, False, True, False, True, False, True, False])
	b = array("h", [0, 1, 2, 3, 4, 5, 6, 7])
	c = array("i", [0, 1, 2, 3, 4, 5, 6, 7])
	d = array("l", [0, 1, 2, 3, 4, 5, 6, 7])
	e = array("f", [1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8])
	f = array("d", [1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8])

	# pack the arrays and other C data types into the outgoing message buffer
	noo.packShort(msgNum, mode)
	noo.packBool(True, mode)
	noo.packChar("A", mode)
	noo.packShort(2, mode)
	noo.packInt(37, mode)
	noo.packLong(42424242, mode)
	noo.packFloat(2.1718, mode)
	noo.packFloat(3.1415, mode)
	noo.packDouble(1.602e-19, mode)
	noo.packString("We are the knights who say nee!!", mode)
	noo.packBoolArray(a, mode)
	noo.packShortArray(b, mode)
	noo.packIntArray(c, mode)
	noo.packLongArray(d, mode)
	noo.packFloatArray(e, mode)
	noo.packDoubleArray(f, mode)

def constructMsg_S(noo, msgNum, mode):
	# create arrays of all supported types for an outgoing message
	a = array("b", [True, False, True, False, True, False, True, False])
	b = array("h", [0, 1, 2, 3, 4, 5, 6, 7])
	c = array("i", [0, 1, 2, 3, 4, 5, 6, 7])
	d = array("l", [0, 1, 2, 3, 4, 5, 6, 7])
	e = array("f", [1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8])
	f = array("d", [1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8])

	# pack the arrays and other C data types into the outgoing message buffer
	noo.packMsg(mode, "hbchilffdsBHILFD", msgNum, True, "A", 2, 37, 42424242,
		2.1718, 3.1415, 1.602e-19, "We are the knights who say nee!!", a, b, c,
		d, e, f)

def deconstructMsg_L(noo, prog, mode):
	# extract and print the contents of the incoming message
	print(prog, " msg ", noo.unpackShort(mode))
	print(noo.unpackBool(mode))
	print(noo.unpackChar(mode))
	print(noo.unpackShort(mode))
	print(noo.unpackInt(mode))
	print(noo.unpackLong(mode))
	print(noo.unpackFloat(mode))
	print(noo.unpackFloat(mode))
	print(noo.unpackDouble(mode))
	print(noo.unpackString(32, mode))
	print(noo.unpackBoolArray(8, mode))
	print(noo.unpackShortArray(8, mode))
	print(noo.unpackIntArray(8, mode))
	print(noo.unpackLongArray(8, mode))
	print(noo.unpackFloatArray(8, mode))
	print(noo.unpackDoubleArray(8, mode), end='\n\n')

def deconstructMsg_S(noo, prog, mode):
	# extract and print the contents of the incoming message
	g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v = \
				noo.unpackMsg(mode, "hbchilffd32s8B8H8I8L8F8D")

	# print the msg contents	
	print(prog, " msg ", g)
	print(h)
	print(i)
	print(j)
	print(k)
	print(l)
	print(m)
	print(n)
	print(o)
	print(p)
	print(q)
	print(r)
	print(s)
	print(t)
	print(u)
	print(v, end='\n\n')
