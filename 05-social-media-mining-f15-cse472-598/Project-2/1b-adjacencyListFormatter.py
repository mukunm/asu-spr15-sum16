__author__ = 'Mukund Manikarnike'

import numpy as np
import sys
from sklearn.metrics import pairwise_distances
from scipy.spatial.distance import cosine
from datetime import datetime

def createFormattedAdjacencyList(filePath, fileName):
    # Open the file in read mode
    fileTarget = open(filePath + "\\" + fileName, 'r')
    formattedAdjacencyListFileTarget = open(filePath + "\\" + "formattedGraph.txt", 'w')

    previousUserId = 0

    for line in fileTarget:
        lineContent = line.split(" ")

        userId     = int(lineContent[0])
        followerId = int(lineContent[1].strip("\n"))

        if previousUserId != userId:
            formattedAdjacencyListFileTarget.write("\n")
            formattedAdjacencyListFileTarget.write(str(userId) + " " + str(followerId) + ",")
        else:
            formattedAdjacencyListFileTarget.write(str(followerId) + ",")

        previousUserId = userId

    fileTarget.close()
    formattedAdjacencyListFileTarget.close()

filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
fileName = "graph.txt"
createFormattedAdjacencyList(filePath, fileName)
