__author__ = 'PawanMahalle'

import HistogramGenerator

# Important note for contributors:
#
# Please refer the following code fragments to get idea accessing a particular values from VideoExtractorObject
#
# API 2 - TestVideoInformationExtractor
# 1. Accessing total number of frames:
#       print videoInformationExtractor.getFrameCount()
# 2. Accessing total number of blocks in a row in zeroth (0th) frame:
#       print videoInformationExtractor.getBlockRowCount()
# 3. Accessing total number of blocks in a col in zeroth (0th) frame:
#       print videoInformationExtractor.getBlockColCount()
# 4. Accessing block (1,2) in frame 0:
#       print videoInformationExtractor.getBlock(0,1,2)

'''
This method extracts n bin histogram feature from video provided.
'''
def extractFeature(videoInformationExtractor, videoFilesPath, videoFileName, n):

    print "Generating n-bin gray scale histogram"
    histogramGenerator = HistogramGenerator.HistogramGenerator(n, 0, 256)
    tupleList = []
    print "There are ",videoInformationExtractor.frameCount," frames in video"
    for frameIndex in range(videoInformationExtractor.frameCount):
        for blockRowIndex in range(videoInformationExtractor.blockRowCount):
            for blockColIndex in range(videoInformationExtractor.blockColCount):
                # get 8*8 block at location (blockRowIndex, blockColIndex)from frame
                block = videoInformationExtractor.getBlock(frameIndex,blockRowIndex, blockColIndex)
                histogramValues = histogramGenerator.generateHistogramValues(block)
                histogramGenerator.generateTupleList(tupleList, frameIndex, (blockRowIndex, blockColIndex), histogramValues)

    outputFileName = str(videoFilesPath+"\\"+videoFileName.split(".")[0])+"_hist_"+str(n)+".hst"

    print "Generating output file"
    f = open(outputFileName,'w')
    f.write(str(videoInformationExtractor.frameCount) + " " + str(videoInformationExtractor.blockRowCount) + " " + str(videoInformationExtractor.blockColCount) + " " + str(n) + "\n")
    f.write('\n'.join('%s %s %s %s %s' % x for x in tupleList))
    f.close()
    print "Output file: ",outputFileName

