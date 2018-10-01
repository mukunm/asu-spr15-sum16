__author__ = 'Mukund Manikarnike'

from nltk.corpus import wordnet as wn
from datetime import datetime
from itertools import groupby
from collections import Counter
import operator
import sys

def findNounFrequencies():
    filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
    fileName = "nouns.txt"
    fileTarget = open(filePath + "\\" + fileName, 'r')
    fileName = "nounFrequency.txt"
    nounFrequencyTarget = open(filePath + "\\" + fileName, 'w')

    nouns = []

    for line in fileTarget:
        lineContent = line.strip("\n")
        nouns.append(lineContent)

    nounFrequencies = dict(Counter(nouns))
    keys = nounFrequencies.keys()

    for i in range(0, len(nounFrequencies)):
        nounFrequencyTarget.write(str(keys[i]) + "\t" + str(nounFrequencies[keys[i]]) + "\n")

    fileTarget.close()
    nounFrequencyTarget.close()

    return list(set(nouns)), nounFrequencies

def findNounImportance():
    filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
    fileName = "nounImportance.txt"
    fileTarget = open(filePath + "\\" + fileName, 'w')

    nouns, nounFrequencies = findNounFrequencies()
    nounImportance = {}

    print "No. of Unique Nouns = " + str(len(nounFrequencies))

    for i in range(0, len(nouns)):
        for j in range(0, len(nouns)):
            if i == j:
                continue

            synsets1 = wn.synsets(nouns[i])
            synsets2 = wn.synsets(nouns[j])

            maxm = 0
            for p in synsets1:
                for q in synsets2:
                    if p == q:
                        continue

                    sim = wn.path_similarity(p, q)

                    if sim > maxm:
                        maxm = sim

            try:
                currentSimilarity = nounImportance[nouns[i]]
                nounImportance[nouns[i]] = currentSimilarity + ((nounFrequencies[nouns[i]] - nounFrequencies[nouns[j]]) * maxm)
            except KeyError as e:
                nounImportance[nouns[i]] = maxm

        fileTarget.write(nouns[i] + "\t" + str(nounImportance[nouns[i]]) + "\n")

    fileTarget.close()

    return nouns, nounFrequencies, nounImportance

def readNounImportances():
    filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
    fileName = "nounImportance.txt"
    fileTarget = open(filePath + "\\" + fileName, 'r')

    nounImportances = {}
    for line in fileTarget:
        lineContent = line.split("\t")
        nounImportances[lineContent[0]] = float(lineContent[1].strip("\n"))

    fileTarget.close()
    return nounImportances

def findTweetImportances():
    #nouns, nounFrequencies, nounImportance = findNounImportance()
    nounImportance = readNounImportances()
    print "Noun Importances Found"

    filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
    fileName = "processedTweets.txt"
    fileTarget = open(filePath + "\\" + fileName, 'r')

    fileName = "tweetImportance.txt"

    tweetImportances = []
    tweetTags = {}

    for line in fileTarget:
        lineContent = line.split("\t")
        tweetId = int(lineContent[0])

        strNounList = lineContent[3]
        strNounList = strNounList.replace("[", "")
        strNounList = strNounList.replace("]", "")
        strNounList = strNounList.replace("'", "")
        strNounList = strNounList.replace(" ", "")
        nounList = strNounList.split(",")

        strHashTagList = lineContent[4]
        strHashTagList = strHashTagList.replace("[", "")
        strHashTagList = strHashTagList.replace("]", "")
        strHashTagList = strHashTagList.replace("'", "")
        strHashTagList = strHashTagList.replace(" ", "")
        hashTagList = strHashTagList.split(",")

        tweetImportance = 0
        for noun in nounList:
            try:
                tweetImportance += nounImportance[noun]
            except KeyError as e:
                pass

        tweetImportances.append((tweetId, tweetImportance))
        tweetTags[tweetId] = hashTagList

    fileTarget.close()

    return tweetTags, tweetImportances

def predictHashTags(tweetId):
    tweetTags, tweetImportances = findTweetImportances()
    sortedTweetImportances = sorted(tweetImportances, key = operator.itemgetter(1), reverse = True)

    i = 0
    for tweetImportance in tweetImportances:
        if tweetImportance[0] == tweetId:
            break
        i += 1

    startIndex = i - 5
    endIndex = i + 5

    if startIndex < 0:
        startIndex = 0
        endIndex += (startIndex * (-1))
    elif startIndex > len(tweetImportances):
        startIndex -= (len(tweetImportances) - endIndex)
        endIndex = len(tweetImportances)

    averageNoOfTags = 0
    tagsInTopTweets = []
    count = 0
    for i in range(startIndex, endIndex):
        if count == 5:
            continue

        tagsInTopTweets.extend(tweetTags[sortedTweetImportances[i][0]])
        averageNoOfTags += len(tweetTags[sortedTweetImportances[i][0]])

        count += 1

    averageNoOfTags /= float(10)
    averageNoOfTags = round(averageNoOfTags)

    filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
    fileName = "topMatchingTags.txt"
    fileTarget = open(filePath + "\\" + fileName, 'w')

    dominantTagFrequencies = dict(Counter(tagsInTopTweets))
    keys = dominantTagFrequencies.keys()
    dominantTags = []
    for i in range(0, len(dominantTagFrequencies)):
        fileTarget.write(str(keys[i]) + "\t" + str(dominantTagFrequencies[keys[i]]) + "\n")
        dominantTags.append(str(keys[i]))

    fileTarget.close()

    numExtractTags = 0
    if len(dominantTags) >= averageNoOfTags:
        numExtractTags = int(averageNoOfTags)
    else:
        numExtractTags = len(dominantTags)

    suggestedTags = dominantTags[:numExtractTags]

    return suggestedTags, tweetTags

tweetId = int(raw_input(("Enter TweetId between 1001 and 1999 for which you want to obtain tags:")))
suggestedTags, tweetTags = predictHashTags(tweetId)
if tweetId > 1999 or tweetId < 1001:
    sys.exit()

print "Suggested Tags are"
print suggestedTags
print "User used Tags are"
print tweetTags[tweetId]
