#Author(s) - Mukund Manikarnike

import cv2 as cv2
import numpy as np
from numpy import linalg
from numpy import matrix
from math import *
import sys

totalAbsolutePredictionError = 0
printing = 0

# This function encodes using predicor A
def encodeSpatiallyOption2(regionOfInterest):
    m = int(len(regionOfInterest))
    n = int(len(regionOfInterest[0]))
    encodedResult = np.ndarray((m, n))
    global totalAbsolutePredictionError

    for i in range(0, m):
        encodedResult[i][0] = int(regionOfInterest[i][0]);

    for i in range(0, m):
        for j in range(1, n):
            encodedResult[i][j] = int(regionOfInterest[i][j]) - int(regionOfInterest[i][j - 1])
            totalAbsolutePredictionError += abs(encodedResult[i][j])

    return encodedResult

# This function decodes using predicor A
def decodeSpatiallyOption2(regionOfInterest):
    m = int(len(regionOfInterest))
    n = int(len(regionOfInterest[0]))
    decodedResult = np.ndarray((m, n))

    for i in range(0, m):
        decodedResult[i][0] = int(regionOfInterest[i][0]);

    for i in range(0, m):
        for j in range(1, n):
            decodedResult[i][j] = int(regionOfInterest[i][j]) + int(decodedResult[i][j - 1])

    return decodedResult

# This function encodes using predicor B
def encodeSpatiallyOption3(regionOfInterest):
    m = int(len(regionOfInterest))
    n = int(len(regionOfInterest[0]))
    encodedResult = np.ndarray((m, n))
    global totalAbsolutePredictionError

    for j in range(0, n):
        encodedResult[0][j] = int(regionOfInterest[0][j])

    for i in range(1, m):
        for j in range(0, n):
            encodedResult[i][j] = int(regionOfInterest[i][j]) - int(regionOfInterest[i - 1][j])
            totalAbsolutePredictionError += abs(encodedResult[i][j])

    return encodedResult

# This function decodes using predicor B
def decodeSpatiallyOption3(regionOfInterest):
    m = int(len(regionOfInterest))
    n = int(len(regionOfInterest[0]))
    decodedResult = np.ndarray((m, n))

    for j in range(0, n):
        decodedResult[0][j] = int(regionOfInterest[0][j])

    for i in range(1, m):
        for j in range(0, n):
            if i % 10 == 0:
                decodedResult[i][j] = int(regionOfInterest[i][j])
            else:
                decodedResult[i][j] = int(regionOfInterest[i][j]) + int(decodedResult[i - 1][j])

    return decodedResult


# This function encodes using predicor C
def encodeSpatiallyOption4(regionOfInterest):
    m = len(regionOfInterest)
    n = len(regionOfInterest[0])
    encodedResult = np.ndarray((m, n))
    global totalAbsolutePredictionError

    for i in range(0, m):
        encodedResult[i][0] = int(regionOfInterest[i][0]);

    for j in range(0, n):
        encodedResult[0][j] = int(regionOfInterest[0][j])

    for i in range(1, m):
        for j in range(1, n):
            encodedResult[i][j] = int(regionOfInterest[i][j]) - int(regionOfInterest[i - 1][j - 1])
            totalAbsolutePredictionError += abs(encodedResult[i][j])

    return encodedResult

# This function encodes using predicor C
def decodeSpatiallyOption4(regionOfInterest):
    m = len(regionOfInterest)
    n = len(regionOfInterest[0])
    decodedResult = np.ndarray((m, n))

    for i in range(0, m):
        decodedResult[i][0] = int(regionOfInterest[i][0]);

    for j in range(0, n):
        decodedResult[0][j] = int(regionOfInterest[0][j])

    for i in range(1, m):
        for j in range(1, n):
            if i % 10 == 0:
                decodedResult[i][j] = int(regionOfInterest[i][j])
            else:
                decodedResult[i][j] = int(regionOfInterest[i][j]) + int(decodedResult[i - 1][j - 1])

    return decodedResult

# This function encodes using predictor alpha1 * A + alpha2 * B + alpha3 * C
def encodeSpatiallyOption5(regionOfInterest):
    m = int(len(regionOfInterest))
    n = int(len(regionOfInterest[0]))
    encodedResult = np.ndarray((m, n))
    intermediateEncoding = np.ndarray((m, n))
    global totalAbsolutePredictionError
    global printing

    for i in range(0, m):
        encodedResult[i][0] = float(regionOfInterest[i][0]);
        intermediateEncoding[i][0] = float(regionOfInterest[i][0]);

    for j in range(0, n):
        encodedResult[0][j] = float(regionOfInterest[0][j])
        intermediateEncoding[0][j] = float(regionOfInterest[0][j])

    for i in range(1, m):
        for j in range(1, n):
            if i < 2 or j < 2:
                #Encode using alphas as 1/3
                alpha = float(1)/float(3)
                intermediateEncoding[i][j] = float(alpha * (float(regionOfInterest[i][j - 1]) + float(regionOfInterest[i - 1][j]) + float(regionOfInterest[i - 1][j - 1])))
            else:
                #Create a system of linear equations using the previously predicted values.
                linearEquationLHS = matrix([[float(regionOfInterest[i - 1][j - 1]), float(regionOfInterest[i - 2][j]), float(regionOfInterest[i - 2][j - 1])],
                                            [float(regionOfInterest[i - 1][j - 2]), float(regionOfInterest[i - 2][j - 1]), float(regionOfInterest[i - 2][j - 2])],
                                            [float(regionOfInterest[i][j - 2]), float(regionOfInterest[i - 1][j - 1]), float(regionOfInterest[i - 1][j - 2])]])
                linearEquationRHS = matrix([[float(regionOfInterest[i - 1][j])], [float(regionOfInterest[i - 1][j - 1])], [float(regionOfInterest[i][j - 1])]])

                try:
                    #Find alphas using previously calculated alphas and then encode the current value
                    alphas = linalg.lstsq(linearEquationLHS, linearEquationRHS)[0]
                    alpha1 = alphas[0]
                    alpha2 = alphas[1]
                    alpha3 = alphas[2]

                    #Encode using the alphas
                    intermediateEncoding[i][j] = alpha1 * float(regionOfInterest[i][j - 1]) + alpha2 * float(regionOfInterest[i - 1][j]) + alpha3 * float(regionOfInterest[i - 1][j - 1])

                    # if the predicted value exceeds the range, encode it using default values
                    if intermediateEncoding[i][j] > 255 or intermediateEncoding[i][j] < 0:
                        #Encode using alphas as 1/3
                        alpha = float(1)/float(3)
                        intermediateEncoding[i][j] = alpha * (float(regionOfInterest[i][j - 1]) + float(regionOfInterest[i - 1][j]) + float(regionOfInterest[i - 1][j - 1]))

                except np.linalg.linalg.LinAlgError, e:
                    #Encode using alphas as 1/3
                    alpha = float(1)/float(3)
                    intermediateEncoding[i][j] = alpha * (float(regionOfInterest[i][j - 1]) + float(regionOfInterest[i - 1][j]) + float(regionOfInterest[i - 1][j - 1]))

    #Compute the prediction error and return the result.
    for i in range(1, m):
        for j in range(1, n):
            encodingError = float(float(intermediateEncoding[i][j]) - int(intermediateEncoding[i][j]))
            encodedResult[i][j] = float(float(regionOfInterest[i][j]) - float(intermediateEncoding[i][j]) + encodingError)
            totalAbsolutePredictionError += abs(encodedResult[i][j])

    return encodedResult

# This function decodes using predictor alpha1 * A + alpha2 * B + alpha3 * C
def decodeSpatiallyOption5(regionOfInterest):
    m = int(len(regionOfInterest))
    n = int(len(regionOfInterest[0]))
    printing = 1
    decodedResult = np.ndarray((m, n))
    intermediateDecoding = np.ndarray((m, n))

    for i in range(0, m):
        decodedResult[i][0] = float(regionOfInterest[i][0]);
        intermediateDecoding[i][0] = float(regionOfInterest[i][0]);

    for j in range(0, n):
        decodedResult[0][j] = float(regionOfInterest[0][j])
        intermediateDecoding[0][j] = float(regionOfInterest[0][j])

    for i in range(1, m):
        for j in range(1, n):
            if (((i - 1) % 10) == 0) or (((j - 1) % 10) == 0):
                #Encode using alphas as 1/3
                alpha = float(1)/float(3)
                intermediateDecoding[i][j] = float(alpha * (float(decodedResult[i][j - 1]) + float(decodedResult[i - 1][j]) + float(decodedResult[i - 1][j - 1])))
            else:
                #Create a system of linear equations using the previously predicted values.
                linearEquationLHS = matrix([[float(decodedResult[i - 1][j - 1]), float(decodedResult[i - 2][j]), float(decodedResult[i - 2][j - 1])],
                                            [float(decodedResult[i - 1][j - 2]), float(decodedResult[i - 2][j - 1]), float(decodedResult[i - 2][j - 2])],
                                            [float(decodedResult[i][j - 2]), float(decodedResult[i - 1][j - 1]), float(decodedResult[i - 1][j - 2])]])

                linearEquationRHS = matrix([[float(decodedResult[i - 1][j])], [float(decodedResult[i - 1][j - 1])], [float(decodedResult[i][j - 1])]])

                try:
                    #Find alphas using previously calculated alphas and then decode the current value
                    alphas = linalg.lstsq(linearEquationLHS, linearEquationRHS)[0]
                    alpha1 = alphas[0]
                    alpha2 = alphas[1]
                    alpha3 = alphas[2]

                    #Decode using the alphas
                    intermediateDecoding[i][j] = alpha1 * float(decodedResult[i][j - 1]) + alpha2 * float(decodedResult[i - 1][j]) + alpha3 * float(decodedResult[i - 1][j - 1])

                    # if the predicted value exceeds the range, encode it using default values
                    if intermediateDecoding[i][j] > 255 or intermediateDecoding[i][j] < 0:
                        #Encode using alphas as 1/3
                        alpha = float(1)/float(3)
                        intermediateDecoding[i][j] = alpha * (float(decodedResult[i][j - 1]) + float(decodedResult[i - 1][j]) + float(decodedResult[i - 1][j - 1]))

                except np.linalg.linalg.LinAlgError, e:
                    #Encode using alphas as 1/3
                    alpha = float(1)/float(3)
                    intermediateDecoding[i][j] = alpha * (float(decodedResult[i][j - 1]) + float(decodedResult[i - 1][j]) + float(decodedResult[i - 1][j - 1]))

            if i % 10 == 0:
                decodedResult[i][j] = int(regionOfInterest[i][j])
            else:
                encodingError = float(float(intermediateDecoding[i][j]) - int(intermediateDecoding[i][j]))
                decodedResult[i][j] = float(float(regionOfInterest[i][j]) + float(intermediateDecoding[i][j]) - encodingError)

    return decodedResult

# This function encodes spatially by calling the appropriate function based
# on the input option chosen by the user.
def encodeSpatially(regionOfInterest, option):
    # No encoding for option 1. The passed array is returned.
    if option == 1:
        return regionOfInterest
    elif option == 2:
        return encodeSpatiallyOption2(regionOfInterest)
    elif option == 3:
        return encodeSpatiallyOption3(regionOfInterest)
    elif option == 4:
        return encodeSpatiallyOption4(regionOfInterest)
    elif option == 5:
        return encodeSpatiallyOption5(regionOfInterest)

# This function extracts the region of interest given the entire region,
# the x and y co-ordinates of the region of interest.
def extractRegionOfInterest(array, xCoordinate, yCoordinate, n = 10):
    return array[xCoordinate: (xCoordinate + n), yCoordinate: (yCoordinate + n)]

#flattens the provided decoded result
def flatten(decodedResult):
    m = int(len(decodedResult))
    n = int(len(decodedResult[0]))
    flattenedResult = [[0 for col in range(n * 10)] for row in range(m / 10)]

    p = 0
    q = 0
    i = 0
    while i < m:
        j = 0
        if i != 0 and (i % 10) == 0:
            p += 1
            q = 0
        while j < n:
            #print "p = " + str(p) + " q = " + str(q) + " i = " + str(i) + " j = " + str(j)

            flattenedResult[p][q] = decodedResult[i][j]
            q += 1

            j += 1
        i += 1

    return flattenedResult

# This function decodes the pixel values in the input in a spatial manner
def decodeSpatially(fileName, option):
    regionOfInterest = extractSpatiallyEncodedData(fileName)

    # No encoding for option 1. The passed array is returned.
    if option == 1:
        decodedResult = regionOfInterest
        return flatten(decodedResult)
    elif option == 2:
        decodedResult = decodeSpatiallyOption2(regionOfInterest)
        return flatten(decodedResult)
    elif option == 3:
        decodedResult = decodeSpatiallyOption3(regionOfInterest)
        return flatten(decodedResult)
    elif option == 4:
        decodedResult = decodeSpatiallyOption4(regionOfInterest)
        return flatten(decodedResult)
    elif option == 5:
        decodedResult = decodeSpatiallyOption5(regionOfInterest)
        return flatten(decodedResult)

# This function extracts the complete region of interest from the file provided
# and returns
def extractSpatiallyEncodedData(fileName):
    numLines = 0

    # Opens the encoded file and counts the number lines
    with open(fileName, 'r') as encodedFile:
        for line in encodedFile:
            numLines += 1

    # Create and populate the region of interest array with each pixel value.
    regionOfInterest = np.ndarray((numLines, 10))

    with open(fileName, 'r') as encodedFile:
        lineCount = 0
        for line in encodedFile:
            pixelValues = line.split('\t')
            for i in range(0, 10):
                regionOfInterest[lineCount, i] = int(pixelValues[i])
            lineCount += 1

    return regionOfInterest

#Read the videoFilesPath
videoFilesPath = raw_input("Enter the Path where the videos are stored: ")

#Read the video file name
videoFileFullName = raw_input("Enter the name of the file to be processed: ")
videoFileNameSplit = videoFileFullName.split(".")
videoFileNumber = int(videoFileNameSplit[0])

#Read the X and Y Co-ordinates
xCoordinate = int(raw_input("Enter the top-left X coordinate:"))
yCoordinate = int(raw_input("Enter the top-left Y coordinate:"))

print "Select any one of the following:"
print "Press 1 for PC Option 1"
print "Press 2 for PC Option 2"
print "Press 3 for PC Option 3"
print "Press 4 for PC Option 4"
print "Press 5 for PC Option 5"
option = int(raw_input("Enter choice:"))

# Open the video capture
vidcap = cv2.VideoCapture(videoFilesPath + "\\" + videoFileFullName)

# Read and display the number of frames in the video for the user's
# benefit.
totalNumFrames = vidcap.get(7)
print "The video contains " + str(totalNumFrames) + " frames.\n"

# Read totalNumFrames number of times to get the nth frame
i = 0
while i < totalNumFrames:
    success, image = vidcap.read()

    # Convert the image that has been read to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    # Extract xSpan x ySpan number of pixels starting from the xCoordinate and
    # yCoordinate
    regionOfInterest = extractRegionOfInterest(gray, xCoordinate, yCoordinate)

    #MDB
    if i == 2:
        printing = 1
    else:
        printing = 0

    # Spatially encode the region of interest
    spatiallyEncodedResult = encodeSpatially(regionOfInterest, option)

    # Dump the output to .spc file.
    if i == 0:
        fileHandle = open(str(videoFileNumber) + "_" + str(option) + ".spc", "w")
        p = len(spatiallyEncodedResult)
        q = len(spatiallyEncodedResult[0])

        for m in range(0, p):
            for n in range(0, q):
                fileHandle.write(str(int(spatiallyEncodedResult[m][n])) + "\t")
            fileHandle.write("\n")
        fileHandle.close()
    else:
        fileHandle = open(str(videoFileNumber) + "_" + str(option) + ".spc", "a")
        p = len(spatiallyEncodedResult)
        q = len(spatiallyEncodedResult[0])

        for m in range(0, p):
            for n in range(0, q):
                fileHandle.write(str(int(spatiallyEncodedResult[m][n])) + "\t")
            fileHandle.write("\n")

        fileHandle.close()

    # Increment counter
    i += 1

print "Info: Total absolute prediction error is " + str(totalAbsolutePredictionError)
print "Encoded Output is written to " + str(videoFileNumber) + "_" + str(option) + ".spc"
# print "---------------------------------"
# print "Decoding..."
# encodedFileName = str(videoFileNumber) + "_" + str(option) + ".spc"
# decodedResult = decodeSpatially(encodedFileName, option)
#
# fileHandle = open(str(videoFileNumber) + "_" + str(option) + ".spc.decoded", "w")
# p = len(decodedResult)
# q = len(decodedResult[0])
#
# for m in range(0, p):
#     for n in range(0, q):
#         fileHandle.write(str(decodedResult[m][n]) + "\t")
#     fileHandle.write("\n")
# fileHandle.close()
#
# print "Decoded Output is written to " + str(videoFileNumber) + "_" + str(option) + ".spc.decoded"

# Release the video capture. We no longer need the video. All images
# required have been extracted from the video.

vidcap.release()
