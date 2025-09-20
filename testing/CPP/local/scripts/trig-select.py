"""
FILE:			trig-select.py

DATE:			February 4, 2025

DESCRIPTION:	Run multiple CPP trigger and selector programs for testing purposes.

AUTHOR:			John Collins
"""

import sys, os, time

numArgs = len(sys.argv)
if numArgs != 2:
    print("Incorrect command line: python3 trig-select.py #\n")
    print("where # is the number of concurrent trigger and selector programs.\n")
    sys.exit()

totalPrograms = int(sys.argv[1])

for x in range(1, totalPrograms + 1):
    cmd = "../bin/selector sel" + str(x) + " &"
    os.system(cmd)

time.sleep(5)

for x in range(1, totalPrograms + 1):
    cmd = "../bin/trigger trig" + str(x) + " sel" + str(x) + " 1000 &"
    os.system(cmd)

time.sleep(10)

for x in range(1, totalPrograms + 1):
    cmd = "../../../../utilities/bin/simSlay sel" + str(x)
    os.system(cmd)
