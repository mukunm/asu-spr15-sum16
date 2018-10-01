__author__ = 'Mukund Manikarnike'

import pywt, numpy, sys
import math
import VideoInformationExtractor

'''
This class stores all information required to carry out a 2D DWT transform.
Also provides methods to perform 2D DWT.
'''
class FrameLevel2DDWTExtractor:
    '''
    Constructor mandates passing
    1. Number of significant wavelet components to be extracted
    2. Number of transformations to be done.
    3. The videoInformation object which contains all frames and blocks
    '''
    def __init__(self, videoInformationObject, videoFilesPath, videoFileName, numWaveletComponents):
        frameWidth = videoInformationObject.getFrame(0).shape[0]
        frameHeight = videoInformationObject.getFrame(0).shape[1]
        nThreshold = frameWidth * frameHeight

        if numWaveletComponents > nThreshold:
            print "ERROR: You need to enter a valid value of n."
            print "We can have " + str(nThreshold) + " most significant values at max for frame of size" + str(frameWidth) + " x " + str(frameHeight)
            sys.exit()

        self.numWaveletComponents       = numWaveletComponents
        self.videoInformationObject     = videoInformationObject
        self.num2DHaarTransformations   = int(math.log(frameWidth, 2))
        self.videoFilesPath             = videoFilesPath
        self.videoFileName              = videoFileName

    '''
    This is a test method which uses PyWT library
    This can be used for comparison with custom implementation.
    '''
    def applyTwoDDWTUsingLibrary(self, blockList):
        '''2D DWT using the pywt library.'''
        coeffs = pywt.dwt2(blockRef.getValue(), 'haar')
        cA, (cH, cV, cD) = coeffs
        print cA
        print cV
        print coeffs

    '''
    This method implements the 2D DWT transform. It performs this transform by
    averaging and finding differences between the neighboring pixels.
    In a way it can be treated as a 1D DWT applied to each block first on a
    row-wise basis and then on a column-wise basis.
    '''
    def applyTwoDDWTImplemented(self, frameRef):
        scale = numpy.sqrt(4.)

        # Creates a numpy Array and casts it to float.
        frameRef = frameRef.astype(float)
        n = self.num2DHaarTransformations
        width = frameRef.shape[0]
        height = frameRef.shape[1]

        '''
        The 2D transform can be applied upto a maximum log to base 2 of the
        dimensions of the region to which it is being applied. The process is
        illustrated below
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  a  |  a  |  a  |  a  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  a  |  a  |  a  |  a  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  a  |  a  |  a  |  a  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  a  |  a  |  a  |  a  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  d  |  d  |  d  |  d  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  d  |  d  |  d  |  d  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  d  |  d  |  d  |  d  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        |  d  |  d  |  d  |  d  |  d  |  d  |  d  |  d  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
        If there's an 8 x 8 region, the uper left quadrant would contain the
        averages after one round of 2D DWT. More such rounds, would leave
        significant information on the upper left quadrants of the region
        and also with more such rounds, the size of the upper left quadrant
        would diminish by a power of 2. Hence, for an 8 x 8 region, we can apply
        2D DWT at max 3 times.
        Once 2D DWT is done, the upper left quadrant contains the averages and
        the other 4 regions constitute the detail in the image. The extraction
        of significant components is done in a zig zag order to capture higher
        detail because the values at the bottom right quadrant tend towards
        zero.
        '''
        for waveletComponentIndex in range(0, n):
            for rowIndex in range(0, (2 ** (int(math.log(height, 2)) - waveletComponentIndex))):
                currentRow = frameRef[rowIndex, :(2 ** (int(math.log(height, 2)) - waveletComponentIndex))]
                avg = (currentRow[0::2] + currentRow[1::2]) / scale
                detail = (currentRow[0::2] - currentRow[1::2]) / scale
                frameRef[rowIndex, :(2 ** (int(math.log(height, 2)) - waveletComponentIndex))] = numpy.concatenate((avg, detail), axis = 0)

            for colIndex in range(0, (2 ** (int(math.log(width, 2)) - waveletComponentIndex - 1))):
                currentColumn = frameRef[:(2 ** (int(math.log(width, 2)) - waveletComponentIndex)), colIndex]
                avg = (currentColumn[0::2] + currentColumn[1::2]) / scale
                detail = (currentColumn[0::2] - currentColumn[1::2]) / scale
                frameRef[:(2 ** (int(math.log(width, 2)) - waveletComponentIndex)), colIndex] = numpy.concatenate((avg, detail), axis = 0)
        return frameRef

    '''
    Writes the wavelet components to an output file.
    '''
    def writeWaveletComponentsToFile(self, fileTarget,  frameIndex, waveletComponents):
        n = len(waveletComponents)

        for p in range(0, n):
            fileTarget.write(str(frameIndex) + "," + str(p) + "," + str(waveletComponents[p]))
            fileTarget.write("\n")

    '''
    Applies 2D DWT to the video by extracting each frame and each block in each
    frame.
    '''
    def extractFeature(self):
        videoNameExtenstionSplit = self.videoFileName.split(".")
        fileName = self.videoFilesPath  + "\\" + videoNameExtenstionSplit[0] + "_framedwt_" + str(self.numWaveletComponents) + ".fwt"
        fileTarget = open(fileName, 'a')
        fileTarget.truncate()
        fileTarget.write(str(self.videoInformationObject.frameCount) + "," + str(self.numWaveletComponents))
        fileTarget.write("\n")
        fileTarget.write("frame_id,wavelet_comp_id,value")
        fileTarget.write("\n")

        for frameIndex in range(self.videoInformationObject.frameCount):
            frameRef = self.videoInformationObject.getFrame(frameIndex)
            width = frameRef.shape[0]
            height = frameRef.shape[1]
            dwtArray = self.applyTwoDDWTImplemented(frameRef)

            '''
            The extraction of the significant wavelet components as
            described in the application of 2D DWT is carried out in
            this function. It is carried out in a zig-zag manner.
            '''
            waveletComponentCount = 0
            wavelets = numpy.zeros(self.numWaveletComponents)
            for i in range(0, height):
                k = 2 * i + 1
                si = i
                sj = 0
                p  = 0

                decrementI = False
                while p < k:

                    wavelets[waveletComponentCount] = dwtArray[si][sj]

                    p += 1
                    if p == (i + 1):
                        decrementI = True

                    if decrementI == True:
                        si -= 1
                    else:
                        sj += 1

                    waveletComponentCount += 1
                    if waveletComponentCount == self.numWaveletComponents:
                        break

                if waveletComponentCount == self.numWaveletComponents:
                    break

            self.writeWaveletComponentsToFile(fileTarget, frameIndex, wavelets)

        fileTarget.close()
        print "Output file: ",fileTarget.name