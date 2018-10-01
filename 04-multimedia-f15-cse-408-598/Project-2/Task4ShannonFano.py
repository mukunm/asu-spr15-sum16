__author__ = 'PawanMahalle'

import operator, sys, numpy as np

def sum(d1):
    sum = 0
    for i in range (0, len(d1)):
        sum += d1[i][1]
    return sum

def getFrequencyDictionary(filename, symbolDictionary):

    frequencyDictionary = {}
    with open(filename) as f:
        values = []
        for line in f: # read rest of lines
            values = [int(x) for x in line.split()]
            for i in range (0, len(values)):
                key = values[i]
                if key in frequencyDictionary:
                    frequencyDictionary[key] = frequencyDictionary[key] + 1
                else:
                    frequencyDictionary[key] = 1
                    symbolDictionary[key] = ""
    print symbolDictionary
    return sorted(frequencyDictionary.items(), key=operator.itemgetter(1), reverse=True)

# compress the data from file and creates compressed file with suffix
def shannonFano(start, end, freqDict, sum, branch, symbolDictionary):

    if start == end:
        symbolDictionary[freqDict[start][0]] = branch
    else:
        freqSum = 0
        dictionaryBreak = 0
        for dictionaryBreak in range (start, end+1):
            if freqSum > sum//2:
                break
            freqSum += freqDict[dictionaryBreak][1]

        shannonFano(start, dictionaryBreak-1, freqDict, freqSum, branch+"0", symbolDictionary)
        shannonFano(dictionaryBreak, end, freqDict, sum-freqSum, branch+"1", symbolDictionary)

def compress(filename):

    symbolDictionary = {}

    frequencyDictionary = getFrequencyDictionary(filename, symbolDictionary)
    print frequencyDictionary
    shannonFano(0,len(frequencyDictionary)-1, frequencyDictionary, sum(frequencyDictionary), "", symbolDictionary)
    print symbolDictionary

    bitSeq = ""

    with open(filename) as f:
        values = []
        for line in f: # read rest of lines
            values = [int(x) for x in line.split()]
            for i in range (0, len(values)):
                bitSeq = bitSeq + symbolDictionary[values[i]]
            bitSeq += "\n"

    symbolDataInFile = ""
    for key in symbolDictionary:
        symbolDataInFile += str(symbolDictionary[key])+","+str(key)+"|"
    symbolDataInFile += "\n"

    dataInFile = symbolDataInFile + bitSeq

    if filename.endswith("tpq"):
        newFilename = filename.replace(".tpq", "_2.tpv")
    if filename.endswith("spq"):
        newFilename = filename.replace(".spq", "_2.spv")

    compressedFile = open(newFilename, 'wb')
    newByteArray = bytearray(dataInFile)
    compressedFile.write(newByteArray)
    compressedFile.close()

def decompress(filename):

    symbolDictionary = {}

    decodedData = []

    with open(filename) as f:

        decompressedFile = open(filename+".decompressed", "w")

        firstLine = True

        lineCounter = 0
        for line in f: # read rest of lines
            decompressedFileRow = []
            if firstLine == True:
                keyValuePairs = line.split("|")
                for i in range(0, len(keyValuePairs)-1):
                    symbolDictionary[keyValuePairs[i].split(",")[0]] = keyValuePairs[i].split(",")[1]
                firstLine = False
            else:
                symbol = ""
                for i in range(0, len(line)):
                    symbol += line[i]
                    if symbol in symbolDictionary.keys():
                        decompressedFile.write(symbolDictionary[symbol])
                        decompressedFile.write("\t")
                        symbol = ""
                decompressedFile.write("\n")
                print "decompressed "+str(lineCounter)
                lineCounter += 1

        decompressedFile.close()
        return filename+".decompressed"

#########################################################################################
#                                   Main                                                #
#########################################################################################

#compress('2_1_1.tpq')
#decompress("2_1_1_2.tpv")