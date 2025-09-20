"""
FILE:			fractals.py

DATE:			January 23, 2025

DESCRIPTION:	This gui program provides an interface for a user to enter 
				various numbers into a message, then sends this message to a 
				C++ based program called fractals which then produces a fractal 
				graphical display.

				The message passing between this program and the fractal 
				generating program is based on the C++ extension library for 
				sim, viz. sim.py

AUTHOR:			John Collins
"""

# import necessary modules
import sim
import sys
from array import array
from tkinter import *
from tkinter.font import Font

global choice
global subchoice
global msg

def drawList(lst, cmd):
	# Listbox creator general to this program
	l = Listbox(root, selectmode=SINGLE, font='Helvetica 15 bold')
	for act in lst:
		l.insert(END, act)
	l.grid()
	l.config(width=0, height=0)
	l.bind("<<ListboxSelect>>", cmd)
	
def select1(event): # from fractalList
	value = event.widget.get(event.widget.curselection()[0])
	event.widget.forget()
	event.widget.destroy()
	
	if value == 'Save':
		save()
	elif value == 'Play':
		play()
		drawList(fractalList, select1)
	elif value == 'Clear':
		clear()
		drawList(fractalList, select1)
	elif value == 'Quit':
		quit()
	else: # one of the fractals
		global choice
		choice = value
		if choice == 'Trig':
			drawList(trigList, select3)
		else:
			drawList(actionList, select2)
		
def select2(event): # from actionList
	value = event.widget.get(event.widget.curselection()[0])
	event.widget.forget()
	event.widget.destroy()
	
	if value == 'Default':
		setDefaultMsg()
		clear()
		sendMpacking(event)
		drawList(fractalList, select1)
	elif value == 'Modify Fractal':
		setDefaultMsg()
		modifyFractal()
	elif value == 'Modify Colours':
		setDefaultMsg()
		modifyColours()
	elif value == 'Back':
		drawList(fractalList, select1)
	else:
		quit()

def select3(event): # from trigList
	value = event.widget.get(event.widget.curselection()[0])
	event.widget.forget()
	event.widget.destroy()
	
	if value == 'Quit':
		quit()
	elif value == 'Back':
		drawList(fractalList, select1)
	else: # a fractal
		global subchoice
		subchoice = value
		drawList(actionList, select2)

def modifyFractal():
	# put up the window for modifying the default fractal message values
	Label(root, text=choice, font='Helvetica 18 bold italic').grid(row=0)

	"""
	Spinbox widget parameters saved in tuples, used for changing
	the message member values sent to the fractal drawing program
	(label, row, default setting, increment, low, high, format)
	"""
	a0 = ('Columns: integer 50 to 2000', 1, str(msg[3]), 10, 50, 2000, "")
	a1 = ('Rows: integer 50 to 1500', 2, str(msg[4]), 10, 50, 1500, "")
	a2 = ('Iterations: integer 10 to 1000', 3, str(msg[5]), 1, 10, 1000, "")
	a3 = ('Divergence: float 1.0 to 100000.0', 4, str(msg[6]), 1.0, 1.0, 
100000.0, "%.1f")
	a4 = ('Xmax: float -10.0 to 10.0', 5, str(msg[7]), 0.01, -10.0, 10.0,"%.7f")
	a5 = ('Ymax: float -10.0 to 10.0', 6, str(msg[8]), 0.01, -10.0, 10.0,"%.7f")
	a6 = ('Xmin: float -10.0 to 10.0', 7, str(msg[9]), 0.01, -10.0, 10.0,"%.7f")
	a7 = ('Ymin: float -10.0 to 10.0', 8, str(msg[10]), 0.01, -10.0,10.0,"%.7f")
	if choice == 'Julia':
		a8 = ('P: float -1.000000 to 1.000000', 9, str(msg[11]), 0.000001, -1,
			1, "%.6f")
		a9 = ('Q: float 0.0000000 to 1.000000', 10, str(msg[12]), 0.000001, 0, 
			1, "%.6f")
	else:
		a8 = ('P: float -5.0 to 5.0', 9, str(msg[11]), 0.01, -5, 5, "%.7f")
		a9 = ('Q: float -1.0 to 1.0', 10, str(msg[12]), 0.01, -1, 1, "%.7f")
		
	# a tuple of the above tuples for the spinners in the following 'for' loop	
	a = (a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)

	for index in range(10):
		if index == 9 and choice == 'Barnsley': # skip Q for Barnsley
			continue
		spinners1(index, a[index])

	b1 = Button(root, justify=CENTER, text='Back')
	b1.grid(row=11, column=0)
	b1.bind('<Button-1>', back2)

	b2 = Button(root, justify=CENTER, text='Send')
	b2.grid(row=11, column=1)
	b2.bind('<Button-1>', sendMpacking)

def modifyColours():
	# put up the window for modifying the default fractal colour values
	Label(root, text=choice, font='Helvetica 18 bold italic').grid(row=0)
	"""
	Spinbox widget parameters saved in tuples, used for changing
	the message member values sent to the fractal drawing program
	(label, row, default setting, increment, low, high, format)
	"""
	if choice == 'Julia':
		numColours = len(cIndex2)
	else:
		numColours = len(cIndex1)
 			
	for index in range(numColours):
		label = 'Colour ' + str(index+1) + ' : integer 0 to 15'
		colourList = (label, index+1, str(msg[14][index]), 1, 0, 15, "")
		spinners2(index, colourList)

	b1 = Button(root, justify=CENTER, text='Back')
	b1.grid(row=numColours+2, column=0)
	b1.bind('<Button-1>', back2)

	b2 = Button(root, justify=CENTER, text='Send')
	b2.grid(row=numColours+2, column=1)
	b2.bind('<Button-1>', sendMpacking)

def	spinners1(index, a):
	# put up the spin boxes to adjust message parameters
	index += 2
	l = Label(root, text=a[0], font='Helvetica 15 bold')
	l.grid(row=a[1], sticky=W)
	v = StringVar(root)
	v.set(a[2])
	s = Spinbox(root, increment=a[3], from_=a[4], to=a[5], wrap=True,
		font='Helvetica 15 bold', textvariable=v, format=a[6],
		command=(lambda: msgElement1(index, s.get())))
	s.grid(row=a[1], column=1)

def	spinners2(index, a):
	# put up the spin boxes to adjust message parameters
	l = Label(root, text=a[0], font='Helvetica 15 bold')
	l.grid(row=a[1], sticky=W)
	v = StringVar(root)
	v.set(a[2])
	s = Spinbox(root, increment=a[3], from_=a[4], to=a[5], wrap=True,
		font='Helvetica 15 bold', textvariable=v, format=a[6],
		command=(lambda: msgElement2(index, s.get())))
	s.grid(row=a[1], column=1)

def msgElement1(index, value):
	# ensure that the following values are the correct types
	if index <= 5 or index == 13:
		value = int(value)
	if index >= 6 and index <= 12:
		value = float(value)
	global msg
	msg[index] = value

def msgElement2(index, value):
	# ensure that the values are ints
	global msg
	msg[14][index] = int(value)

def back1(event):
	# jump to previous listing
	for x in root.grid_slaves():
		x.grid_remove() 
	drawList(fractalList, select1)

def back2(event):
	# jump to previous listing
	for x in root.grid_slaves():
		x.grid_remove() 
	drawList(actionList, select2)

def setDefaultMsg():
	# something to start with
	global msg

	# populate the outgoing message
	match choice:		
		case 'Mandelbrot':
			for m in range(len(m0)):
				msg[m] = msgTup[0][m]
				
		case 'Julia':
			for m in range(len(m1)):
				msg[m] = msgTup[1][m]
			
		case 'Trig':
			match subchoice:
				case 'Cos':
					for m in range(len(m2)):
						msg[m] = msgTup[2][m]
				case 'Sin':
					for m in range(len(m3)):
						msg[m] = msgTup[3][m]
				case 'Cosh':
					for m in range(len(m4)):
						msg[m] = msgTup[4][m]
				case 'Sinh':
					for m in range(len(m5)):
						msg[m] = msgTup[5][m]
				case 'Exp':
					for m in range(len(m6)):
						msg[m] = msgTup[6][m]
				case _:
					print("unknown subchoice")
					
		case 'Legendre': 
			for m in range(len(m7)):
				msg[m] = msgTup[7][m]
			
		case 'Laguerre':
			for m in range(len(m8)):
				msg[m] = msgTup[8][m]
			
		case 'Dragon':
			for m in range(len(m9)):
				msg[m] = msgTup[9][m]
			
		case 'Tchebychev':
			for m in range(len(m10)):
				msg[m] = msgTup[10][m]
			
		case 'Barnsley':
			for m in range(len(m11)):
				msg[m] = msgTup[11][m]
			
		case 'Sierpinski':
			for m in range(len(m12)):
				msg[m] = msgTup[12][m]
			
		case 'Phoenix':
			for m in range(len(m13)):
				msg[m] = msgTup[13][m]
		
		case _:
			print("unknown choice")			
			
def sendMpacking(event):
	# pack the message
	nee.packMsg(nee.BIN, "iiiiiidddddddiIs", msg[0], msg[1], msg[2],
		msg[3],	msg[4],	msg[5], msg[6], msg[7], msg[8], msg[9], msg[10], 
		msg[11], msg[12], msg[13], msg[14], msg[15])

	"""
	# alternate message packing scheme - good for checking one thing at a time 
	nee.packInt(msg[0], nee.BIN)
	nee.packInt(msg[1], nee.BIN)
	nee.packInt(msg[2], nee.BIN)
	nee.packInt(msg[3], nee.BIN)
	nee.packInt(msg[4], nee.BIN)
	nee.packInt(msg[5], nee.BIN)
	nee.packDouble(msg[s], nee.BIN)
	nee.packDouble(msg[7], nee.BIN)
	nee.packDouble(msg[8], nee.BIN)
	nee.packDouble(msg[9], nee.BIN)
	nee.packDouble(msg[10], nee.BIN)
	nee.packDouble(msg[11], nee.BIN)
	nee.packDouble(msg[12], nee.BIN)
	nee.packInt(msg[13], nee.BIN)
	nee.packIntArray(msg[14], nee.BIN)
	nee.packString(msg[15], nee.BIN)
	"""

	# send the message to the SRY receiver fractals.cpp for display
	if nee.send(receiverId) == -1:
		print("send draw error")

def submit():
	# get the name of the bitmap file for saving the current screen image
	name = nameVar.get()
	# reset the stringVar
	nameVar.set("test")
	
	# load save message for the receiver
	# note that msg elements 1-14 are dummy values; only 0 and 15 are used
	nee.packMsg(nee.BIN, "iiiiiidddddddiIs", actions['SAVE'], 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, len(cIndex1), array("I", cIndex1), name)
	
	# send save message to the receiver
	if nee.send(receiverId) == -1:
		print("send save error")

	# remove save() Frame etc. and put up list of fractals for next round
	for x in root.grid_slaves():
		x.grid_remove() 
	drawList(fractalList, select1)

def save():
	# clean up
	for x in root.grid_slaves():
		x.grid_remove() 

	# put up a frame as parent for Label(), Entry() and Button()
	frame = Frame(root)
	frame.grid()

	# retrieve a file name from the user
	global nameVar
	nameVar = StringVar(master=frame, value="test")

	# create a label for file name
	nameLabel = Label(frame, text='File Name', font=('calibre',10,'bold'))
	nameLabel.grid(row = 0,column = 0)
	
	# create and entry for file name 
	nameEntry = Entry(frame, textvariable=nameVar, font=('calibre',10,'normal'))
	nameEntry.grid(row = 0, column = 1)
	nameEntry.focus()
	
	# create a button that will call the submit function
	subBtn = Button(frame, text='Submit', command=submit)
	subBtn.grid(row = 2, column = 1)
	
	# create a button that will call the back1 function
	backBtn=Button(frame, text='Back')
	backBtn.bind('<Button-1>', back1)
	backBtn.grid(row = 2, column = 0)

def play():
	# load and send a play message to the receiver
	nee.packMsg(nee.BIN, "i", actions['PLAY'])
	if nee.send(receiverId) == -1:
		print("send play error")

def clear():
	# load and send a clear the screen message to the receiver
	nee.packMsg(nee.BIN, "i", actions['CLEAR'])
	if nee.send(receiverId) == -1:
		print("send clear error-")

def quit():
	# load and send a 'time to quit' message to the receiver
	nee.packMsg(nee.BIN, "i", actions['QUIT'])
	nee.send(receiverId)
	sys.exit()

# operational start of the program *********************************************

# gui Listbox tuples
fractalList = ('Mandelbrot', 'Julia', 'Trig', 'Exponential', 'Legendre', 
	'Laguerre', 'Dragon', 'Tchebychev', 'Barnsley', 'Sierpinski', 'Phoenix',
	'Save', 'Play', 'Clear', 'Quit')
trigList = ('Cos', 'Sin', 'Cosh', 'Sinh', 'Exp', 'Back', 'Quit')
actionList = ('Default', 'Modify Fractal', 'Modify Colours', 'Back', 'Quit')

# dictionaries of message tokens for the receiver - what to do and with whom
actions = {'DRAW':4, 'SAVE':3, 'PLAY':2, 'CLEAR':1, 'QUIT':0}
pFractals = {'MANDELBROT':0, 'JULIA':1, 'TRIG':2, 'LEGENDRE':3, 'LAGUERRE':4, 
	'DRAGON':5, 'TCHEBYCHEV':6, 'BARNSLEY':7, 'SIERPINSKI':8, 'PHOENIX':9}
sFractals = {'COS':0, 'SIN':1, 'COSH':2, 'SINH':3, 'EXP':4, 'NA':5}

"""
fractal portrayals can differ greatly by extending the number of colours 
available and/or rearranging the colour index order
"""
# default lists of fractal colour indices
cIndex1 = [13,1,2,3,4,5,6,7,8,9,10,11,12,0,14,15]
#cIndex1 = [15,14,0,12,11,10,9,8,7,6,5,4,3,2,1,13]
cIndex2 = [13,8,2,3,4,5,6]

""" message structure expected by the C++ receiver
ACTION_TYPE actionType;
PRIMARY_FRACTAL_TYPE primaryFractalType;
SECONDARY_FRACTAL_TYPE secondaryFractalType;
unsigned maxColumns;
unsigned maxRows;
unsigned maxIters;
double divergence;
double XMax;
double YMax;
double XMin;
double YMin;
double P;
double Q;
unsigned maxColours;
unsigned colourIndex[MAX_COLOURS];
char name[80];
"""

# default fractal message values as tuples
m0 = (actions['DRAW'], pFractals['MANDELBROT'], sFractals['NA'], 1000, 650, 
	512, 4.0, 1.2, 1.2,	-2.0, -1.2, -2.0, 1.0, len(cIndex1),
	array("I", cIndex1), "")
m1 = (actions['DRAW'], pFractals['JULIA'], sFractals['NA'], 1000, 800, 96,
	4.0, 1.8, 1.2, -1.8, -1.2, -0.743036, 0.113467, len(cIndex2),
	array("I", cIndex2), "")
m2 = (actions['DRAW'], pFractals['TRIG'], sFractals['COS'], 1000, 800, 64, 
	1000.0, 5.0, 8.0, -7.0, -8.0, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m3 = (actions['DRAW'], pFractals['TRIG'], sFractals['SIN'], 1000, 800, 64, 
	1000.0, 5.0, 8.0, -7.0, -8.0, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m4 = (actions['DRAW'], pFractals['TRIG'], sFractals['COSH'], 1000, 800, 64, 
	1000.0, 5.0, 8.0, -7.0, -8.0, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m5 = (actions['DRAW'], pFractals['TRIG'], sFractals['SINH'], 1000, 800, 64, 
	1000.0, 5.0, 8.0, -7.0, -8.0, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m6 = (actions['DRAW'], pFractals['TRIG'], sFractals['EXP'], 1000, 800, 64, 
	1000.0, 2.0, 1.0, 0.5, -1.0, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m7 = (actions['DRAW'], pFractals['LEGENDRE'], sFractals['NA'], 1200, 700, 64, 
	1000.0, 0.6, 0.4, -0.6, -0.4, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m8 = (actions['DRAW'], pFractals['LAGUERRE'], sFractals['NA'], 1200, 700, 64, 
	1000.0, 4.0, 1.4, 0.25, -1.4, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m9 = (actions['DRAW'], pFractals['DRAGON'], sFractals['NA'], 1200, 700, 256, 
	4.0, 1.4, 0.8, -0.4, -0.8, 1.646009, 0.967049, len(cIndex1),
	array("I", cIndex1), "")
m10 = (actions['DRAW'], pFractals['TCHEBYCHEV'], sFractals['NA'], 1200, 700, 
	64,	100000.0, 1.6, 0.75, -1.6, -0.75, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m11 = (actions['DRAW'], pFractals['BARNSLEY'], sFractals['NA'], 1300, 700, 64, 
	4.0, 1.7, 1.03,	-2.1, -1.03, 1.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m12 = (actions['DRAW'], pFractals['SIERPINSKI'], sFractals['NA'], 900, 700, 64, 
	64.0, 1.3, 1.22, -0.5, -0.22, 0.0, 0.0, len(cIndex1),
	array("I", cIndex1), "")
m13 = (actions['DRAW'], pFractals['PHOENIX'], sFractals['NA'], 1000, 650, 64, 
	4.0, 1.5, 1.2, -1.5, -0.7, 0.56667, -0.5, len(cIndex1),
	array("I", cIndex1), "")

# tuple of default message tuples - useful in loops
msgTup = (m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13)

# message to be sent - a list so it can be modified, msg has len(m?) elements
global msg
msg = [None] * len(m0)

# make a sender instance of sry
nee = sim.SRY("fractalSender", 1024)

# name locate the local sry receiver program
receiverId = nee.locate("", "FractalReceiver", 1024, nee.SIM_LOCAL)
if receiverId == -1:
	print("name locate error")
	sys.exit()

# initialize Tk for graphics
root = Tk()

# bring up the list box of possible fractals to display
drawList(fractalList, select1)

# handle user input
root.mainloop()
