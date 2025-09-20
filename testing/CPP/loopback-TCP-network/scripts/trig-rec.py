"""
FILE:			trig-rec.py

DATE:			February 4, 2025

DESCRIPTION:	Run multiple C++ trigger and receiver programs for testing purposes.

AUTHOR:			John Collins
"""

import sys, os, time

numArgs = len(sys.argv)
if numArgs != 2:
    print("Incorrect command line: python3 trig-rec.py #\n")
    print("where # is the number of concurrent trigger and receiver programs.\n")
    sys.exit()

totalPrograms = int(sys.argv[1])

for x in range(1, totalPrograms + 1):
    cmd = "../bin/receiver rec" + str(x) + " &"
    os.system(cmd)

time.sleep(5)

for x in range(1, totalPrograms + 1):
    cmd = "../bin/trigger" + " trig" + str(x) + " rec" + str(x) + " 1000 &"
    os.system(cmd)
   # need a moment here for TCP_surrogate_R to fork TCP_surrogate_r
    time.sleep(0.1)

time.sleep(10)

for x in range(1, totalPrograms + 1):
    cmd = "../../../../utilities/bin/simSlay rec" + str(x)
    os.system(cmd)
