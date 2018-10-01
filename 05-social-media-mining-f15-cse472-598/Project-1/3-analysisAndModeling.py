#Author(s) - Mukund Manikarnike

from igraph import *
from random import randint
import scipy
import scipy.stats

#Read from the anonymized edge list that was created
anonymizedEdgeListFile = open("anonymized_edge_list.csv", 'r')

#Create a graph object for the graph that has been created
graph = Graph()

graph.add_vertices(1517)
edges = []
i = 0
for line in anonymizedEdgeListFile.xreadlines():
    if i == 0:
        i += 1
        continue

    edgeString = line.split(",")
    edgeSource = int(str(edgeString[0]))
    edgeDest = int(str(edgeString[1]).strip("\r\n"))
    edge = (edgeSource, edgeDest)
    edges.append(edge)

    i += 1

#Add all edges to the graph
graph.add_edges(edges)

#Compute a power law version of the graph with exponent = 2
newGraph = graph.Static_Power_Law(1517, 547873, 2)

#Display the degree distribution of the oringal graph
print "Plotting distribution"
plot(graph.degree_distribution(), "DegreeDistribution.pdf")

#Display the degree distribution of the power Law graph
print "Power Law Deg Dist."
plot(newGraph.degree_distribution(), "PowerLawDegreeDistribution.pdf")

print "--------Created Graph Stats---------"
print "Diamter: " + str(graph.diameter())
print "The created graph doesn't exhibit a power law because the graph was"
print "based on the location of the users and not actual connections."
print "Bridges: " + str(len(graph.cut_vertices())/2)
print "Tempe, Chandler, Glendale and Surprise have common users and each of them are connected."

print "-------Power Law Graph Stats--------"
print "Diameter: " + str(newGraph.diameter())
print "Bridges: " + str(len(newGraph.cut_vertices())/2)

x = 1
while x < 100:
    print str(x) + "percent of edges being removed"
    tempGraph = newGraph
    numEdgesToRemove = 547873 * (x / 100)
    i = 0
    edgesToRemove = []
    while i < numEdgesToRemove:
        rand1 = randint(1, 1516)
        rand2 = randint(1, 1516)
        edgeToRemove = (rand1, rand2)
        edgesToRemove.append(edgeToRemove)
        i += 1

    #Removing edges
    tempGraph.delete_edges(edgesToRemove)

    #Computing size of largest connected component
    sizes = tempGraph.components().sizes()
    size = max(sizes)
    print "Size of component: " + str(size)
    print "===================================="
    x += 1

print "=====================Network Measures============="
print "Average Local Clustering Coefficient: " + str(newGraph.transitivity_avglocal_undirected())
print "Global Clustering Coefficient: " + str(newGraph.transitivity_undirected())
print "Average Path Length: " + str(newGraph.average_path_length(directed = False))

#Compute PageRank
pageRanks = newGraph.pagerank()
pageRanks.sort(reverse = True)

i = 0
while i < 10:
    print "pageRank No. " + str(i) + ":" + str(pageRanks[i])
    i += 1

#Compute EigenVector Centralities
eigenVectorCent = newGraph.evcent(directed = False)
eigenVectorCent.sort(reverse = True)

i = 0
while i < 10:
    print "eigen vector centrality No. " + str(i) + ": " + str(eigenVectorCent[i])
    i += 1

#Degree centrality
degrees = newGraph.degree()
degrees.sort(reverse = True)

i = 0
sum1 = 0
while i < len(degrees):
    sum1 += degrees[i]
    i += 1
avg = sum1/len(degrees)

print "Average degree: " + str(avg)

i = 0
while i < 10:
    print "degree centrality No. " + str(i) + ": " + str(degrees[i])
    i += 1

#Rank Correlation
pPageRankEigen = scipy.stats.pearsonr(pageRanks, eigenVectorCent)
pPageRankDegree = scipy.stats.pearsonr(pageRanks, degrees)
pEigenDegree = scipy.stats.pearsonr(eigenVectorCent, degrees)

print "Rank Correlation between pageRank and EigenVector: " + str(pPageRankEigen)
print "Rank Correlation between pageRank and degree: " + str(pPageRankDegree)
print "Rank Correlation between EigenVector and degree: " + str(pEigenDegree)

#Jaccard Similarity
jaccards = newGraph.similarity_jaccard(loops = False)

i = 0
max1 = 0
while i < len(jaccards):
    j = 1
    while j < len(jaccards[i]):
        if jaccards[i][j] > max1 and jaccards[i][j] != 1:
            max1 = jaccards[i][j]
        j += 1
    i += 1

print "Highest Similarity is " + str(max1)


print "===============Network Models=============="
#Simulate a random graph
print "Random Graph"
randomGraph = Graph.Erdos_Renyi(n=1517, m=547873)
print "Global Clustering Coefficient: " + str(randomGraph.transitivity_undirected())
print "Average Path Length: " + str(randomGraph.average_path_length(directed = False))
print "Plot Degree Distribution for random graph"
plot(randomGraph.degree_distribution(), "PowerLawDegreeDistribution-RandomGraph.pdf")

print "Small World"
smallWorldGraph = newGraph
#Simulate a small world graph
p = 0.75 * ((avg - 2)/(avg - 1))
dim = 1
size = 1517
nei = 2
smallWorldGraph = Graph.Watts_Strogatz(dim, size, nei, p)
print "Global Clustering Coefficient: " + str(smallWorldGraph.transitivity_undirected())
print "Average Path Length: " + str(smallWorldGraph.average_path_length(directed = False))
print "Plot Degree Distribution for small world graph"
plot(smallWorldGraph.degree_distribution(), "PowerLawDegreeDistribution-SmallWorldGraph.pdf")


print "PreferentialAttachment"
prefGraph = newGraph
#Simulate a preferential attachment graph
nei = newGraph.average_path_length(directed = False)
prefGraph = Graph.Barabasi(1517, 547873)
print "Global Clustering Coefficient: " + str(prefGraph.transitivity_undirected())
print "Average Path Length: " + str(prefGraph.average_path_length(directed = False))
print "Plot Degree Distribution for Preferential Attachment graph"
plot(prefGraph.degree_distribution(), "PowerLawDegreeDistribution-PreferentialAttachmetGraph.pdf")
