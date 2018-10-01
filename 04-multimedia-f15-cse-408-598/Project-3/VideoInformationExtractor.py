__author__ = 'PawanMahalle'

import cv2 as cv2
import numpy

''' This class contains critical information about the video.
 Important methods:
 1. getFrame(frameIndex) - returns Frame object for given frameIndex
 2. getFrameCount() - returns total count of frames in the video
'''

class TestVideoInformationExtractor:

    # This constructor extracts the information from video file
    def __init__(self, path):

        filename = path.split('\\')[-1].split(".")[0]

        # Read video into VideoCapture object
        vidcap = cv2.VideoCapture(path)

        self.frameCount = int(vidcap.get(7))

        ret, colorFrame = vidcap.read()

        # Convert color frames to gray frames
        grayFrame = cv2.cvtColor(colorFrame, cv2.COLOR_BGR2GRAY)

        self.videoInformation = numpy.ndarray(shape=(self.frameCount, grayFrame.shape[0], grayFrame.shape[1]))
        self.videoInformation[0] = grayFrame
        self.blockRowCount = grayFrame.shape[0]//8
        self.blockColCount = grayFrame.shape[1]//8

        with open(filename + '.original','w') as f_handle:
            numpy.savetxt(f_handle,grayFrame, fmt="%d")

        for frameIndex in range(1, self.frameCount):

            # Read frames from the image with RGB color instances
            ret, colorFrame = vidcap.read()

            # Convert color frames to gray frames
            grayFrame = cv2.cvtColor(colorFrame, cv2.COLOR_BGR2GRAY)

            with open(filename + '.original','a') as f_handle:
                numpy.savetxt(f_handle,grayFrame, fmt="%d")

            self.videoInformation[frameIndex] = grayFrame

    def getFrame(self, frameIndex):
        return self.videoInformation[frameIndex]

    def getBlock(self, frameIndex, x, y):
        return self.getFrame(frameIndex)[x*8:x*8+8,y*8:y*8+8]

    def getPixel(self, frameIndex, x, y, i, j):
        return self.getBlock(frameIndex, x, y)[i,j]

