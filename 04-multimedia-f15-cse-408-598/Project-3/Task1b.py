__author__ = 'Runyu Jin'
__author__ = 'Pawan Mahalle'
__author__ = 'Mukund Manikarnike'

import pywt, numpy, sys, time
import VideoInformationExtractor
import math

'''
This class stores all information required to carry out a 2D DCT transform.
Also provides methods to perform 2D DCT.
'''
class BlockLevel2DDCTExtractor:

    '''
    Constructor mandates passing
    1. Number of significant freqency components to be extracted
    2. Number of transformations to be done.
    3. The videoInformation object which contains all frames and blocks
    '''
    def __init__(self, videoInformationObject, videoFilesPath, videoFileName, numFreqComponents):
        self.numFreqComponents = numFreqComponents
        self.videoInformationObject = videoInformationObject
        self.videoFilesPath             = videoFilesPath
        self.videoFileName              = videoFileName
        self.precalculated = numpy.zeros(shape=(8,8,8,8))
        for u in range(0, 8):
            for v in range(0, 8):
                 for i in range(0, 8):
                    for j in range(0, 8):
                        self.precalculated[u][v][i][j] = numpy.cos((2 * i + 1.0) * u * numpy.pi/16) * numpy.cos((2 * j + 1.0) * v * numpy.pi/16)

    '''
    This method implements the 2D DCT transform. It performs this transform by
    averaging and finding differences between the neighboring pixels.
    In a way it can be treated as a 1D DCT applied to each block first on a
    row-wise basis and then on a column-wise basis.
    '''
    def applyTwoDDCTImplemented(self, block):

        # Creates a numpy Array and casts it to float.
        blockDCT = numpy.zeros(shape=(8,8))

        for u in range(0, min(int(math.sqrt(self.numFreqComponents)+1), 8)):
            for v in range(0, min(int(math.sqrt(self.numFreqComponents)+1), 8)):

                if u == 0:
                    Cu = 0.70710678118 # Cu = sqrt(2)/2
                else:
                    Cu = 1

                if v == 0:
                    Cv = 0.70710678118 # Cu = sqrt(2)/2
                else:
                    Cv = 1

                value = 0
                for i in range(0,8):
                    for j in range(0, 8):
                        value += (self.precalculated[u][v][i][j] * block[i][j])
                blockDCT[u][v] = (Cu * Cv / 4.0) * value

        return blockDCT

    '''
    Writes the freqency components to an output file.
    '''
    def writeFreqComponentsToFile(self, fileTarget,  frameIndex, blockRowIndex, blockColIndex, freqComponents):
        n = len(freqComponents)
        for p in range(0, n):
            fileTarget.write(str(frameIndex) + "," + str(blockRowIndex) + " x " + str(blockColIndex) + "," + str(p) + "," + str(freqComponents[p]))
            fileTarget.write("\n")

    '''
    Applies 2D DCT to the video by extracting each frame and each block in each
    frame.
    '''
    def extractFeature(self):
        videoNameExtenstionSplit = self.videoFileName.split(".")
        fileName = self.videoFilesPath   + "\\" +  videoNameExtenstionSplit[0] + "_blockdct_" + str(self.numFreqComponents) + ".bct"
        fileTarget = open(fileName, 'a')
        fileTarget.truncate()
        fileTarget.write(str(self.videoInformationObject.frameCount)+","+str(self.videoInformationObject.blockRowCount * self.videoInformationObject.blockColCount) +","+ str(self.numFreqComponents))
        fileTarget.write("\n")
        fileTarget.write("frame_id,block_coord,freq_comp_id,value")
        fileTarget.write("\n")

        for frameIndex in range(self.videoInformationObject.frameCount):
            for blockRowIndex in range(self.videoInformationObject.blockRowCount):
                for blockColIndex in range(self.videoInformationObject.blockColCount):
                    blockRef = self.videoInformationObject.getBlock(frameIndex, blockRowIndex, blockColIndex)
                    dctBlockArray = self.applyTwoDDCTImplemented(blockRef)

                    '''
                    The extraction of the significant components as
                    described in the application of 2D DCT is carried out in
                    this function. It is carried out in a zig-zag manner.
                    '''
                    dctComponentCount = 0
                    components = numpy.zeros(self.numFreqComponents)
                    for i in range(0, 8):
                        k = 2 * i + 1
                        si = i
                        sj = 0
                        p = 0

                        decrementI = False
                        while p < k:
                            components[dctComponentCount] = dctBlockArray[si][sj]

                            p += 1
                            if p == (i + 1):
                                decrementI = True

                            if decrementI == True:
                                si -= 1
                            else:
                                sj += 1

                            dctComponentCount += 1
                            if dctComponentCount == self.numFreqComponents:
                                break

                        if dctComponentCount == self.numFreqComponents:
                            break

                    self.writeDCTComponentsToFile(fileTarget, frameIndex, blockRowIndex * 8, blockColIndex * 8, components)
        fileTarget.close()
        print "Output file: ",fileTarget.name

    '''
    Writes the dct components to an output file.
    '''
    def writeDCTComponentsToFile(self, fileTarget,  frameIndex, blockRowIndex, blockColIndex, components):
        n = len(components)

        for p in range(0, n):
            fileTarget.write(str(frameIndex) + "," + str(blockRowIndex) + "x" + str(blockColIndex) + "," + str(p) + "," + str(components[p]))
            fileTarget.write("\n")