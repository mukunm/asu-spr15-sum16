__author__ = 'PawanMahalle'

import numpy as np
import Task4ShannonFano

#########################################################################################
#                                   Main                                                #
#########################################################################################

filename = raw_input("Enter TPQ or SPQ file name: ")

print "Select the compression strategy:"
print "Press 1 for No compression"
print "Press 2 for ShannonFano algorithm"
print "Press 3 for LZW algorithm"
print "Press 4 for Arithmatic algorithm"
option = input("Enter choice: ")

if filename.endswith("tpq"):
    targetFilename = filename.replace(".tpq","_"+str(option)+".tpv")
elif filename.endswith("spq"):
    targetFilename = filename.replace(".spq","_"+str(option)+".spv")

if option == 1:
    with open(filename) as f:
        compressedDataList = []
        for line in f: # read rest of lines
            compressedDataList.append([int(x) for x in line.split()])
        np.savetxt(targetFilename, compressedDataList, fmt='%i', delimiter='\t')

elif option == 2:
    Task4ShannonFano.compress(filename)

print "Compressed data from " + filename + " to " + targetFilename