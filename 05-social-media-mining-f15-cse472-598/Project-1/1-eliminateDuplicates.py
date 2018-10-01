#Author(s) - Mukund Manikarnike
import sys

def eliminateDuplicates(filename):
    userNames = []
    userIds = []
    with open(fileName, 'r') as scrapedDataFile:
        i = 0
        for line in scrapedDataFile:
            if i == 0:
                i += 1
                continue

            userString = line.split(",")
            userNames.append(str(userString[0]))
            userIds.append(str(userString[1]).strip("\r\n"))

    #Create an array to store the indices of duplicates. We can later remove
    #all but one in the duplicate list.
    duplicates = []
    i = 0
    #Traverse the entire list of userIds
    while i < len(userIds):
        j = i + 1
        #Compare every other userId with each userId in the list
        while j < len(userIds):
            if userIds[i] == userIds[j]:
                duplicates.append(j)
            j += 1
        i += 1

    if len(duplicates) == 0:
        return

    duplicates.sort()
    print duplicates

    #Now that the duplicates have been identified, the same have to be removed
    #from the input file
    i = 0
    j = 0

    scrapedDataFile = open(fileName, 'w')
    scrapedDataFile.truncate()
    scrapedDataFile.write("userName,userId\n")
    while j < len(userIds):
        if i < len(duplicates) and j == duplicates[i]:
            #Remove line number indicated by duplicates[i]
            #This is achieved by not performing a write.
            i += 1
        else:
            #Write the line into the file
            scrapedDataFile.write(str(userNames[j]) + "," + str(userIds[j]) + "\n")
        j += 1
    scrapedDataFile.close()

#Open all the scraped data files and eliminate duplicates in each of those
#files.
fileName = "scraped_data_surprise.csv"
eliminateDuplicates(fileName)
fileName = "scraped_data_glendale.csv"
eliminateDuplicates(fileName)
fileName = "scraped_data_tempe.csv"
eliminateDuplicates(fileName)
fileName = "scraped_data_chandler.csv"
eliminateDuplicates(fileName)
