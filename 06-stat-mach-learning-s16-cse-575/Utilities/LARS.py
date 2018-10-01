__author__ = 'Bharat'

import numpy as np
from sklearn.neighbors import KDTree
from sklearn import preprocessing
import csv

cod_file = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\Coordinates_Phoenix.csv'

# Get K nearest neighbors to the inputted user coordinates.
def getKNN():
    X = []
    with open(cod_file, 'rb') as inp:
        count = 0
        for row in csv.reader(inp):
            if count == 0:
                count +=1
                continue
            X.append(row[1:])

    X = np.asarray(X)
    print X.shape

    # User latitude and longitude as Input
    cod = [0, 0]
    cod[0] = float(raw_input("Enter the latitude: Like 33.4 something "))
    cod[1] = float(raw_input("Enter the longitude: Like 112.0 something "))


    # Finding K nearest neighbors using KD Tree
    tree = KDTree(X, leaf_size=2)
    dist, ind = tree.query(cod, k=25)
    print dist
    print ind  # indices of 3 closest neighbors

    # Scaling Distance to a range of 0 to 1
    minmaxscaler = preprocessing.MinMaxScaler(feature_range=(0, 1), copy=True)
    dist = np.asarray(dist)
    scaled_dist = minmaxscaler.fit_transform(dist[0])
    print scaled_dist

# Find a sub matrix mapping business_id to latitude and longitude
def getCoordinates():
    phx_res = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\Restaurants_Phoenix.csv'

    with open(phx_res, 'rb') as inp, open(cod_file, 'wb') as out_file:
        writer = csv.writer(out_file)

        for row in csv.reader(inp):
            out = []
            out.append(row[15])
            out.append(row[66])
            out.append(row[69])
            writer.writerow(out)

    print "Done"


if __name__ == '__main__':
    #getCoordinates()
    getKNN()