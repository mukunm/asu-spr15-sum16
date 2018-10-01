__author__ = 'Mukund Manikarnike'

import numpy as np
import sys
import math
from datetime import datetime

def getListOfUsers():
    filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
    fileName = "processedTweets.txt"

    # Open the file in read mode
    fileTarget = open(filePath + "\\" + fileName, 'r')
    userIds = {}

    for line in fileTarget:
        lineContent = line.split("\t")
        userId = int(lineContent[0])
        userIds[userId] = 1

    #userIds = list(set(userIds))

    fileTarget.close()

    return userIds


def findCosineSimilarity(filePath, fileName):
    # Open the file in read mode
    fileTarget = open(filePath + "\\" + fileName, 'r')
    adjacencyListDictionary = {}

    userIds = getListOfUsers()

    for line in fileTarget:
        lineContent = line.split(" ")

        userId = int(lineContent[0])

        try:
            access = userIds[userId]
            followerIds = (lineContent[1].strip("\n")).split(",")
            adjacencyListDictionary[userId] = set(map(int, followerIds[:len(followerIds) - 1]))
        except KeyError as e:
            pass

    fileTarget.close()

    cosineSimilarityFileTarget = open(filePath + "\\" + "cosineSimilarity.txt", 'w')
    userIds = adjacencyListDictionary.keys()
    print "No of Unique Users = " + str(len(userIds))
    for i in range(0, len(userIds)):
        for j in range(0, len(userIds)):
            if userIds[i] == userIds[j]:
                continue

            followerSet1 = adjacencyListDictionary[userIds[i]]
            followerSet2 = adjacencyListDictionary[userIds[j]]

            cosineSimilarity = 0.0
            intersectionSetLength = len(followerSet1 & followerSet2)

            if intersectionSetLength != 0:
                cosineSimilarity = intersectionSetLength / float(math.sqrt(len(followerSet1) * len(followerSet2)))

            cosineSimilarityFileTarget.write(str(userIds[i]) + " " + str(userIds[j]) + " " + str(cosineSimilarity) + "\n")

    cosineSimilarityFileTarget.close()


filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
fileName = "formattedGraph.txt"
findCosineSimilarity(filePath, fileName)
