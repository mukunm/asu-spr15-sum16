__author1__ = 'PawanMahalle'
__author2__ = 'MukundManikarnike'

import HistogramGenerator
import VideoInformationExtractor
import numpy

'''
This method quantizes difference between blocks of kth frame and k + 1th
frame into n bins, except for the last frame.
'''
def extractFeature(videoInformationObject, videoFilesPath, videoFileName, n):

    # Initializes the histogram generator with n bins
    histogramGenerator = HistogramGenerator.HistogramGenerator(n, -256, 256)
    tupleList = []

    print "There are ",videoInformationObject.frameCount," frames in the video"

    # For each frame in the video
    for frameIndex in range(videoInformationObject.frameCount):
        if frameIndex == videoInformationObject.frameCount - 1:
            break

        videoInformation = videoInformationObject.videoInformation;

        # For each block present in the frame.
        for blockRowIndex in range(videoInformationObject.blockRowCount):
            for blockColIndex in range(videoInformationObject.blockColCount):

                # Extract block from current and next frame.
                blockCurrent = videoInformationObject.getBlock(frameIndex, blockRowIndex, blockColIndex)
                blockNext = videoInformationObject.getBlock(frameIndex+1, blockRowIndex, blockColIndex)

                # Compute difference block.
                diffBlock = numpy.subtract(blockCurrent, blockNext)

                # Generate the histogram for the differences.
                histogramValues = histogramGenerator.generateHistogramValues(diffBlock)
                histogramGenerator.generateTupleList(tupleList, frameIndex, (blockRowIndex, blockColIndex), histogramValues)

    print "Generating output file"
    # Create the output file in the location where the video is placed.
    outputFileName = videoFilesPath  + "\\" + videoFileName.split(".")[0] + "_diff_" + str(n) + ".dhc"
    outputFileTarget = open(outputFileName, 'w')
    outputFileTarget.truncate()

    # Write the quantized histogram one after the other into the output file.
    outputFileTarget.write(str(videoInformationObject.frameCount) + " " + str(videoInformationObject.blockRowCount) + " " + str(videoInformationObject.blockColCount) + " " + str(n) + "\n")
    outputFileTarget.write('\n'.join('%s %s %s %s %s' % x for x in tupleList))
    outputFileTarget.close()
    print "Output file: ",outputFileName