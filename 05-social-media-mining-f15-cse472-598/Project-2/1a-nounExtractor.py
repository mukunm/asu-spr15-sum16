__author__ = 'Mukund Manikarnike'

from nltk.tag import pos_tag
import re

''' The following things need to be done
    1. Remove
        #Tags, @Mentions, URLs, RTs
        Numbers
        ~ ` ! $ % ^ & * ( ) _ - + = \ | { } [ ] : ; " ' , < . > ? /
    2. Convert everything to lower case

'''
def preprocessTweet(tweet):
    hashTags = []

    # Convert to lower case
    tweet = tweet.lower()

    #Extract Words
    wordsInTweet = tweet.split(" ")

    wordsToRemove = []
    for i in range(0, len(wordsInTweet)):
        if "#" in wordsInTweet[i]:
            hashTags.append(wordsInTweet[i][1:])
            wordsToRemove.append(i)
        elif "@" in wordsInTweet[i]:
            wordsToRemove.append(i)
        elif "rt" == wordsInTweet[i]:
            wordsToRemove.append(i)
        elif wordsInTweet[i].startswith("http:") or wordsInTweet[i].startswith("https:") or wordsInTweet[i].startswith("ftp:") or wordsInTweet[i].startswith("file:"):
            wordsToRemove.append(i)
        else:
            strippedWord = ''.join(ch for ch in wordsInTweet[i] if ch.isalpha())
            if strippedWord is '':
                wordsToRemove.append(i)
            else:
                wordsInTweet[i] = strippedWord

    for i in range(0, len(wordsToRemove)):
        wordsInTweet.remove(wordsInTweet[wordsToRemove[i] - i])

    processedTweet = ""
    for word in wordsInTweet:
        processedTweet += (word + " ")

    return processedTweet, hashTags

def extractNounListFromTweetFile(filePath, fileName):
    fileTarget = open(filePath + "\\" + fileName, 'r')
    fileName = "processedTweets.txt"
    processedTweetsFileTarget = open(filePath + "\\" + fileName , 'w')
    allNouns = []

    i = 0
    for line in fileTarget:
        lineContent = line.split("\t")

        userId = lineContent[0]
        tweet = lineContent[1]
        #latitude = int(lineContent[2])
        #longitude = int(lineContent[3].strip("\n"))

        processedTweet, hashTags = preprocessTweet(tweet)
        taggedTweet = pos_tag(processedTweet.split())
        nounsInTweet = [word for word, pos in taggedTweet if pos == 'NNP' or pos == 'NN']

        allNouns.extend(nounsInTweet)

        processedTweetsFileTarget.write(str(i) + "\t" + userId + "\t" + processedTweet + "\t" + str(nounsInTweet) + "\t" + str(hashTags) + "\n")

        i += 1

        if i == 2000:
            break

    fileTarget.close()
    processedTweetsFileTarget.close()

    return allNouns

filePath = "C:\\Users\\Mukund\\Documents\\CSE472-598-F15\\Dataset\\f_hashtag_prediction\\f_hashtag_prediction"
fileName = "dataForStudent.txt"
allNouns = extractNounListFromTweetFile(filePath, fileName)

print "Got all Nouns"

fileName = "nouns.txt"
nounsFileTarget = open(filePath + "\\" + fileName, 'w')
for noun in allNouns:
    nounsFileTarget.write(noun + "\n")

nounsFileTarget.close()
