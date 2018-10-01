import cv2 as cv2
import sys
from matplotlib.cbook import todate
import numpy as np
import math

#########################################################################################
#                                   Main                                                #
#########################################################################################

filename = raw_input("Enter TPC or SPC file name: ")

print "Select any one of the following:"
print "Press 1 for No Quantization"
print "Press 2 for 2^m Quantization"
option = input("Enter choice: ")

quantizedList = []

if filename.endswith("tpc"):
    targetFileName = filename.replace(".tpc","_"+str(option)+".tpq")
if filename.endswith("spc"):
    targetFileName = filename.replace(".spc","_"+str(option)+".spq")

print "crunching numbers..."

if option == 1:

    with open(filename) as f:
        for line in f: # read rest of lines
            quantizedList.append([int(x) for x in line.split()])

elif option == 2:

    m = input("Enter the value of m: ")
    with open(filename) as f:
        for line in f: # read rest of lines

            #data structure to store quatized values for all pixels in a frame.
            quantizedArray = []

            #contains unquantized values from tpc or spc file.
            array = [int(x) for x in line.split()]
            for i in range(0, len(array)):
                value = array[i]
                bin = int((256 + value)//(256//pow(2,m-1)))
                interval = 256//pow(2,m-1)
                start = -256 + interval/2
                number = start + bin*interval
                quantizedArray.append(number)
            quantizedList.append(quantizedArray)

print "creating file "+targetFileName+"..."

# store the qunatized output to file with extension tpq or spq.
np.savetxt(targetFileName, quantizedList, fmt='%i', delimiter='\t')
