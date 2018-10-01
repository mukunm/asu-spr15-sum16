#Author(s) - Mukund Manikarnike
import sys

# Define a class to hold mapping of node to list to which it belongs
class UserInstance(object):
    def __init__(self, userId, anonymizedUserId, region):
        self.userId = userId
        self.anonymizedUserId = anonymizedUserId
        self.region = region

#Open Surprise and Glendale files. Read all the data from both files and
#create 3 separate arrays - S only, G only, S & G
surpriseFile = open("scraped_data_surprise.csv", 'r')
i = 0
surpriseUserNames = []
surpriseUserIds = []
for line in surpriseFile:
    if i == 0:
        i += 1
        continue

    userString = line.split(",")
    surpriseUserNames.append(str(userString[0]))
    surpriseUserIds.append(str(userString[1]).strip("\r\n"))

#Reading from Glendale file
glendaleFile = open("scraped_data_glendale.csv", 'r')
i = 0
glendaleUserNames = []
glendaleUserIds = []
for line in glendaleFile:
    if i == 0:
        i += 1
        continue

    userString = line.split(",")
    glendaleUserNames.append(str(userString[0]))
    glendaleUserIds.append(str(userString[1]).strip("\r\n"))

surpriseFile.close();
glendaleFile.close();

print "SLen: " + str(len(surpriseUserIds))
print "GLen: " + str(len(glendaleUserIds))

# Create a common array for both Glendale and Surprise and remove the common
# ones from either of these lists.
k = 0
i = 0
surpriseAndGlendaleUserNames = []
surpriseAndGlendaleUserIds = []

while i < len(surpriseUserIds):
    j = 0
    while j < len(glendaleUserIds):
        if surpriseUserIds[i] == glendaleUserIds[j]:
            # Add to common list
            surpriseAndGlendaleUserNames.append(surpriseUserNames[i])
            surpriseAndGlendaleUserIds.append(surpriseUserIds[i])

            # Remove from both lists
            surpriseUserNames.remove(surpriseUserNames[i])
            surpriseUserIds.remove(surpriseUserIds[i])
            glendaleUserNames.remove(glendaleUserNames[j])
            glendaleUserIds.remove(glendaleUserIds[j])

            #After doing this, we can break because we'll only find one duplicate
            #at max because this script is going to be run after running the
            #eliminate_duplicates script. This proceed to comparing the next
            #user in surprise with all users in glendale
            break
        j += 1
    i += 1

print "New SLen: " + str(len(surpriseUserIds))
print "New GLen: " + str(len(glendaleUserIds))
print "S&GLen: " + str(len(surpriseAndGlendaleUserIds))

#Open the list of users in tempe and perform a similar comparison with Glendale
tempeFile = open("scraped_data_tempe.csv", 'r')
i = 0
tempeUserNames = []
tempeUserIds = []
for line in tempeFile:
    if i == 0:
        i += 1
        continue

    userString = line.split(",")
    tempeUserNames.append(str(userString[0]))
    tempeUserIds.append(str(userString[1]).strip("\r\n"))

print "TLen: " + str(len(tempeUserIds))

# Create a common array for both Glendale and Tempe and remove the common
# ones from either of these lists.
k = 0
i = 0
tempeAndGlendaleUserNames = []
tempeAndGlendaleUserIds = []

while i < len(tempeUserIds):
    j = 0
    while j < len(glendaleUserIds):
        if tempeUserIds[i] == glendaleUserIds[j]:
            # Add to common list
            tempeAndGlendaleUserNames.append(tempeUserNames[i])
            tempeAndGlendaleUserIds.append(tempeUserIds[i])

            # Remove from both lists
            tempeUserNames.remove(tempeUserNames[i])
            tempeUserIds.remove(tempeUserIds[i])
            glendaleUserNames.remove(glendaleUserNames[j])
            glendaleUserIds.remove(glendaleUserIds[j])

            #After doing this, we can break because we'll only find one duplicate
            #at max because this script is going to be run after running the
            #eliminate_duplicates script. This proceed to comparing the next
            #user in surprise with all users in glendale
            break
        j += 1
    i += 1

print "New TLen: " + str(len(tempeUserIds))
print "New1 GLen: " + str(len(glendaleUserIds))
print "T&GLen: " + str(len(tempeAndGlendaleUserIds))

#Open the list of users in Chandler and perform a similar comparison with Tempe
chandlerFile = open("scraped_data_chandler.csv", 'r')
i = 0
chandlerUserNames = []
chandlerUserIds = []
for line in chandlerFile:
    if i == 0:
        i += 1
        continue

    userString = line.split(",")
    chandlerUserNames.append(str(userString[0]))
    chandlerUserIds.append(str(userString[1]).strip("\r\n"))

print "CLen: " + str(len(chandlerUserIds))

# Create a common array for both Chandler and Tempe and remove the common
# ones from either of these lists.
k = 0
i = 0
tempeAndChandlerUserNames = []
tempeAndChandlerUserIds = []

while i < len(tempeUserIds):
    j = 0
    while j < len(chandlerUserIds):
        if tempeUserIds[i] == chandlerUserIds[j]:
            # Add to common list
            tempeAndChandlerUserNames.append(tempeUserNames[i])
            tempeAndChandlerUserIds.append(tempeUserIds[i])

            # Remove from both lists
            tempeUserNames.remove(tempeUserNames[i])
            tempeUserIds.remove(tempeUserIds[i])
            chandlerUserNames.remove(chandlerUserNames[j])
            chandlerUserIds.remove(chandlerUserIds[j])

            #After doing this, we can break because we'll only find one duplicate
            #at max because this script is going to be run after running the
            #eliminate_duplicates script. This proceed to comparing the next
            #user in surprise with all users in glendale
            break
        j += 1
    i += 1

print "New CLen: " + str(len(chandlerUserIds))
print "New1 TLen: " + str(len(tempeUserIds))
print "T&CLen: " + str(len(tempeAndChandlerUserIds))
#sys.exit()

# Merge all the usersIds into one big list
userInstanceList = []
i = 0
while i < len(surpriseUserIds):
    userInstanceList.append(UserInstance(surpriseUserIds[i], (i + 1), "Surprise"))
    i += 1
surpriseEnds = i

while i < len(surpriseAndGlendaleUserIds) + surpriseEnds:
    userInstanceList.append(UserInstance(surpriseAndGlendaleUserIds[i - surpriseEnds], (i + 1), "SurpriseAndGlendale"))
    i += 1
surpriseAndGlendaleEnds = i

while i < len(glendaleUserIds) + surpriseAndGlendaleEnds:
    userInstanceList.append(UserInstance(glendaleUserIds[i - surpriseAndGlendaleEnds], (i + 1), "Glendale"))
    i += 1
glendaleEnds = i

while i < len(tempeAndGlendaleUserIds) + glendaleEnds:
    userInstanceList.append(UserInstance(tempeAndGlendaleUserIds[i - glendaleEnds], (i + 1), "TempeAndGlendale"))
    i += 1
tempeAndGlendaleEnds = i

while i < len(tempeUserIds) + tempeAndGlendaleEnds:
    userInstanceList.append(UserInstance(tempeUserIds[i - tempeAndGlendaleEnds], (i + 1), "Tempe"))
    i += 1
tempeEnds = i

while i < len(tempeAndChandlerUserIds) + tempeEnds:
    userInstanceList.append(UserInstance(tempeAndChandlerUserIds[i - tempeEnds], (i + 1), "TempeAndChandler"))
    i += 1
tempeAndChandlerEnds = i

while i < len(chandlerUserIds) + tempeAndChandlerEnds:
    userInstanceList.append(UserInstance(chandlerUserIds[i - tempeAndChandlerEnds], (i + 1), "Chandler"))
    i += 1
chandlerEnds = i

userListFile = open("user_list.csv", 'w')
userListFile.truncate()
userListFile.write("Number of users found," + str(len(userInstanceList)) + "\n")
userListFile.write("UserId,AnonymizedId,Region\n")

# Creating the adjacency matrix
adjacencyMatrix = [[0 for x in range(len(userInstanceList))] for x in range(len(userInstanceList))]
i = 0
while i < len(userInstanceList):
    #Print the user list and the anonymized User list
    userListFile.write(str(userInstanceList[i].userId) + "," + str(userInstanceList[i].anonymizedUserId) + "," + str(userInstanceList[i].region) + "\n")
    region = userInstanceList[i].region

    if region == "Surprise":
        j = 0
        while j < surpriseAndGlendaleEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    elif region == "SupriseAndGlendale":
        j = 0
        while j < glendaleEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    elif region == "Glendale":
        j = surpriseEnds
        while j < tempeAndGlendaleEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    elif region == "TempeAndGlendale":
        j = surpriseAndGlendaleEnds
        while j < tempeEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    elif region == "Tempe":
        j = glendaleEnds
        while j < tempeAndChandlerEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    elif region == "TempeAndChandler":
        j = tempeAndGlendaleEnds
        while j < chandlerEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    elif region == "Chandler":
        j = tempeEnds
        while j < chandlerEnds and j != i:
            adjacencyMatrix[i][j] = 1
            j += 1
    i += 1

userListFile.close()

#Print the edge list and the anonymized edge list
i = 0
edgeListFile = open("edge_list.csv", 'w')
edgeListFile.write("Source,Destination\n")
while i < len(adjacencyMatrix):
    j = 0
    while j < len(adjacencyMatrix[i]):
        if adjacencyMatrix[i][j] == 1:
            edgeListFile.write(str(userInstanceList[i].userId) + "," + str(userInstanceList[j].userId) + "\n")
        j += 1
    i += 1
edgeListFile.close()

i = 0
anonymizedEdgeListFile = open("anonymized_edge_list.csv", 'w')
anonymizedEdgeListFile.write("Source,Destination\n")
while i < len(adjacencyMatrix):
    j = 0
    while j < len(adjacencyMatrix[i]):
        if adjacencyMatrix[i][j] == 1:
            anonymizedEdgeListFile.write(str(userInstanceList[i].anonymizedUserId) + "," + str(userInstanceList[j].anonymizedUserId) + "\n")
        j += 1
    i += 1
anonymizedEdgeListFile.close()

layout = graph.layout("kk")

adjacencyMatrixFile = open("adjacency_matrix.txt", 'w')
i = 0
while i < len(adjacencyMatrix):
    j = 0
    while j < len(adjacencyMatrix[i]):
        if j == len(adjacencyMatrix[i]) - 1:
            adjacencyMatrixFile.write(str(adjacencyMatrix[i][j]) + "\n")
        else:
            adjacencyMatrixFile.write(str(adjacencyMatrix[i][j]) + ", ")
        j += 1
    i += 1
adjacencyMatrixFile.close()
