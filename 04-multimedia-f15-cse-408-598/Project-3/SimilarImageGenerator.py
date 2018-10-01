__author__ = 'PawanMahalle'

import numpy
import cv2

'''
This class is responsible for generating the similar images from the index of similar frames.
'''
class SimilarImageGenerator:

    @staticmethod
    def generateOutput(videoFilesPath, videoFileName, originalFrame, similarFrameIndices, feature):

        filePrefix = videoFilesPath + "\\" + videoFileName.split(".")[0];

        video = numpy.load(filePrefix+".original")

        cv2.imwrite(filePrefix+"-original-frame" + str(originalFrame)+'.png',video[originalFrame])

        for i in range(0,similarFrameIndices.size):
            cv2.imwrite(filePrefix + "-feature-"+ str(feature) + "-Rank" + str(i+1) + "-frame" + str(similarFrameIndices[i])+'.png',video[similarFrameIndices[i]])
