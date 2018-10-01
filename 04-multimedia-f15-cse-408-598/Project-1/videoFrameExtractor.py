# Author(s) - Mukund Manikarnike

import cv2 as cv2
import sys
import numpy as np

# Define a class to hold one color map instance
class ColorMapInstance(object):
    def __init__(self, channel1, channel2, channel3):
        self.channel1 = channel1
        self.channel2 = channel2
        self.channel3 = channel3

try:
    while 1:
        # Read inputs from the user
        videoFilesPath = raw_input("Enter the Path where the videos are stored: ")
        videoFileFullName = raw_input("Enter the name of the file to be processed: ")
        videoFileNameSplit = videoFileFullName.split(".")
        videoFileName = videoFileNameSplit[0]

        # Open the video capture
        vidcap = cv2.VideoCapture(videoFilesPath + "\\" + videoFileFullName)

        # Read and display the number of frames in the video for the user's
        # benefit.
        totalNumFrames = vidcap.get(7)
        print "The video contains " + str(totalNumFrames) + " frames.\n"

        print "It looks like you're looking to process 2 frames in the video.\n"
        print "Frame numbers start from 1 to " + str(totalNumFrames)
        frame1Num = int(raw_input("Enter the frame number of the 1st frame: "))
        frame2Num = int(raw_input("Enter the frame number of the 2nd frame: "))

        # Check if frame numbers are valid
        if frame1Num <= 0 or frame1Num > totalNumFrames:
            print "Invalid Frame 1 Number. Provided Input: " + str(frame1Num) + "Expected range 1 to " + str(totalNumFrames) + "\n"
            break

        if frame2Num <= 0 or frame2Num > totalNumFrames:
            print "Invalid Frame 2 Number. Provided Input: " + str(frame2Num) + "Expected range 1 to " + str(totalNumFrames) + "\n"
            break

        # Read frame 1 number of times to get the nth frame
        i = 0
        while i < frame1Num:
            success, image = vidcap.read()
            i += 1

        # Write Frame 1 to an image file
        if success:
            # Convert the image that has been read to grayscale and display
            gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
            cv2.imwrite(videoFilesPath + "\\" + videoFileName + "-frame-" + str(frame1Num) + "-gray.jpg", gray)
            print "Frame No. " + str(frame1Num) + " has been written to the file \"" + videoFileName + "-frame-" + str(frame1Num) + "-gray.jpg\""
        else:
            print "Error: Frame 1 read failed"

        # Release the video capture. We no longer need the video. All images
        # required have been extracted from the video.
        vidcap.release()

        # Open the video capture
        vidcap = cv2.VideoCapture(videoFilesPath + "\\" + videoFileFullName)

        # Read frame 2 number of times get the nth frame
        i = 0
        while i < frame2Num:
            success, image = vidcap.read()
            i += 1

        # Write Frame 2 to an image file
        if success:
            # Convert the image that has been read to grayscale and display
            gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
            cv2.imwrite(videoFilesPath + "\\" + videoFileName + "-frame-" + str(frame2Num) + "-gray.jpg", gray)
            print "Frame No. " + str(frame2Num) + " has been written to the file \"" + videoFileName + "-frame-" + str(frame2Num) + "-gray.jpg\""
        else:
            print "Error: Frame 2 read failed"

        # Release the video capture. We no longer need the video. All images
        # required have been extracted from the video.
        vidcap.release()

        # The value 0 is passed to the imread function to read the image in
        # grayscale
        gray1 = cv2.imread(videoFilesPath + "\\" + videoFileName + "-frame-" + str(frame1Num) + "-gray.jpg", 0)
        gray2 = cv2.imread(videoFilesPath + "\\" + videoFileName + "-frame-" + str(frame2Num) + "-gray.jpg", 0)

        w1 = gray1.shape[1]
        h1 = gray1.shape[0]

        w2 = gray2.shape[1]
        h2 = gray2.shape[0]

        if w1 != w2 or h1 != h2:
            print "Error: Frame 1 and Frame 2 sizes don't match\n"
            break

        width = w1
        height = h1

        print "Creating Diff image"
        print "Would you like to visualize the difference image with a color map file?\n"
        visualizeColorMap = raw_input("Enter y for Yes, n for No: ")

        # If yes, read color map file
        if visualizeColorMap == "y" or visualizeColorMap == "Y":
            # Read the colorMap File and map the difference image to the color map
            colorMapFileName = raw_input("Enter the name of the color map file: ")
            colorMaps = colorMapFileName.split("-")
            colorSpaceName = colorMaps[1]
            c1r      = colorMaps[2]
            c1g      = colorMaps[3]
            c1b      = colorMaps[4]
            c2r      = colorMaps[5]
            c2g      = colorMaps[6]
            c2b      = colorMaps[7]
            c3r      = colorMaps[8]
            c3g      = colorMaps[9]
            c3b      = colorMaps[10]

            reducedColorInstances = []
            actualColorInstances = []

            # Loop through all the lines in the file and populate the color instance
            # lists.
            with open(videoFilesPath + "\\" + colorMapFileName, 'r') as colorMapFile:
                for line in colorMapFile:
                    # Strip the new line character at the end of each line
                    line = line.rstrip("\n")

                    # Extract the left and right sides of the color map
                    colorMapInstanceValues = line.split(":")

                    # Store the left side as reduced instances
                    reducedColorInstances.append(float(colorMapInstanceValues[0]))

                    # Split the right side into 3 channels
                    actualColorInstanceChannels = colorMapInstanceValues[1].split(",")

                    # Store the 3 channels as an actual color instance
                    actualColorInstances.append(ColorMapInstance(float(actualColorInstanceChannels[0]),
                                                                 float(actualColorInstanceChannels[1]),
                                                                 float(actualColorInstanceChannels[2])))

        #Create a temporary location for the diffImageNormalized
        diffImage = np.zeros((height, width, 1), np.float32)
        diffImage1 = np.zeros((height, width, 1), np.float32)
        diffImageNormalized = np.zeros((height, width, 1), np.float32)
        diffImageNormalized1 = np.zeros((height, width, 1), np.float32)

        # Create a temporary location for Mapped image
        if visualizeColorMap == "y" or visualizeColorMap == "Y":
            reducedColorInstanceDifference = reducedColorInstances[1] - reducedColorInstances[0]
            mappedImage = np.zeros((height, width, 3), np.uint8)
            mappedImage1 = np.zeros((height, width, 3), np.uint8)
            indexComputationError = 0

        # Loop through all pixels of both images, compute the difference and
        # scale it down to a range between -1 and 1.
        p = 0
        while p < height:
            q = 0
            while q < width:
                # Extract pixels from both the grayscales
                pixel1 = gray1[p, q]
                pixel2 = gray2[p, q]

                # Calculate the result pixel
                resultPixel = float(pixel2) - float(pixel1)
                resultPixel1 = float(pixel1) - float(pixel2)
                resultPixelNormalized = (float(pixel2) - float(pixel1)) / float(255)
                resultPixelNormalized1 = (float(pixel1) - float(pixel2)) / float(255)

                # Save the result pixel
                diffImage[p, q] = resultPixel
                diffImage1[p, q] = resultPixel1
                diffImageNormalized[p, q] = resultPixelNormalized
                diffImageNormalized1[p, q] = resultPixelNormalized1

                # If the difference image needs to be visualized using a color
                # map, proceed further
                if visualizeColorMap == "y" or visualizeColorMap == "Y":
                    # Calculate the index of the current pixel value in the
                    # reduced color instances.
                    # The reduced color instances in the color map will always
                    # be an arithmetic progression and hence we can find the
                    # index where the current pixel lies in the list using
                    # nth Term = a + (n - 1) * d
                    # where a is the first term, n is the index, d is difference
                    # a is constant and equal to -1
                    pixelIndex = int(((float(resultPixelNormalized) + 1) / float(reducedColorInstanceDifference)) + 1)

                    if pixelIndex > len(actualColorInstances):
                        pixelIndex = len(actualColorInstances)
                        indexComputationError += 1

                    mappedImage[p, q] = (actualColorInstances[pixelIndex - 1].channel1,
                                         actualColorInstances[pixelIndex - 1].channel2,
                                         actualColorInstances[pixelIndex - 1].channel3)

                    pixelIndex1 = int(((float(resultPixelNormalized1) + 1) / float(reducedColorInstanceDifference)) + 1)
                    mappedImage1[p, q] = (actualColorInstances[pixelIndex1 - 1].channel1,
                                         actualColorInstances[pixelIndex1 - 1].channel2,
                                         actualColorInstances[pixelIndex1- 1].channel3)

                q += 1
            q = 0
            p += 1

        #Write all pixels of an image to a text file
        fileName = videoFilesPath + "\\" + str(frame2Num) + "-minus-" + str(frame1Num) + "-" + colorSpaceName + "-" + c1r + "-" + c1g + "-" + c1b + "-" + c2r + "-" + c2g + "-" + c2b + "-" + c3r + "-" + c3g + "-" + c3b + ".txt"
        fileTarget = open(fileName, 'w')
        fileTarget.truncate()

        for p in range(0, 10):
            for q in range(0, 10):
                fileTarget.write(str(diffImageNormalized[p, q]) + ", ")
            fileTarget.write("\n")
        fileTarget.close()

        #Write all pixels of an image to a text file
        fileName = videoFilesPath + "\\" + str(frame1Num) + "-minus-" + str(frame2Num) + "-" + colorSpaceName + "-" + c1r + "-" + c1g + "-" + c1b + "-" + c2r + "-" + c2g + "-" + c2b + "-" + c3r + "-" + c3g + "-" + c3b + ".txt"
        fileTarget = open(fileName, 'w')
        fileTarget.truncate()

        for p in range(0, 10):
            for q in range(0, 10):
                fileTarget.write(str(diffImageNormalized1[p, q]) + ", ")
            fileTarget.write("\n")
        fileTarget.close()

        cv2.imwrite(videoFilesPath + "\\" + videoFileName + "-" + str(frame2Num) + "-minus-" + str(frame1Num) + "-grayDiff.jpg", diffImage)
        cv2.imwrite(videoFilesPath + "\\" + videoFileName + "-" + str(frame1Num) + "-minus-" + str(frame2Num) + "-grayDiff.jpg", diffImage1)
        print "Difference Image in gray scale has been written to the file " + videoFileName + "-" + str(frame2Num) + "-minus-" + str(frame1Num) + "-grayDiff.jpg"

        if visualizeColorMap == "y" or visualizeColorMap == "Y":
            cv2.imwrite(videoFilesPath + "\\" + videoFileName + "-" + str(frame2Num) + "-minus-" + str(frame1Num) + "-" + colorSpaceName + "-" + c1r + "-" + c1g + "-" + c1b + "-" + c2r + "-" + c2g + "-" + c2b + "-" + c3r + "-" + c3g + "-" + c3b + "-grayDiffMapped.jpg", mappedImage)
            print "Difference Image in gray scale mapped using the color map has been written to the file " + videoFileName + "-" + str(frame2Num) + "-minus-" + str(frame1Num) + "-" + colorSpaceName + "-" + c1r + "-" + c1g + "-" + c1b + "-" + c2r + "-" + c2g + "-" + c2b + "-" + c3r + "-" + c3g + "-" + c3b + "-grayDiffMapped.jpg"
            cv2.imwrite(videoFilesPath + "\\" + videoFileName + "-" + str(frame1Num) + "-minus-" + str(frame2Num) + "-" + colorSpaceName + "-" + c1r + "-" + c1g + "-" + c1b + "-" + c2r + "-" + c2g + "-" + c2b + "-" + c3r + "-" + c3g + "-" + c3b + "-grayDiffMapped.jpg", mappedImage1)
            print "Difference Image in gray scale mapped using the color map has been written to the file " + videoFileName + "-" + str(frame1Num) + "-minus-" + str(frame2Num) + "-" + colorSpaceName + "-" + c1r + "-" + c1g + "-" + c1b + "-" + c2r + "-" + c2g + "-" + c2b + "-" + c3r + "-" + c3g + "-" + c3b + "-grayDiffMapped.jpg"
            #print "Index Computation Error = " + str(indexComputationError)

        # Prompt to exit from the loop
        continueRun = raw_input("Enter 'x' to exit, anything else to continue with processing another video: ")
        if continueRun == "x" or continueRun == "X":
            break

except:
    e = sys.exc_info()
    print e
