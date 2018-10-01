__author1__ = 'PawanMahalle'
__author2__ = 'MukundManikarnike'
__author3__ = 'RunyuJin'

import numpy
import sys
import operator
import math
'''
This file cotains the SimilarityMatching classes for various features.
'''

'''
This class provides similarity mathcing based on N bin difference histogram values for given video file
'''
class NBinDiffHistogramSimilarityMatcher:
    # This constructor extracts the information from video file
    def __init__(self, videoFilesPath, videoFileName, n, feature):
        if feature == 1:
            absoluteFilename = videoFilesPath+"\\"+videoFileName.split(".")[0] + "_hist_" + str(n) + ".hst"
        if feature == 4:
            absoluteFilename = videoFilesPath+"\\"+videoFileName.split(".")[0] + "_diff_" + str(n) + ".dhc"

        self.videoData = self.readNBinHistogramValuesFromFile(absoluteFilename)

    def readNBinHistogramValuesFromFile(self, filename):

        with open(filename) as f:
            self.frameCount, self.blockRowCount, self.blockColCount, self.grayValue = [int(x) for x in f.readline().split()]
            videoData = numpy.ndarray(shape=(self.frameCount,self.blockRowCount,self.blockColCount,self.grayValue))
            for line in f:
                frameId, blockRowIndex, blockColIndex, grayValue, value = [int(x) for x in line.split()]
                videoData[frameId][blockRowIndex][blockColIndex][grayValue] = value

        return videoData

    def getSimilarFrames(self, frameIndex, limit):

        distance = numpy.zeros(shape=(self.frameCount))
        distance[frameIndex] = sys.maxint
        cummulativeDifference = numpy.zeros(shape=(self.blockRowCount, self.blockColCount, self.grayValue))
        for i in range(frameIndex+1, self.frameCount):
            for r in range(self.blockRowCount):
                for c in range(self.blockColCount):
                    for h in range(self.grayValue):
                        cummulativeDifference[r][c][h] += self.videoData[i][r][c][h];
                        distance[i] += ((self.videoData[frameIndex][r][c][h] - cummulativeDifference[r][c][h])*(self.videoData[frameIndex][r][c][h] - cummulativeDifference[r][c][h]))

        cummulativeDifference = numpy.zeros(shape=(self.blockRowCount, self.blockColCount, self.grayValue))
        for i in range(frameIndex-1, -1, -1):
            for r in range(self.blockRowCount):
                for c in range(self.blockColCount):
                    for h in range(self.grayValue):
                        cummulativeDifference[r][c][h] += self.videoData[i][r][c][h];
                        distance[i] += ((self.videoData[frameIndex][r][c][h] - cummulativeDifference[r][c][h])*(self.videoData[frameIndex][r][c][h] - cummulativeDifference[r][c][h]))

        return numpy.argsort(distance)[0:limit:1]

#This class provides similarity mathcing based on N bin histogram values for given video file
class NBinHistogramSimilarityMatcher:

    # This constructor extracts the information from video file
    def __init__(self, videoFilesPath, videoFileName, n, feature):
        if feature == 1:
            absoluteFilename = videoFilesPath+"\\"+videoFileName.split(".")[0] + "_hist_" + str(n) + ".hst"
        if feature == 4:
            absoluteFilename = videoFilesPath+"\\"+videoFileName.split(".")[0] + "_diff_" + str(n) + ".dhc"

        self.videoData = self.readNBinHistogramValuesFromFile(absoluteFilename)#numpy.ndarray(shape=(self.frameCount, grayFrame.shape[0], grayFrame.shape[1]))


    def getSimilarFrames(self, frameIndex, limit):
        meanSquareError = numpy.zeros(shape=(self.frameCount))

        for i in range (self.frameCount):
            if i == frameIndex:
                meanSquareError[i] = sys.maxint
                continue
            for r in range(self.blockRowCount):
                for c in range(self.blockColCount):
                    for h in range(self.grayValue):
                        meanSquareError[i] += (((self.videoData[i][r][c][h] - self.videoData[frameIndex][r][c][h]) * (self.videoData[i][r][c][h] - self.videoData[frameIndex][r][c][h])))

        return numpy.argsort(meanSquareError)[0:limit:1]

    def readNBinHistogramValuesFromFile(self, filename):

        with open(filename) as f:
            self.frameCount, self.blockRowCount, self.blockColCount, self.grayValue = [int(x) for x in f.readline().split()]
            videoData = numpy.ndarray(shape=(self.frameCount,self.blockRowCount,self.blockColCount,self.grayValue))
            for line in f:
                frameId, blockRowIndex, blockColIndex, grayValue, value = [int(x) for x in line.split()]
                videoData[frameId][blockRowIndex][blockColIndex][grayValue] = value

        return videoData

class BlockLevel2DDWTSimilarityMatcher:

    # This constructor extracts the information from video file
    def __init__(self, videoFilesPath, videoFileName, n):
        absoluteFilename = videoFilesPath + "\\"+ videoFileName.split(".")[0] + "_blockdwt_" + str(n) + ".bwt"
        self.videoData = self.read2DDWTComponentsFromFile(absoluteFilename)

    def computeSimilarityMetric(self, videoFrame):
        avgStdDevList = []

        for i in range(0, self.blockCount):
            averageFrequencies = 0
            for j in range(0, self.numWaveletComponents):
                if j == 0:
                    continue
                else:
                    averageFrequencies += videoFrame[i][j]
            averageFrequencies /= (self.numWaveletComponents - 1)

            stdDevFrequencies = 0
            for j in range(0, self.numWaveletComponents):
                if j != 0:
                    stdDevFrequencies += math.pow((averageFrequencies - videoFrame[i][j]), 2)

            stdDevFrequencies /= (self.numWaveletComponents - 1)
            stdDevFrequencies = math.sqrt(stdDevFrequencies)
            avgStdDevList.append((videoFrame[i][0], stdDevFrequencies))

        return avgStdDevList

    def getSimilarFrames(self, frameIndex, limit):
        frameMetric = (0, 0)
        similarityMetrics = []
        frameMetric = self.computeSimilarityMetric(self.videoData[frameIndex])

        for i in range(0, self.frameCount):
            if i == frameIndex:
                continue
            else:
                similarityMetric = self.computeSimilarityMetric(self.videoData[i])
                avgEuclideanDistance = 0
                sdevEuclideanDistance = 0

                for k in range(0, len(similarityMetric)):
                    avgEuclideanDistance += (frameMetric[k][0] - similarityMetric[k][0]) ** 2
                    sdevEuclideanDistance += (frameMetric[k][1] - similarityMetric[k][1]) ** 2

                comparisonMetric = (i, avgEuclideanDistance, sdevEuclideanDistance)
                similarityMetrics.append(comparisonMetric)

        #Sort the similarity metrics
        similarityMetrics = sorted(similarityMetrics, key = operator.itemgetter(1, 2))

        frameIndices = numpy.zeros(limit)
        for i in range(0, limit):
            frameIndices[i] = similarityMetrics[i][0]

	    print "selected frames: ", frameIndices
        return frameIndices

    def read2DDWTComponentsFromFile(self, filename):
        with open(filename) as f:
            self.frameCount, self.blockCount, self.numWaveletComponents = [int(x) for x in f.readline().split(",")]
            videoData = numpy.ndarray(shape=(self.frameCount, self.blockCount, self.numWaveletComponents))

            f.readline()

            frameIndex = 0
            blockIndex = 0
            waveletComponentIndex = 0

            for line in f:
                frameId, blockCoord, waveletCompId, value = [x for x in line.rstrip('\r\n').split(",")]
                frameId = int(frameId)
                blockXCoordinate = int(blockCoord.split("x")[0])
                blockYCoordinate = int(blockCoord.split("x")[1])
                waveletCompId = int(waveletCompId)
                value = float(str(value))

                videoData[frameIndex][blockIndex][waveletComponentIndex] = value

                waveletComponentIndex += 1
                if waveletComponentIndex == self.numWaveletComponents:
                    blockIndex += 1
                    waveletComponentIndex = 0

                if blockIndex == self.blockCount:
                    frameIndex += 1
                    blockIndex = 0

        return videoData

class FrameLevel2DDWTSimilarityMatcher:

    # This constructor extracts the information from video file
    def __init__(self, videoFilesPath, videoFileName, n):
        absoluteFilename = videoFilesPath+"\\"+videoFileName.split(".")[0] + "_framedwt_" + str(n) + ".fwt"
        self.videoData = self.read2DDWTComponentsFromFile(absoluteFilename)
    def computeSimilarityMetric(self, videoFrame):

        average = videoFrame[0]
        averageFrequencies = 0

        for j in range(1, self.numWaveletComponents):
            averageFrequencies += videoFrame[j]
        averageFrequencies /= (self.numWaveletComponents - 1)

        stdDevFrequencies = 0
        for j in range(1, self.numWaveletComponents):
            stdDevFrequencies += math.pow((averageFrequencies - videoFrame[j]), 2)
        stdDevFrequencies /= (self.numWaveletComponents - 1)
        stdDevFrequencies = math.sqrt(stdDevFrequencies)

        return (average, stdDevFrequencies)

    def getSimilarFrames(self, frameIndex, limit):
        frameMetric = (0, 0)
        similarityMetrics = []
        frameMetric = self.computeSimilarityMetric(self.videoData[frameIndex])

        for i in range(0, self.frameCount):
            if i == frameIndex:
                continue
            else:
                similarityMetric = self.computeSimilarityMetric(self.videoData[i])
                comparisonMetric = (i, (frameMetric[0] - similarityMetric[0]) ** 2, (frameMetric[1] - similarityMetric[1]) ** 2)
                similarityMetrics.append(comparisonMetric)

        #Sort the similarity metrics
        similarityMetrics = sorted(similarityMetrics, key = operator.itemgetter(1, 2))

        frameIndices = numpy.zeros(limit)
        for i in range(0, limit):
            frameIndices[i] = similarityMetrics[i][0]

	    print "selected frames: ", frameIndices
        return frameIndices

    def read2DDWTComponentsFromFile(self, filename):
        with open(filename) as f:
            self.frameCount, self.numWaveletComponents = [int(x) for x in f.readline().split(",")]
            videoData = numpy.ndarray(shape=(self.frameCount, self.numWaveletComponents))

            f.readline()

            frameIndex = 0
            waveletComponentIndex = 0

            for line in f:
                frameId, waveletCompId, value = [x for x in line.rstrip('\r\n').split(",")]
                frameId = int(frameId)
                waveletCompId = int(waveletCompId)
                value = float(str(value))

                videoData[frameIndex][waveletComponentIndex] = value

                waveletComponentIndex += 1
                if waveletComponentIndex == self.numWaveletComponents:
                    frameIndex += 1
                    waveletComponentIndex = 0

        return videoData

class BlockLevel2DDCTSimilarityMatcher:

    # This constructor extracts the information from video file
    def __init__(self, videoFilesPath, videoFileName, n):
        absoluteFilename = videoFilesPath + "\\" + videoFileName.split(".")[0] + "_blockdct_" + str(n) + ".bct"
        self.videoData = self.read2DDCTComponentsFromFile(absoluteFilename)

    def computeSimilarityMetric(self, videoFrame):
        avgStdDevList = []

        for i in range(0, self.blockCount):
            averageFrequencies = 0
            for j in range(0, self.numFreqComponents):
                if j == 0:
                    continue
                else:
                    averageFrequencies += videoFrame[i][j]
            averageFrequencies /= (self.numFreqComponents - 1)

            stdDevFrequencies = 0
            for j in range(0, self.numFreqComponents):
                if j != 0:
                    stdDevFrequencies += math.pow((averageFrequencies - videoFrame[i][j]), 2)

            stdDevFrequencies /= (self.numFreqComponents - 1)
            stdDevFrequencies = math.sqrt(stdDevFrequencies)
            avgStdDevList.append((videoFrame[i][0], stdDevFrequencies))

        return avgStdDevList

    def getSimilarFrames(self, frameIndex, limit):
        frameMetric = (0, 0)
        similarityMetrics = []
        frameMetric = self.computeSimilarityMetric(self.videoData[frameIndex])

        for i in range(0, self.frameCount):
            if i == frameIndex:
                continue
            else:
                similarityMetric = self.computeSimilarityMetric(self.videoData[i])
                avgEuclideanDistance = 0
                sdevEuclideanDistance = 0

                for k in range(0, len(similarityMetric)):
                    avgEuclideanDistance += (frameMetric[k][0] - similarityMetric[k][0]) ** 2
                    sdevEuclideanDistance += (frameMetric[k][1] - similarityMetric[k][1]) ** 2

                comparisonMetric = (i, avgEuclideanDistance, sdevEuclideanDistance)
                similarityMetrics.append(comparisonMetric)

        #Sort the similarity metrics
        similarityMetrics = sorted(similarityMetrics, key = operator.itemgetter(1, 2))

        frameIndices = numpy.zeros(limit)
        for i in range(0, limit):
            frameIndices[i] = similarityMetrics[i][0]

	    print "selected frames: ", frameIndices
        return frameIndices

    def read2DDCTComponentsFromFile(self, filename):
        with open(filename) as f:
            self.frameCount, self.blockCount, self.numFreqComponents = [int(x) for x in f.readline().split(",")]
            videoData = numpy.ndarray(shape=(self.frameCount, self.blockCount, self.numFreqComponents))

            f.readline()

            frameIndex = 0
            blockIndex = 0
            freqComponentIndex = 0

            for line in f:
                frameId, blockCoord, freqCompId, value = [x for x in line.rstrip('\r\n').split(",")]
                frameId = int(frameId)
                blockXCoordinate = int(blockCoord.split("x")[0])
                blockYCoordinate = int(blockCoord.split("x")[1])
                freqCompId = int(freqCompId)
                value = float(str(value))

                videoData[frameIndex][blockIndex][freqComponentIndex] = value

                freqComponentIndex += 1
                if freqComponentIndex == self.numFreqComponents:
                    blockIndex += 1
                    freqComponentIndex = 0

                if blockIndex == self.blockCount:
                    frameIndex += 1
                    blockIndex = 0
        return videoData
