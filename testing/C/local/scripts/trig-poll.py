"""
FILE:			trig-poll.py

DATE:			January 24, 2025

DESCRIPTION:	Run multiple C trigger and poller programs for testing purposes.

AUTHOR:			John Collins
"""

import sys, os, time

numArgs = len(sys.argv)
if numArgs != 2:
    print("Incorrect command line: python3 trig-poll.py #\n")
    print("where # is the number of concurrent trigger and poller programs.\n")
    sys.exit()

programLimit = int(sys.argv[1])

#print("num of progs=", programLimit)


for x in range(1, programLimit + 1):
    cmd = "../bin/poller pol" + str(x) + " &"
   # print("cmd1=", cmd)
    os.system(cmd)

time.sleep(5)

for x in range(1, programLimit + 1):
    cmd = "../bin/trigger" + " trig" + str(x) + " pol" + str(x) + " 1000 &"
    #print("cmd2=", cmd)
    os.system(cmd)

time.sleep(10)

for x in range(1, programLimit + 1):
    cmd = "../../../../utilities/bin/simSlay pol" + str(x)
    #print("cmd3=", cmd)
    os.system(cmd)
