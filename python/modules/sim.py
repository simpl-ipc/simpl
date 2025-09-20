"""
FILE:			sim.py

DATE:			January 23, 2025

DESCRIPTION:	Contains wrapped calls to simc.so which is a C SIMPL extension
				library.

AUTHOR:			John Collins
"""

# this file is to be imported as a module only
if __name__ == "__main__":
	print("sim.py is not to be run as a main program; it is a module only.")
	exit(-1)

# import some required module bits
from sys import exit, version
from struct import pack, unpack, calcsize

# which C extension library to import?
if version < '3':
	print("sim.py module must be run by Python version 3.*")
	exit(-1)

# import the C sim bits from the simc.so shared object
import simc

#############################################################################
# Msg class definition - useful bits used by the SRY class
#############################################################################
class Msg(object):
	# available constants for packing/unpacking
	BIN = -2
	CHR = -1

	# available constants for byte ordering
	NATIVE1 = '@'
	NATIVE2 = '='
	LITTLE_ENDIAN = '<'
	BIG_ENDIAN = '>'
	NETWORK = '!'

	# data model constants used in this module
	OUT_DM = 0
	IN_DM = 1

	# available constants for data model types
	LP64 = 0
	ILP64 = 1
	LLP64 = 2
	ILP32 = 3
	LP32 = 4

	def __init__(self):
		"""
		the default data model is made by setting sizes of the the various C
		data primitives based on the host computer where this function is
		run. Used for setting the default data sizes for message components.

		The data models governing the tcl command set and both message buffers
		are set to the default value ordinarily at start up.
		"""
		self.__DMstr = "?chilfd"
		self.__defaultDM = {}
		for key in self.__DMstr:
			self.__defaultDM[key] = calcsize(key)

		# default values for incoming (via receive/reply) message variables
		self.__inMsgDM = self.__defaultDM
		self.__inMsgByteOrder = Msg.NATIVE1
		self.__inMsgBuf = [None]
		self.__inMsgCtr = 0

		# default values for outgoing (via send/reply) messages variables
		self.__outMsgDM = self.__defaultDM
		self.__outMsgByteOrder = Msg.NATIVE1
		self.__outMsgBuf = [None]
		self.__outMsgCtr = 0

	#=====================================================================
	# message packing methods
	#=====================================================================

	def packMsg(self, mode, form, *args):
		"""
		packMsg() is currently concerned with C data types. Python types etc.
		are	not considered per say.

		packMsg() is a general packing routine used for constructing sim
		messages that are bound to be forwarded to a receiver program via a sim
		send() call. It is modeled after and uses the module struct.

		Message data may be packed in a binary or a character format. The
		'mode' defines the type of packing to occur: binary or character
		based.

		The 'form' defines a format string which defines the type, size and
		order of the data to be found in the outgoing message. The 'args' are
		the	data type values to be placed in the outgoing message buffer. The
		target is the receiver program in question which will receive the
		message format and disassemble it accordingly.

		The sizes given in bytes reflect a LP64 or a LP32 data model.

		The format string can be quantified as follows:
		'b': a single Boolean character (1 byte)
		'c': a single character string (1 byte)
		's': a character string (length of string in bytes)
		'h': a signed short integer (2 bytes)
		'i': a signed integer (4 bytes)
		'l': a signed long integer (4 bytes/32-bit or 8 bytes/64-bit)
		'f': a signed float (4 bytes)
		'd': a signed double (4 bytes/32-bit or 8 bytes/64-bit)
		'B': a Boolean array (size * 1 byte)
		'H': a signed short integer array (size * 2 bytes)
		'I': a signed integer array (size * 4 bytes)
		'L': a signed long integer array(size * 4 bytes/32-bit or 8
			bytes/64-bit)
		'F': a signed float array (size * 4 bytes)
		'D': a signed double array (size * 4 bytes/32-bit or 8 bytes/64-bit)

		A general format can be seen as follows:
		"bchilfd#s#H#I#L#F#D"
		where '#' is the size/number of data types in the string or array.

		The above format defines a message that is as follows: (recall C types)
		1. one single character (a Python boolean)
		2. one single character (1 byte, not unicode)
		3. one signed short integer
		4. one signed integer
		5. one signed long integer
		6. one floating point number
		7. one double precision floating point number
		8. one string # bytes long
		9. one Boolean array # long
		10. one signed short array of numbers # long
		11. one signed integer array of numbers # long
		12. one signed long array of numbers # long
		13. one floating point array of numbers # long
		14. one double precision floating point array of numbers # long

		The way the message is packed depends on:
		a) is it binary or character based?
		b) is the receiving architecture 32 or 64 bit?

		The outgoing message buffer is looked after internally to the module and
		the calling program (and programmer) need not be concerned, unlike the C
		version of sim.

		Returns: nothing.
		"""
		# check inputted command line
		if len(form) != len(args):
			print("Number of format types not equal to number of arg types.")
			exit(-1)
		if not form.isalpha():
			print("Format string contains non-alphanumeric character(s).")
			exit(-1)

		# delineate the form(at) string and process each variable type in args
		for (offset, item) in enumerate(form):
			if item == 'c' or item == 's':
				arg = args[offset].encode('utf-8')
			else:
				arg = args[offset]

			if item.isupper() or item == 's':
				# an array or string
				amount = len(arg)
			else:
				# a single value
				amount = 1

			kind = self.__outMsgByteOrder
			# single character True/False 1/0 or an aray of same
			if item == 'b' or item == 'B':
				atomicSize = self.__outMsgDM['?']
				kind += self.__setDataSize(Msg.OUT_DM, '?')
			# single character
			elif item == 'c':
				atomicSize = self.__outMsgDM['c']
				kind += self.__setDataSize(Msg.OUT_DM, 'c')
			# string of single byte characters
			elif item == 's':
				atomicSize = self.__outMsgDM['c']
				kind += str(len(arg)) + 's'
			# single short integer or an array of same
			elif item == 'h' or item == 'H':
				atomicSize = self.__outMsgDM['h']
				kind += self.__setDataSize(Msg.OUT_DM, 'h')
			# single integer or an array of same
			elif item == 'i' or item == 'I':
				atomicSize = self.__outMsgDM['i']
				kind += self.__setDataSize(Msg.OUT_DM, 'i')
				# single long integer or an array of same
			elif item == 'l' or item == 'L':
				atomicSize = self.__outMsgDM['l']
				kind += self.__setDataSize(Msg.OUT_DM, 'l')
			# single floating point number or an array of same
			elif item == 'f' or item == 'F':
				atomicSize = self.__outMsgDM['f']
				kind += self.__setDataSize(Msg.OUT_DM, 'f')
			# single double precision floating point number or an array of same
			elif item == 'd' or item == 'D':
				atomicSize = self.__outMsgDM['d']
				kind += self.__setDataSize(Msg.OUT_DM, 'd')
				# unknown C variable type
			else:
				print("Incorrect argument type =", item)
				exit(-1)

			# set counters and pack the out message buffer
			self.__setOutMsgBuf(atomicSize, mode)
			if item.isupper(): # an array
				for i in range(amount):
					self.__outMsgBuf += pack(kind, arg[i])
			else: # a single value or a string of byte characters
				self.__outMsgBuf += pack(kind, arg)
			self.__outMsgCtr += amount * atomicSize

	"""
	The following methods are of the form packType(value, mode). They pack a
	C 'Type' into the outgoing message buffer. The 'p' argument is the value of
	the C type and the 'mode' is the packing mode required; ie. binary (BIN)
	or character (CHR) based.

	These methods are offered as a choice in packing a message. They act as
	wrappers for packMsg(). A complex message can be made with a single	(and
	possibly very complicated) format call to packMsg() or by multiple calls to
	the following methods with a single format.

	They return nothing to the calling script.
	"""

	def packBool(self, p, mode):
		self.packMsg(mode, "b", p)

	def packChar(self, p, mode):
		self.packMsg(mode, "c", p)

	def packShort(self, p, mode):
		self.packMsg(mode, "h", p)

	def packInt(self, p, mode):
		self.packMsg(mode, "i", p)

	def packLong(self, p, mode):
		self.packMsg(mode, "l", p)

	def packFloat(self, p, mode):
		self.packMsg(mode, "f", p)

	def packDouble(self, p, mode):
		self.packMsg(mode, "d", p)

	def packString(self, p, mode):
		self.packMsg(mode, "s", p)

	def packBoolArray(self, p, mode):
		self.packMsg(mode, "B", p)

	def packShortArray(self, p, mode):
		self.packMsg(mode, "H", p)

	def packIntArray(self, p, mode):
		self.packMsg(mode, "I", p)

	def packLongArray(self, p, mode):
		self.packMsg(mode, "L", p)

	def packFloatArray(self, p, mode):
		self.packMsg(mode, "F", p)

	def packDoubleArray(self, p, mode):
		self.packMsg(mode, "D", p)

	#=======================================================================
	# message unpacking methods
	#=======================================================================

	def unpackMsg(self, mode, form):
		"""
		unpackMsg() is currently only concerned with C data types. Python types
		etc. are not considered per say (except the Boolean).

		unpackMsg() is a general unpacking routine used for deconstructing sim
		messages that have been received by a receiver program via a reply() to
		a send() call. It is modelled after and uses the module struct.

		The 'mode' defines the type of packing to occur: binary or character
		based. The 'form' defines a format string which defines the type, size
		and	order of the data to be found in the incoming message.

		The sizes given in bytes reflect a LP64 or a LP32 data model.

		The format string can be quantified as follows:
		'b': a single Boolean character (1 byte)
		'c': a single character string (1 byte)
		's': a character string (length of string in bytes)
		'h': a signed short integer (2 bytes)
		'i': a signed integer (4 bytes)
		'l': a signed long integer (4 bytes/32-bit or 8 bytes/64-bit)
		'f': a signed float (4 bytes)
		'd': a signed double (4 bytes/32-bit or 8 bytes/64-bit)
		'B': a Boolean array (size * 1 byte)
		'H': a signed short integer array (size * 2 bytes)
		'I': a signed integer array (size * 4 bytes)
		'L': a signed long integer array(size * 4 bytes/32-bit or 8
			bytes/64-bit)
		'F': a signed float array (size * 4 bytes)
		'D': a signed double array (size * 4 bytes/32-bit or 8 bytes/64-bit)

		A general format can be seen as follows:
		"bchilfd#s#B#H#I#L#F#D"
		where '#' is the size/number of data types in the string or array.

		Returns: a tuple of one or more members which corresponds to the
		elements of the extracted message.
		"""

		# check inputted command line for aberrations
		if not len(form):
			print("Format string must not be null.")
			exit(-1)
		if not form.isalnum():
			print("Format string contains non-alphanumeric character(s).")
			exit(-1)

		# determine the length of the working list based on the number of
		# variables and not the numerical values
		ctr = 0
		for ch in form:
			if ch.isalpha():
				ctr += 1

		# create a working list of values to be read into from inBuffer and
		# returned to the calling program
		args = [None] * ctr

		# string holding numerical values (string or array lengths) encountered
		length = ""

		# extract data based upon the variable type and size
		i = 0
		for item in form:
			# an alphabetic character in the format string
			if item.isalpha():
				# determine the size of the array or string
				if item.isupper() or item == 's':
					if not length: # a single valued string or array
						size = 1
					else:
						size = int(length)
				# a single value type
				else:
					size = 1
				# reset the length just in case it was used
				length = ""

				if item == 'b' or item == 'B':
					kind = self.__setDataSize(Msg.IN_DM, '?')
					atomicSize = self.__inMsgDM['?']
				elif item == 'c':
					kind = self.__setDataSize(Msg.IN_DM, 'c')
					atomicSize = self.__inMsgDM['c']
				elif item == 's':
					kind = str(size) + 's'
					atomicSize = self.__inMsgDM['c']
				elif item == 'h' or item == 'H':
					kind = self.__inMsgByteOrder
					kind += self.__setDataSize(Msg.IN_DM, 'h')
					atomicSize = self.__inMsgDM['h']
				elif item == 'i' or item == 'I':
					kind = self.__inMsgByteOrder
					kind += self.__setDataSize(Msg.IN_DM, 'i')
					atomicSize = self.__inMsgDM['i']
				elif item == 'l' or item == 'L':
					kind = self.__inMsgByteOrder
					kind += self. __setDataSize(Msg.IN_DM, 'l')
					atomicSize = self.__inMsgDM['l']
				elif item == 'f' or item == 'F':
					kind = self.__inMsgByteOrder
					kind += self.__setDataSize(Msg.IN_DM, 'f')
					atomicSize = self.__inMsgDM['f']
				elif item == 'd' or item == 'D':
					kind = self.__inMsgByteOrder
					kind += self.__setDataSize(Msg.IN_DM, 'd')
					atomicSize = self.__inMsgDM['d']
				else:
					print("Incorrect alphabetical type =", item)
					exit(-1)

				# set counters and unpack the in message buffer
				self.__setInMsgBuf(atomicSize, mode)
				if item.isupper():
					args[i] = [None] * size
					for j in range(size):
						begin = self.__inMsgCtr
						self.__inMsgCtr = end = begin + atomicSize
						# recall, unpack returns a tuple
						t = unpack(kind, self.__inMsgBuf[begin:end])
						args[i][j] = t[0]
				else:
					begin = self.__inMsgCtr
					self.__inMsgCtr = end = begin + atomicSize * size
					# recall, unpack returns a tuple
					t = unpack(kind, self.__inMsgBuf[begin:end])
					args[i] = t[0]
				i += 1 # only increment return value list now
			# a numeric value in the format string
			else:
				length += item # build an array or string length value

		# return the required arguments to the calling program
		if i == 1: # for unpackByte() etc. and unpackMsg() with single arguments
			return args[0]
		else: # for unpackMsg() with multiple arguments
			return args

	"""
	The following methods are of the form unpackType(mode). They unpack a C
	'Type' from the incoming message buffer. The 'mode' is the unpacking mode
	required; ie. binary (BIN) 	or character (CHR) based.

	Like the packType() methods, they are offered as a choice in unpacking a
	message and act as 	wrappers for unpackMsg().

	They return the 'Type' to the calling script.
	"""

	def unpackBool(self, mode):
		return self.unpackMsg(mode, 'b')

	def unpackChar(self, mode):
		return self.unpackMsg(mode, 'c')

	def unpackShort(self, mode):
		return self.unpackMsg(mode, 'h')

	def unpackInt(self, mode):
		return self.unpackMsg(mode, 'i')

	def unpackLong(self, mode):
		return self.unpackMsg(mode, 'l')

	def unpackFloat(self, mode):
		return self.unpackMsg(mode, 'f')

	def unpackDouble(self, mode):
		return self.unpackMsg(mode, 'd')

	def unpackString(self, i, mode):
		return self.unpackMsg(mode, str(i) + 's')

	def unpackBoolArray(self, i, mode):
		return self.unpackMsg(mode, str(i) + 'B')

	def unpackShortArray(self, i, mode):
		return self.unpackMsg(mode, str(i) + 'H')

	def unpackIntArray(self, i, mode):
		return self.unpackMsg(mode, str(i) + 'I')

	def unpackLongArray(self, i, mode):
		return self.unpackMsg(mode, str(i) + 'L')

	def unpackFloatArray(self, i, mode):
		return self.unpackMsg(mode, str(i) + 'F')

	def unpackDoubleArray(self, i, mode):
		return self.unpackMsg(mode, str(i) + 'D')

	#==============================================================
	# other methods
	#==============================================================

	def crossCopy(self):
		"""
		copies the contents of the outgoing message buffer to the incoming
		message buffer for reasons of testing and verification.
		"""
		self.__inMsgBuf = self.__outMsgBuf

	def returnInMsgBuffer(self):
		# returns the contents of the incoming message buffer.
		return self.__inMsgBuf

	def returnOutMsgBuffer(self):
		# returns the contents of the outgoing message buffer.
		return self.__outMsgBuf

	def loadOutMsgBuffer(self, message):
		# loads the contents of the outgoing message buffer.
		self.__outMsgBuf = message

	def setInMsgByteOrder(self, order):
		# sets the byte order on incoming messages.
		self.inMsgByteOrder = order

	def setOutMsgByteOrder(self, order):
		# sets the byte order on outgoing messages.
		self.__outMsgByteOrder = order

	def setDMs(self, dataModel, dataType):
		"""
		setDMs() sets the C variable sizes on the various data models (DMs,
		incoming messages or outgoing messages or tclSurrogate commands)
		depending on the data models.

		Note that _int32 or int32 and long long are NOT supported. Don't use
		them as part of the contents of your messages on the C side of
		things.

		The sizes are set in bytes rather than bits where 1 byte = 8 bits.
		"""

		if dataModel == Msg.IN_DM:
			self.__inMsgDM['?'] = 1
			self.__inMsgDM['c'] = 1
			self.__inMsgDM['h'] = 2
			self.__inMsgDM['f'] = 4
			self.__inMsgDM['d'] = 8

			if dataType == Msg.LP64:
				self.__inMsgDM['i'] = 4
				self.__inMsgDM['l'] = 8
			elif dataType == Msg.ILP64:
				self.__inMsgDM['i'] = 8
				self.__inMsgDM['l'] = 8
			elif dataType == Msg.LLP64:
				self.__inMsgDM['i'] = 4
				self.__inMsgDM['l'] = 4
			elif dataType == Msg.ILP32:
				self.__inMsgDM['i'] = 4
				self.__inMsgDM['l'] = 4
			elif dataType == Msg.LP32:
				self.__inMsgDM['i'] = 2
				self.__inMsgDM['l'] = 4
			else:
				print("setDMs: Error choosing supported input data type.")
		elif dataModel == Msg.OUT_DM:
			self.__outMsgDM['?'] = 1
			self.__outMsgDM['c'] = 1
			self.__outMsgDM['h'] = 2
			self.__outMsgDM['f'] = 4
			self.__outMsgDM['d'] = 8

			if dataType == SRY_LP64:
				self.__outMsgDM['i'] = 4
				self.__outMsgDM['l'] = 8
			elif dataType == SRY_ILP64:
				self.__outMsgDM['i'] = 8
				self.__outMsgDM['l'] = 8
			elif dataType == SRY_LLP64:
				self.__outMsgDM['i'] = 4
				self.__outMsgDM['l'] = 4
			elif dataType == SRY_ILP32:
				self.__outMsgDM['i'] = 4
				self.__outMsgDM['l'] = 4
			elif dataType == SRY_LP32:
				self.__outMsgDM['i'] = 2
				self.__outMsgDM['l'] = 4
			else:
				print("setDMs: Error choosing supported output data type.")
		else:
			print("setDMs: Error choosing supported data model.")

	def resetInMsgBuffer(self, size):
		# re-initializes the incoming message buffer and byte counter.
		self.__inMsgBuf = b" " * size
		self.__inMsgCtr = 0

	def resetOutMsgBuffer(self):
		# re-initializes the outgoing message buffer and byte counter.
		self.__outMsgBuf = b""
		self.__outMsgCtr = 0

	def what(self, which, numlines = 15):
		# good for printing out text files such as error logs
		open(which).read()
		lines = which.splitlines()
		while lines:
			chunk = lines[:numlines]
			lines = lines[numlines:]
			for line in chunk:
				print(line)
			if lines and input('More?') not in ['Y', 'y']:
				break

	#==============================================================
	# Auxiliary methods needed by pack/unpack methods
	#==============================================================

	def __setDataSize(self, dataModel, datum):
		"""
		it is the C int and C long size value in bytes that differs between the
		data models recognized in this module.

		dataModel is as below, datum is one of "?chilfd", usually i or l since
		they are the ones that differ between data models.
		"""
		if dataModel == Msg.IN_DM:
			dm = self.__inMsgDM
		elif dataModel == Msg.OUT_DM:
			dm = self.__outMsgDM
		else:
			print("setDataSize: Error choosing supported data model.")
			return -1

		if dm[datum] == self.__defaultDM[datum]:
				return datum
		else:
			for key in self.__DMstr:
				if dm[datum] == self.__defaultDM[key]:
					return key

		print("setDataSize: Error choosing supported data model key type.")
		return -1

	def __setInMsgBuf(self, size, mode):
		"""
		setInBuf() grows the incoming message buffer and increments the	incoming
		message buffer byte counter appropriately depending on the "mode".

		Binary (BIN) unpacks messages in a memory format C can understand.
		Character (CHR) unpacks messages directly.
		"""
		if mode == Msg.BIN: # binary unpacking for incoming messages
			while self.__inMsgCtr % size:
				self.__inMsgCtr += 1

	def __setOutMsgBuf(self, size, mode):
		"""
		setOutBuf() grows the outgoing message buffer and increments the
		outgoing message buffer byte counter appropriately depending on the
		"mode".

		Binary (BIN) packs messages in a memory format C can understand.
		Character (CHR) packs messages directly.
		"""
		if mode == Msg.BIN: # binary packing for outgoing messages
			while self.__outMsgCtr % size:
				self.__outMsgBuf += b"\000"
				self.__outMsgCtr += 1
#############################################################################
#	End of Msg class definition
#############################################################################

#############################################################################
#	Start of SRY class definition
#############################################################################
class SRY(Msg):
	# needed for Locate protocols and found in sim.h under SURR_PROTOCOLS
	SIM_LOCAL = 0
	SIM_TCP = 1
	SIM_RS232 =2

	# constructor
	def __init__(self, name, maxMsgSize):
		# run the Msg base class constructor
		Msg.__init__(self)

		# run simc sim receiver start up
		simc.openSRY(name)

		# set in in/out msg buffers
		Msg.resetInMsgBuffer(self, maxMsgSize)
		Msg.resetOutMsgBuffer(self)

	# destructor
	def __del__(self):
		# close up sim bits
		simc.closeSRY()

	def receive(self):
		"""
		receive() receives the messages sent by send() or trigger(). It blocks
		awaiting an incoming message/trigger.

		Returns: success (messageSize >= 0) and senderId (>=0), or failure
		(messageSize = -1).
		"""
		self.resetInMsgBuffer(len(self._Msg__inMsgBuf))
		return simc.receive(self._Msg__inMsgBuf)

	def reply(self, senderId):
		"""
		reply() replies a message or a null message received by send(). Upon
		receiving the reply, send() unblocks.

		Returns: success (reply msg size in bytes >= 0) or failure (-1).
		"""
		ret = simc.reply(senderId, self._Msg__outMsgBuf)
		self.resetOutMsgBuffer() # after reply
		return ret

	def returnProxy(self, value):
		"""
		When a proxy is received (receive() via a trigger()), its value is <=-2.
		In order to regain the true value of the proxy, this 'value' is passed
		into returnProxy().

		Returns: the true value of the proxy (>= 0)
		"""
		return simc.returnProxy(value)

	def locate(self, hname, rname, msgSize, protocol):
		"""
		nameLocate makes a communication connection with a another sim based
		program. 'hname'=host name or IP address, 'rname'=the sim name of the
		process with which communications are desired.

		This method is typically used by 'sender' processes to make a
		connection to a 'receiver' process for the purposes of exchanging data.

		Example: hname="concord", rname="tim" and msgSize=1024 where the
		computer host name is 'concord', the receiver sim name is 'tim' and the
		maximum size of a sent or replied message is 1024 bytes.

		Example: hname="127.0.0.1", rname="zut" and msgSize=100 such that the
		host name is the 'local host' (loopback), the receiver name is 'zut'
		(just zut) and the maximum message size is 100 bytes.

		Returns: the unique sim Id of the receiver (>= 0) or failure (-1)
		"""
		return simc.locate(hname, rname, msgSize, protocol)

	def send(self, id):
		"""
		send() delivers a message that has been preformed via the use of the
		packMsg() method to the sim receiver program with sim identification
		number 'id' and then blocks awaiting a reply.

		Returns: the number of bytes replied (>= 0) or failure (-1)
		"""
		self.resetInMsgBuffer(len(self._Msg__inMsgBuf)) # before send
		ret = simc.send(id, self._Msg__outMsgBuf, self._Msg__inMsgBuf)
		self.resetOutMsgBuffer() # after send
		return ret

	def trigger(self, receiverId, proxy):
		"""
		trigger() delivers a proxy 'proxy' (>= 0) to the sim receiver program
		with sim identification number 'receiverId'. It does not block awaiting
		a reply.

		Returns: success (0) or failure (-1)
		"""
		return simc.trigger(receiverId, proxy)

	def rfd(self):
		# Returns: success (receive fd >= 3) or failure (-1).
		return simc.rfd()

	def yfd(self):
		# Returns: success (sender fd >= 3) or failure (-1).
		return simc.yfd()
#############################################################################
#	End of SRY class definition
#############################################################################
