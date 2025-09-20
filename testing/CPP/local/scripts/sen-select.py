"""
FILE:			sen-select.py

DATE:			February 4, 2025

DESCRIPTION:	Run multiple CPP sender and selector programs for testing purposes.

AUTHOR:			John Collins
"""

import sys, os, time


numArgs = len(sys.argv)
if numArgs != 2:
    print("Incorrect command line: python3 sen-select.py #\n")
    print("where # is the number of concurrent sender and selector programs.\n")
    sys.exit()

totalPrograms = int(sys.argv[1])

for x in range(1, totalPrograms + 1):
    cmd = "../bin/selector sel" + str(x) + " &"
    os.system(cmd)

time.sleep(5)

for x in range(1, totalPrograms + 1):
    cmd = "../bin/sender sen" + str(x) + " sel" + str(x) + " 1000 &"
    os.system(cmd)

time.sleep(10)

for x in range(1, totalPrograms + 1):
    cmd = "../../../../utilities/bin/simSlay sel" + str(x)
    os.system(cmd)
