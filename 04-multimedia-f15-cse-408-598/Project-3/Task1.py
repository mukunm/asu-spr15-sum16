__author__ = 'PawanMahalle'

import VideoInformationExtractor
import Task1a
import Task1b
import Task1c
import Task1d
import Task2

'''
This class is a driver class that takes video input from user and
provides options to user to selction feature to be extracted.
'''
def main():

    # Set sample video location to a default location
    videoFilesPath = raw_input("Enter the path where the video files are: ")
    videoFileName = raw_input("Enter the name of video file: ")
    # create a videoInformationExtractor object
    videoInformationExtractor = VideoInformationExtractor.TestVideoInformationExtractor(videoFilesPath + "\\" + videoFileName)
    videoInformationExtractor.videoInformation.dump(videoFilesPath + "\\" + videoFileName.split(".")[0]+".original")

    while True:
        # Accept feture number to be extracted from user
        printLine()
        print "Following options are available for feature extraction"
        print "0. Exit"
        print "1. Extract n-bin histogram for 8x8 pixel blocks"
        print "2. Extract n most significant 2-D DCT components for 8x8 pixel blocks"
        print "3. Extract n most significant 2-D DWT components for 8x8 pixel blocks"
        print "4. Extract n-bin histogram for 8x8 pixel difference blocks"
        print "5. Extract m most significant 2-D DWT components per frame"
        feature = input("Enter option: ")

        if feature == 0:
            return
        if feature == 1:
            # Accept value n from user
            n = input("Enter the value of n: ")
            Task1a.extractFeature(videoInformationExtractor, videoFilesPath, videoFileName, n)
        if feature == 2:
            # Accept value n from user
            n = input("Enter the value of n: ")
            blockLevel2DDCTExtractor = Task1b.BlockLevel2DDCTExtractor(videoInformationExtractor, videoFilesPath, videoFileName, n)
            blockLevel2DDCTExtractor.extractFeature()
        if feature == 3:
            # Accept value n from user
            n = input("Enter the value of n: ")
            blockLevel2DDWTExtractor = Task1c.BlockLevel2DDWTExtractor(videoInformationExtractor, videoFilesPath, videoFileName, n)
            blockLevel2DDWTExtractor.extractFeature()
        if feature == 4:
            # Accept value n from user
            n = input("Enter the value of n: ")
            Task1d.extractFeature(videoInformationExtractor, videoFilesPath, videoFileName, n)
        if feature == 5:
            # Accept value n from user
            m = input("Enter the value of m: ")
            frameLevel2DDWTExtractor = Task2.FrameLevel2DDWTExtractor(videoInformationExtractor, videoFilesPath, videoFileName, m)
            frameLevel2DDWTExtractor.extractFeature()

def printLine():
    print "\n------------------------------------------------------------------------------\n"

if __name__ == '__main__':
    main()
