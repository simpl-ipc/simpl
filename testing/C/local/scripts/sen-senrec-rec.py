"""
FILE:			sen-senrec-rec.py

DATE:			January 24, 2025

DESCRIPTION:	Run multiple C sender and receiver programs for testing purposes.

AUTHOR:			John Collins
"""

import sys, os, time

numArgs = len(sys.argv)
if numArgs != 2:
    print("Incorrect command line: python3 sen-senrec-rec.py #\n")
    print("# is the number of concurrent sender, sender/receiver and receiver programs.\n")
    sys.exit()

totalPrograms = int(sys.argv[1])

#print("num of progs=", totalPrograms)

for x in range(1, totalPrograms + 1):
    cmd = "../bin/receiver rec" + str(x) + " &"
    os.system(cmd)

time.sleep(5)

for x in range(1, totalPrograms + 1):
    cmd = "../bin/senrec senrec" + str(x) + " rec" + str(x) + " &"
    os.system(cmd)

time.sleep(5)

for x in range(1, totalPrograms + 1):
    cmd = "../bin/sender sen" + str(x) + " senrec" + str(x) + " 1000 &"
    os.system(cmd)

time.sleep(10)

for x in range(1, totalPrograms + 1):
    cmd = "../../../../utilities/bin/simSlay rec" + str(x)
    os.system(cmd)
    cmd = "../../../../utilities/bin/simSlay senrec" + str(x)
    os.system(cmd)
