__author__ = 'PawanMahalle'

import numpy

'''
This class is responsible for generating histogram out of the video input provided.
'''
class HistogramGenerator:

    def __init__(self, binCount, rangeLowerBound, rangeUpperBound):
        self.binCount = binCount
        self.rangeLowerBound = rangeLowerBound
        self.rangeUpperBound = rangeUpperBound
        self.interval = (rangeUpperBound - rangeLowerBound)//binCount

    def getGrayInstanceID(self, grayPixelValue):
        return grayPixelValue//(self.interval)

    def generateHistogramValues(self, block):

        hist = numpy.histogram(block, self.binCount, (self.rangeLowerBound, self.rangeUpperBound))

        return hist[0]

    def generateTupleList(self, tupleList, frameIndex, (blockRowIndex, blockColIndex), histogramValues):
        for i in range(self.binCount):
            tuple = (frameIndex, blockRowIndex, blockColIndex, i, histogramValues[i])
            tupleList.append(tuple)
