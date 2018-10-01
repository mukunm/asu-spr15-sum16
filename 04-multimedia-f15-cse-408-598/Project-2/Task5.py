import cv2 as cv2
import sys
import numpy as np
import math
from numpy import linalg
from numpy import matrix
import Task4ShannonFano

#configuration variable borrowed from Task1 configuration
xSpan = 10
ySpan = 10

toVideo = False

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


def decodeData(predictiveCodingStrategy, filename):
	decodedData = []

	if filename.endswith("tpv.decompressed"):

		if predictiveCodingStrategy == 1:
			print "codingStrategy1"
			with open(filename) as f:
				for line in f: # read rest of lines
					decodedData.append([int(x) for x in line.split()])
                np.savetxt(filename+".decoded", decodedData, fmt='%i', delimiter='\t')

		if predictiveCodingStrategy == 2:
			print "codingStrategy2"
			with open(filename) as f:
				arr = []
				isFisrtLine = True
				for line in f: # read rest of lines
					if isFisrtLine:
						arr = [int(x) for x in line.split()]
						isFisrtLine = False
						isSecondLine = True
						decodedData.append(arr)
					elif isSecondLine:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = decodedData[len(decodedData)-1][i] + arr[i]
						decodedData.append(arr)
						isSecondLine = False
						isThirdLine = True
					elif isThirdLine:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = int((decodedData[len(decodedData)-1][i] + decodedData[len(decodedData)-2][i])/2) + arr[i]
						decodedData.append(arr)
                np.savetxt(filename+".decoded", decodedData, fmt='%i', delimiter='\t')

	if predictiveCodingStrategy == 3:
			print "codingStrategy3"
			with open(filename) as f:
				arr = []
				isFisrtLine = True
				isSecondLine = False
				for line in f: # read rest of lines
					if isFisrtLine:
						arr = [int(x) for x in line.split()]
						isFisrtLine = False
						isSecondLine = True
						decodedData.append(arr)
					elif isSecondLine:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = decodedData[len(decodedData)-1][i] + arr[i]
						decodedData.append(arr)
						isSecondLine = False
					else:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = int((decodedData[len(decodedData)-1][i] + decodedData[len(decodedData)-2][i])/2) + arr[i]
						decodedData.append(arr)
				np.savetxt(filename+".decoded", decodedData, fmt='%i', delimiter='\t')

        if predictiveCodingStrategy == 4:
			print "codingStrategy4"
			with open(filename) as f:
				arr = []
				isFisrtLine = True
				isSecondLine = False
				isThirdLine = False
				isForthLine = False
				for line in f: # read rest of lines
					if isFisrtLine:
						arr = [int(x) for x in line.split()]
						isFisrtLine = False
						isSecondLine = True
						decodedData.append(arr)
					elif isSecondLine:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = decodedData[len(decodedData)-1][i] + arr[i]
						decodedData.append(arr)
						isSecondLine = False
						isThirdLine = True
					elif isThirdLine:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = int((decodedData[len(decodedData)-1][i] + decodedData[len(decodedData)-2][i])/2) + arr[i]
						decodedData.append(arr)
						isThirdLine = False
						isForthLine = True
					elif isForthLine:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							arr[i] = int((decodedData[len(decodedData)-1][i] + decodedData[len(decodedData)-2][i])/2) + arr[i]
						decodedData.append(arr)
						isForthLine = False
					else:
						arr = [int(x) for x in line.split()]
						for i in range(0, len(arr)):
							determinant = (decodedData[len(decodedData)-4][i] * decodedData[len(decodedData)-2][i]) - (decodedData[len(decodedData)-3][i] * decodedData[len(decodedData)-3][i])

							if determinant == 0:
								alpha1 = 0.5
								alpha2 = 0.5
							else:
								alpha1 = ((decodedData[len(decodedData)-1][i] * decodedData[len(decodedData)-4][i]) + (decodedData[len(decodedData)-2][i] * decodedData[len(decodedData)-3][i] * -1))*(1.0)/determinant
								alpha2 = ((decodedData[len(decodedData)-1][i] * decodedData[len(decodedData)-3][i] * -1) + (decodedData[len(decodedData)-2][i] * decodedData[len(decodedData)-2][i]))*(1.0)/determinant

							if alpha1 + alpha2 == 0:
								alpha1 = 0.5
								alpha2 = 0.5

							predictedValue = (int((decodedData[len(decodedData)-1][i] * alpha1) + (decodedData[len(decodedData)-2][i] * alpha2)))
							if 0 <= predictedValue and predictedValue <= 255:
								arr[i] = arr[i] + predictedValue
							else:
								arr[i] =  arr[i] + (int((decodedData[len(decodedData)-1][i] * 0.5) + (decodedData[len(decodedData)-2][i] * 0.5)))

						decodedData.append(arr)
				np.savetxt(filename+".decoded", decodedData, fmt='%i', delimiter='\t')

	elif filename.endswith("spv.decompressed"):

		regionOfInterest = extractSpatiallyEncodedData(filename)

		if predictiveCodingStrategy == 1:
			decodedResult = regionOfInterest
			decodedData = flatten(decodedResult)
		elif predictiveCodingStrategy == 2:
			print "codingStrategy2"
			decodedResult = decodeSpatiallyOption2(regionOfInterest)
			decodedData = flatten(decodedResult)
		elif predictiveCodingStrategy == 3:
			print "codingStrategy3"
			decodedResult = decodeSpatiallyOption3(regionOfInterest)
			decodedData = flatten(decodedResult)
		elif predictiveCodingStrategy == 4:
			decodedResult = decodeSpatiallyOption4(regionOfInterest)
			decodedData = flatten(decodedResult)
		elif predictiveCodingStrategy == 5:
			decodedResult = decodeSpatiallyOption5(regionOfInterest)
			decodedData = flatten(decodedResult)

		fileHandle = open(filename+".decoded", "w")
		p = len(decodedData)
		q = len(decodedData[0])

		for m in range(0, p):
			if m != 0:
				fileHandle.write("\n")
			for n in range(0, q):
				fileHandle.write(str(int(decodedData[m][n])) + "\t")

		fileHandle.close()
		return decodedData


def getDecompressedData(compressionStrategy, filename):
	decompressedData = []

	if compressionStrategy == 1:
		print "Data present in file is UNCOMPRESSED"
		with open(filename) as f:
			for line in f: # read rest of lines
				decompressedData.append([int(x) for x in line.split()])
			np.savetxt(filename+".decompressed", decompressedData, fmt='%i', delimiter='\t')
		return filename+".decompressed"

	if compressionStrategy == 2:
		return Task4ShannonFano.decompress(filename)
	if compressionStrategy == 3:
		print "Data present in file is compressed using LZW algorithm"
	if compressionStrategy == 4:
		print "Data present in file is compressed using Arithmatic algorithm"

	return decompressedData

#########################################################################################
#                                   Main                                                #
#########################################################################################

filename = raw_input("Enter the filename: ");

videoNumber = int(filename.split(".")[0].split("_")[0])
predictiveCodingStrategy = int(filename.split(".")[0].split("_")[1])
quantizationScheme = int(filename.split(".")[0].split("_")[2])
compressionStrategy = int(filename.split(".")[0].split("_")[3])

print "Decompressing "+filename+" using compression algorithm " + str(compressionStrategy)
decompressedFile = getDecompressedData(compressionStrategy, filename)
print "Generated decompressed file: "+decompressedFile

print "Decoding "+filename+" which was encoded using encoder " + str(predictiveCodingStrategy)
decodedData = decodeData(predictiveCodingStrategy, filename+".decompressed")
print "Generated decoded file: "+filename+".decompressed.decoded"

print "Creating video from decompressed and decoded data..."

if toVideo:
	outputFile = "compressed.avi"
	image = cv2.imread("reference.jpg")

	reference = cv2.resize(image,(xSpan, ySpan), interpolation = cv2.INTER_NEAREST)

	videoWriter = cv2.VideoWriter(outputFile, cv2.VideoWriter_fourcc(*'MP4V'), 30,
							  (xSpan, ySpan), True)

with open(filename+".decompressed.decoded") as f:
	frame = 0
	for line in f: # read rest of lines
		arr = []
		arr = line.split('\t')
		if toVideo == False:
			reference = np.ndarray((xSpan,ySpan))
		for i in range (0, xSpan*ySpan):
			reference[i/xSpan][i%ySpan] = arr[i]

		if toVideo:
			videoWriter.write(reference)
		else:
			cv2.imwrite("output\\frame" + str(frame) + ".jpg", reference)
		frame +=1

print "done"
if toVideo:
	del videoWriter
