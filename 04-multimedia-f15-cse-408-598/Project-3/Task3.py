__author__ = 'PawanMahalle'

import SimilarityMatcher
import SimilarImageGenerator

'''
This is driver class for task 3 of similarity matching and image generation.
'''
def main():

    # Set sample video location to a default location
    videoFilesPath = raw_input("Enter the path where feature files are kept: ")

    while True:
        printLine()
        videoFileName = raw_input("Enter the name of video file: ")
        # Accept id of frame to be queried
        frameId = input("Enter the value of frame id: ")
        # Accept n
        n = input("Enter the value of n: ")
        # Accept m
        m = input("Enter the value of m: ")
        # Accept feature type to match

        print "Enter the feature to be matched for similar frames"
        print "1. n-bin histogram"
        print "2. 2-D DCT components of blocks"
        print "3. 2-D DWT components of blocks"
        print "4. n-bin block difference histogram"
        print "5. 2-D DWT components of frames"
        print "6. All individually"
        feature = input("Enter choice ")

        if feature == 1 or feature == 6:
            similarityMatcher = SimilarityMatcher.NBinHistogramSimilarityMatcher(videoFilesPath, videoFileName, n, feature)
            similarFrameIndices = similarityMatcher.getSimilarFrames(frameId, 10)
            SimilarImageGenerator.SimilarImageGenerator.generateOutput(videoFilesPath, videoFileName, frameId, similarFrameIndices, feature)
        if feature == 2 or feature == 6:
            similarityMatcher = SimilarityMatcher.BlockLevel2DDCTSimilarityMatcher(videoFilesPath, videoFileName, n)
            similarFrameIndices = similarityMatcher.getSimilarFrames(frameId, 10)
            SimilarImageGenerator.SimilarImageGenerator.generateOutput(videoFilesPath, videoFileName, frameId, similarFrameIndices, feature)
        if feature == 3 or feature == 6:
            similarityMatcher = SimilarityMatcher.BlockLevel2DDWTSimilarityMatcher(videoFilesPath, videoFileName, n)
            similarFrameIndices = similarityMatcher.getSimilarFrames(frameId, 10)
            SimilarImageGenerator.SimilarImageGenerator.generateOutput(videoFilesPath, videoFileName, frameId, similarFrameIndices, feature)
        if feature == 4 or feature == 6:
            similarityMatcher = SimilarityMatcher.NBinDiffHistogramSimilarityMatcher(videoFilesPath, videoFileName, n, feature)
            similarFrameIndices = similarityMatcher.getSimilarFrames(frameId, 10)
            SimilarImageGenerator.SimilarImageGenerator.generateOutput(videoFilesPath, videoFileName, frameId, similarFrameIndices, feature)
        if feature == 5 or feature == 6:
            similarityMatcher = SimilarityMatcher.FrameLevel2DDWTSimilarityMatcher(videoFilesPath, videoFileName, m)
            similarFrameIndices = similarityMatcher.getSimilarFrames(frameId, 10)
            SimilarImageGenerator.SimilarImageGenerator.generateOutput(videoFilesPath, videoFileName, frameId, similarFrameIndices, feature)

        exitCode = raw_input("\n...press 0 to exit: ")
        if exitCode == "0":
            return

def printLine():
    print "\n------------------------------------------------------------------------------\n"

if __name__ == '__main__':
    main()
