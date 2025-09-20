/******************************************************************************
FILE:			simcmodule.c

DATE:			January 23, 2025

DESCRIPTION:	This file contains the function calls (C wrappers)
				for the python sim library used by python programs.
				This is compiled into the simc.so shared object.

AUTHOR:			John Collins
******************************************************************************/

// should always be first
#define PY_SSIZE_T_CLEAN
#include <Python.h>

// sry header
#include <sim.h>

// defines
#define SENDER_LIMIT 128

// global variables
static void *SenderArray[SENDER_LIMIT];

// function prototypes
static void iniSenderArray(void);
static int setSenderArray(void *);
static void *getSenderArray(int);

/*#############################################################################
								Library Functions
##############################################################################*/

/**********************************************************************
FUNCTION:	sry_openSRY(PyObject *, PyObject *)

CALLED AS:	returnValue = sry.openSRY(programName)

PURPOSE:	Add sry capability to a python program.

RETURNS:	success: 0 
			failure: -1
***********************************************************************/

static PyObject *sry_openSRY(PyObject *self, PyObject *args)
{
int ret = -1;
char *name;

if (PyArg_ParseTuple(args, "s", &name))
	ret = openSRY(name);

return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_closeSRY(PyObject *, PyObject *)

CALLED AS:	returnValue = sry.closeSRY()

PURPOSE:	Remove sry receiver capability from a python program.

RETURNS:	0 
***********************************************************************/

static PyObject *sry_closeSRY(PyObject *self, PyObject *args)
{
closeSRY();

return Py_BuildValue("i", 0);
}

/**********************************************************************
FUNCTION:	sry_receive(PyObject *, PyObject *)

CALLED AS:	msgSize, senderId = sry.receive(inBuffer)

PURPOSE:	Receive an incoming sry message.

RETURNS:	success: message size >= 0 bytes 
			failure: message size = -1
			proxy: value < -1
***********************************************************************/

static PyObject *sry_receive(PyObject *self, PyObject *args)
{
int ret = -1, senderOffset;
Py_ssize_t iSize;
char *iPtr;
void *senderId;

if (PyArg_ParseTuple(args, "z#", &iPtr, &iSize))
	ret = Receive(&senderId, iPtr, iSize);

// load the sender address into the sender table
senderOffset = setSenderArray(senderId);

return Py_BuildValue("ii", ret, senderOffset);
}

/**********************************************************************
FUNCTION:	sry_reply(PyObject *, PyObject *)

CALLED AS:	replyMsgSize = sry.reply(senderId, outBuffer)

PURPOSE:	Reply to sender.

RETURNS:	success: reply message size in bytes >= 0
			failure: -1
***********************************************************************/

static PyObject *sry_reply(PyObject *self, PyObject *args)
{
int senderOffset, ret = -1;
Py_ssize_t oSize;
char *oPtr;
void *sender;

if (PyArg_ParseTuple(args, "iz#", &senderOffset, &oPtr, &oSize))
	{
	// get the actual sender address
	sender = getSenderArray(senderOffset);
	// call the actual simpl C library function
	ret = Reply(sender, oPtr, oSize);
	}

return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_returnProxy(PyObject *, PyObject *)

CALLED AS:	true proxy value = sry.returnProxy(received proxy value)

PURPOSE:	Return the true value of a triggered proxy.

RETURNS:	success: proxy >= 1
			failure: -1
***********************************************************************/

static PyObject *sry_returnProxy(PyObject *self, PyObject *args)
{
int incomingProxy, proxy = -1;

// parse out the passed in arguments
if (PyArg_ParseTuple(args, "i", &incomingProxy))
	proxy = returnProxy(incomingProxy);

return Py_BuildValue("i", proxy);
}

/**********************************************************************
FUNCTION:	sry_locate(PyObject *, PyObject *)

CALLED AS:	receiverId = sry.nameLocate(hostName, receiverName, largestMsgSize)

PURPOSE:	Get the id of a sry receiver.

RETURNS:	success: > 0 
			failure: -1
***********************************************************************/

static PyObject *sry_locate(PyObject *self, PyObject *args)
{
long receiverId = -1;
char *hostName, *recName;
int protocol, msgSize;

if (PyArg_ParseTuple(args, "ssii", &hostName, &recName, &msgSize, &protocol))
	receiverId = Locate(hostName, recName, msgSize, protocol);

return Py_BuildValue("l", receiverId);
}

/**********************************************************************
FUNCTION:	sry_send(PyObject *, PyObject *)

CALLED AS:	replyMsgSize = sry.send(receiverId, outBuffer, inBuffer)

PURPOSE:	Sends a sry message to a sry receiver.

RETURNS:	success: reply message size in bytes >= 0 
			failure: -1
***********************************************************************/

static PyObject *sry_send(PyObject *self, PyObject *args)
{
int ret = -1, id;
Py_ssize_t iSize, oSize;
char *iPtr, *oPtr;

if (PyArg_ParseTuple(args, "iz#z#", &id, &oPtr, &oSize, &iPtr, &iSize))
	ret = Send(id, oPtr, oSize, iPtr, iSize);

return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_trigger(PyObject *, PyObject *)

CALLED AS:	sry.trigger(receiverId, proxy)

PURPOSE:	Send a sry proxy to a sry receiver.

RETURNS:	success: 0 
			failure: -1
***********************************************************************/

static PyObject *sry_trigger(PyObject *self, PyObject *args)
{
int proxy, ret = -1, receiverId;

if (PyArg_ParseTuple(args, "ii", &receiverId, &proxy))
	ret = Trigger(receiverId, proxy);

return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_getSenderRplySize(PyObject *, PyObject *)

CALLED AS:	max reply msg size = sry.getSenderRplySize(receiverId)

PURPOSE:	Returns the largest reply (in bytes) a sender can accept.

RETURNS:	success: >= 0
			failure: -1
***********************************************************************/

static PyObject *sry_getSenderRplySize(PyObject *self, PyObject *args)
{
void *senderId;
int ret = -1;

// parse out the passed in arguments
if (PyArg_ParseTuple(args, "z", &senderId))
	ret =  getSenderRplySize(senderId);
	
return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_chkReceiver(PyObject *, PyObject *)

CALLED AS:	status = sry.chkReceiver(receiverId)

PURPOSE:	Checks on receiver status.

RETURNS:	success: 0 
			failure: -1
***********************************************************************/

static PyObject *sry_chkReceiver(PyObject *self, PyObject *args)
{
char *receiverName;
pid_t pid;
int ret = -1;

// parse out the passed in arguments
if (PyArg_ParseTuple(args, "s", &receiverName, pid))
	ret =  chkReceiver(receiverName, pid);
	
return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_chkSender(PyObject *, PyObject *)

CALLED AS:	status = sry.chkSender(senderId)

PURPOSE:	Checks on sender status.

RETURNS:	success: 0 
			failure: -1
***********************************************************************/

static PyObject *sry_chkSender(PyObject *self, PyObject *args)
{
void *senderId;
int ret = -1;

// parse out the passed in arguments
if (PyArg_ParseTuple(args, "z", &senderId))
	ret =  chkSender(senderId);
	
return Py_BuildValue("i", ret);
}

/**********************************************************************
FUNCTION:	sry_rfd(PyObject *, PyObject *)

CALLED AS:	fd = sry.rfd()

PURPOSE:	Returns the file descriptor to the receive fifo.

RETURNS:	success: > 2 
			failure: -1
***********************************************************************/

static PyObject *sry_rfd(PyObject *self, PyObject *args)
{
return Py_BuildValue("i", rfd());
}

/**********************************************************************
FUNCTION:	sry_yfd(PyObject *, PyObject *)

CALLED AS:	fd = sry.yfd()

PURPOSE:	Returns the file descriptor to the receive fifo.

RETURNS:	success: > 2 
			failure: -1
***********************************************************************/

static PyObject *sry_yfd(PyObject *self, PyObject *args)
{
return Py_BuildValue("i", yfd());
}

/**********************************************************************
FUNCTION:	sry_returnModuleVersion(PyObject *, PyObject *)

CALLED AS:	python version = sry.returnModuleVersion()

PURPOSE:	Return the python library version
***********************************************************************/

static PyObject *sry_returnModuleVersion(PyObject *self, PyObject *args)
{
// convert C return value to python object for return
return Py_BuildValue("i", 3);
}

/************ function name binding/registration table **************/

static PyMethodDef sryMethods[] =
	{
	{"openSRY", sry_openSRY, METH_VARARGS, NULL},
	{"closeSRY", sry_closeSRY, METH_NOARGS, NULL},
	{"receive", sry_receive, METH_VARARGS, NULL},
	{"reply", sry_reply, METH_VARARGS, NULL},
	{"returnProxy", sry_returnProxy, METH_VARARGS, NULL},
	{"locate", sry_locate, METH_VARARGS, NULL},
	{"send", sry_send, METH_VARARGS, NULL},
	{"trigger", sry_trigger, METH_VARARGS, NULL},
	{"maxRplySize", sry_getSenderRplySize, METH_VARARGS, NULL},
	{"chkReceiver", sry_chkReceiver, METH_VARARGS, NULL},	
	{"chkSender", sry_chkSender, METH_VARARGS, NULL},
	{"rfd", sry_rfd, METH_NOARGS, NULL},
	{"yfd", sry_yfd, METH_NOARGS, NULL},
	{"returnModuleVersion", sry_returnModuleVersion, METH_NOARGS, NULL},
	{NULL, NULL, 0, NULL}
	};

/************************ module definition **************************/

static struct PyModuleDef simcmodule =
	{
	PyModuleDef_HEAD_INIT,
	"simc",
	NULL,
	-1,
	sryMethods
	};

/********************** module initialization *************************/

PyMODINIT_FUNC PyInit_simc()
{
return( PyModule_Create(&simcmodule) );
}

/********************** sender table functions ***********************/

static void iniSenderArray()
{
for (int i = 0; i < SENDER_LIMIT; i++)
	SenderArray[i] = (void *)NULL;
}

static int setSenderArray(void *sender)
{
int i;

for (i = 0; i < SENDER_LIMIT; i++)
	{
	if (SenderArray[i] == NULL)
		{
		SenderArray[i] = sender;
		break;
		}
	}

return (i == SENDER_LIMIT) ? -1 : i;
}

static void *getSenderArray(int senderOffset)
{
void *sender;

sender = SenderArray[senderOffset];
SenderArray[senderOffset] = (void *)NULL;

return sender;
}
