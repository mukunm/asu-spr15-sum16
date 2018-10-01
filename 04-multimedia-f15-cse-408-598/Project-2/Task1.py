import cv2 as cv2
import sys
from matplotlib.cbook import todate
import numpy as np

# configuration variable
xSpan = 10
ySpan = 10

# predicts the pixel values of next frame using alpha1, alpha2 values which are trained using last 3 values
def encodeUsingTPCOption4(sourceDataForTPEncoding):

    totalPredictionError = 0

    # datastructe to save pixel values of each frame. This data will be used to create tpc file for a video.
    TPEncodedData = np.ndarray((totalFrames, xSpan * ySpan))

    for frameIndex in range(0, sourceDataForTPEncoding.shape[0]):

        for i in range(0, xSpan * ySpan):

            # For first frame, the values in encoded file will be same as that of original video. These values will be used as reference values.
            if frameIndex == 0:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i]
            # For second frame, the values in encoded file will predicted using last frame assuming that alpha1=alpha2=0.5. Since, predictor 4 need corresponding pixel values from last 4 frame which are not available.
            elif frameIndex == 1:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - int(
                    (sourceDataForTPEncoding[frameIndex - 1][i] + sourceDataForTPEncoding[frameIndex - 1][i]) / 2)
                totalPredictionError = totalPredictionError + abs(TPEncodedData[frameIndex][i])
            # For third and forth frame, the values in encoded file will predicted using last 2 frame assuming that alpha1=alpha2=0.5. Since, predictor 4 need corresponding pixel values from last 4 frame which are not available.
            elif frameIndex == 2 or frameIndex == 3:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - int(
                    (sourceDataForTPEncoding[frameIndex - 1][i] + sourceDataForTPEncoding[frameIndex - 2][i]) / 2)
                totalPredictionError = totalPredictionError + abs(TPEncodedData[frameIndex][i])
            # For rest all frames, we have corresponding pixel values from last 4 frames and hence can calculate alpha1 and alpha2 to predict the value.
            else:

                # In order to calculate alpha1 and alpha2, we need to solve a linear equation in two variables.
                # calculate determinent to solve system of the linear equation
                determinant = (sourceDataForTPEncoding[frameIndex - 4][i] * sourceDataForTPEncoding[frameIndex - 2][i]) \
                              - (sourceDataForTPEncoding[frameIndex - 3][i] * sourceDataForTPEncoding[frameIndex - 3][i])

                # If determinent is zero, take alpha1=alpha2=0.5 since solving linear equation will not give any result.
                if determinant == 0:
                    alpha1 = 0.5
                    alpha2 = 0.5
                else:
                    #Calcualte values of alpha1 and alpha2 by solving linear equation with teo variables
                    alpha1 = (((sourceDataForTPEncoding[frameIndex - 1][i] * sourceDataForTPEncoding[frameIndex - 4][
                        i]) + (sourceDataForTPEncoding[frameIndex - 2][i] * sourceDataForTPEncoding[frameIndex - 3][
                        i] * -1)) / determinant)

                    alpha2 = (((sourceDataForTPEncoding[frameIndex - 1][i] * sourceDataForTPEncoding[frameIndex - 3][
                        i] * -1) + (
                               sourceDataForTPEncoding[frameIndex - 2][i] * sourceDataForTPEncoding[frameIndex - 2][
                                   i])) / determinant)

                    if alpha1 + alpha2 == 0:
                        alpha1 = 0.5
                        alpha2 = 0.5
                try:
                    #Predict the values using the calculated alpha1 and alpha2 value.
                    predictedValue = (int((sourceDataForTPEncoding[frameIndex - 1][i] * alpha1) + (
                    sourceDataForTPEncoding[frameIndex - 2][i] * alpha2)))
                    if 0 <= predictedValue and predictedValue <= 255:
                        TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - predictedValue
                    else:
                        TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - (int(
                            (sourceDataForTPEncoding[frameIndex - 1][i] * 0.5) + (
                            sourceDataForTPEncoding[frameIndex - 2][i] * 0.5)))

                except:
                    e = sys.exc_info()
                    print e
                totalPredictionError = totalPredictionError + abs(TPEncodedData[frameIndex][i])

    return TPEncodedData, totalPredictionError

# returns the encoded values predicted using predictive encoding strategy 3.
def encodeUsingTPCOption3(sourceDataForTPEncoding):

    totalPredictionError = 0

    # datastructe to save pixel values of each frame. This data will be used to create tpc file for a video.
    TPEncodedData = np.ndarray((totalFrames, xSpan * ySpan))

    for frameIndex in range(0, sourceDataForTPEncoding.shape[0]):
        for i in range(0, xSpan * ySpan):

            # For first frame, the values in encoded file will be same as that of original video. These values will be used as reference values.
            if frameIndex == 0:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i]
            # For second frame, the values in encoded file will predicted using last frame assuming that alpha1=alpha2=0.5. Since, predictor 4 need corresponding pixel values from last 4 frame which are not available.
            elif frameIndex == 1:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - (
                (sourceDataForTPEncoding[frameIndex - 1][i] + sourceDataForTPEncoding[frameIndex - 1][i]) / 2)
                totalPredictionError = totalPredictionError + abs(TPEncodedData[frameIndex][i])
            # From third frame onwards, we have sufficient values to predict pixel value in next frame
            else:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - (
                int((sourceDataForTPEncoding[frameIndex - 1][i] + sourceDataForTPEncoding[frameIndex - 2][i]) / 2))
                totalPredictionError = totalPredictionError + abs(TPEncodedData[frameIndex][i])


    return TPEncodedData, totalPredictionError

# returns the encoded values predicted using predictive encoding strategy 2.
def encodeUsingTPCOption2(sourceDataForTPEncoding):

    totalPredictionError = 0

    # datastructe to save pixel values of each frame. This data will be used to create tpc file for a video.
    TPEncodedData = np.ndarray((totalFrames, xSpan * ySpan))

    for frameIndex in range(0, sourceDataForTPEncoding.shape[0]):
        for i in range(0, xSpan * ySpan):
            # For first frame, the values in encoded file will be same as that of original video. These values will be used as reference values.
            if frameIndex == 0:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i]
            #From second frames onward, use pixel value from previous frame to predict current frame pixel value
            else:
                TPEncodedData[frameIndex][i] = sourceDataForTPEncoding[frameIndex][i] - \
                                               sourceDataForTPEncoding[frameIndex - 1][i]
                totalPredictionError = totalPredictionError + abs(TPEncodedData[frameIndex][i])

    return TPEncodedData, totalPredictionError


def calculateTemporalPredictiveEncoding(sourceDataForTPEncoding, option):
    if option == 1:
        return sourceDataForTPEncoding, 0
    elif option == 2:
        return encodeUsingTPCOption2(sourceDataForTPEncoding)
    elif option == 3:
        return encodeUsingTPCOption3(sourceDataForTPEncoding)
    elif option == 4:
        return encodeUsingTPCOption4(sourceDataForTPEncoding)
    else:
        print "Please enter valid temporal predictive coding option."
        sys.exit(0)

# extracts the windows from frame of size xSpan X ySpan starting with top-right corner.
def extract(frame, xCoordinate, yCoordinate, xSpan, ySpan):
    return frame[xCoordinate:xCoordinate + xSpan, yCoordinate:yCoordinate + ySpan]

#########################################################################################
#                                   Main                                                #
#########################################################################################

videoFilesPath = raw_input("Enter the Path where the videos are stored: ")

videoFileNumber = input("Enter the video file number: ")
xCoordinate = input("Enter the top-left X coordinate:")
yCoordinate = input("Enter the top-left X coordinate:")

print "Select any one of the following:"
print "Press 1 for PC Option 1"
print "Press 2 for PC Option 2"
print "Press 3 for PC Option 3"
print "Press 4 for PC Option 4"
option = input("Enter choice: ")

print "Temporal Predictive Encoding"

print "=====================Input=========================="

print "Video file chosen: " + str(videoFileNumber) + ".mp4"
print "Temporal predictive encoding option: " + str(option)
print "Selected (x,y) location : (" + str(xCoordinate) + "," + str(yCoordinate) + ")"
print "Window size (by default): " + str(xSpan) + "x" + str(ySpan)

vidcap = cv2.VideoCapture(videoFilesPath + "\\" + str(videoFileNumber) + ".mp4")

totalFrames = int(vidcap.get(7))

print "Total frames in video: " + str(totalFrames)

sourceDataForTPEncoding = np.ndarray((totalFrames, xSpan * ySpan))

for frameIndex in range(0, totalFrames):

    ret, colorFrame = vidcap.read()

    grayFrame = cv2.cvtColor(colorFrame, cv2.COLOR_BGR2GRAY)

    window = extract(grayFrame, xCoordinate, yCoordinate, xSpan, ySpan)

    for i in range(0, xSpan):
        for j in range(0, ySpan):
            try:
                sourceDataForTPEncoding[frameIndex][i * ySpan + j] = window[i][j]
            except:
                print "Issue while creating source data for temporal encoding for following values:"
                print "i = " + str(i)
                print "j = " + str(j)
                print "frameindex = " + str(frameIndex)

np.savetxt(str(videoFileNumber) + ".original", sourceDataForTPEncoding, fmt='%i', delimiter='\t')
print "Original pixel values file: " + str(videoFileNumber) + ".original"

TPEncodedData, totalPredictionError = calculateTemporalPredictiveEncoding(sourceDataForTPEncoding, option);

np.savetxt(str(videoFileNumber) + "_" + str(option) + ".tpc", TPEncodedData, fmt='%i', delimiter='\t')
print "Temporal Predictive coding output file: " + str(videoFileNumber) + "_" + str(option) + ".tpc"

print "Total prediction error: " + str(totalPredictionError)

vidcap.release()

cv2.destroyAllWindows()

print "Done"
