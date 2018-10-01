"""
Restaurant Recommendation System
Using Yelp Dataset

Sample Input:
33.402772
-111.976086
U4AAjQYM8tKs-PzHmfIp5g
"""

import numpy as np
from sklearn.neighbors import KDTree
from sklearn import preprocessing
import csv

# CSV with Business IDs and Coordinates
coordinate_file = "Features/Coordinates_Phoenix.csv"

# Put trained User_Feature_Matrix at this location
user_feature_file = "Features/User_Feature_Matrix.csv"

res_features_file = "Features/Business_Feature_Matrix.csv"
res_file = "Restaurants_Details_PHX.csv"


def get_user_feature(user_id):
    user_feature = []
    with open(user_feature_file, 'rb') as inp:
        for row in csv.reader(inp):
                if row[0] == user_id:
					user_feature = row[1:]
					break

    return user_feature


def get_res_features(res_ids):
    res_features = []

    for id in res_ids:
        found = False
        with open(res_features_file, 'rb') as inp:
            for row in csv.reader(inp):
                if row[0] == id:
                    res_features.append(row[1:])
                    found = True
                    break

            if not found:
                print id, "Not found"

    return res_features


# Get K nearest neighbors to the inputted user coordinates.
def get_knn(cod):
    # Load all Coordinates
    X = []
    res_ids = []
    with open(coordinate_file, 'rb') as inp:
        header = True
        for row in csv.reader(inp):
            if header:
                header = False
                continue
            X.append(row[1:])
            res_ids.append(row[0])

    X = np.asarray(X)

    # Finding K nearest neighbors using KD Tree
    tree = KDTree(X, leaf_size=2)
    dist, ind = tree.query(cod, k=25)

    #print dist.shape

    res_ids_dist = []
    for i in range(0, 25):
        res_ids_dist.append([res_ids[ind[0][i]], dist[0][i]])

    res_ids_dist = np.asarray(res_ids_dist)
    return res_ids_dist


# Scaling distances on the scale of 0 to 1
def scale_dist(dist):
    dist = np.asarray(dist, dtype=float)
    dist = dist.reshape((25, 1))
    minmaxscaler = preprocessing.MinMaxScaler(feature_range=(0, 1), copy=True)

    scaled_dist = minmaxscaler.fit_transform(dist)
    return scaled_dist


def display_top_res(res_ids):
    count = 1

    for (x, y) in reversed(res_ids):
        with open(res_file, 'rb') as inp:
            for row in csv.reader(inp):
                if row[0] == x:
                    print count, "-> ", row[1], "->", row[2], "\n"
                    count += 1
                    break

            if count == 11:
                break


if __name__ == '__main__':

    # User latitude and longitude as Input
    cod = [33.4, 112.0]
    cod[0] = float(raw_input("Enter the latitude: Like 33.4 something "))
    cod[1] = float(raw_input("Enter the longitude: Like -112.0 something "))

    # User ID as Input
    user_id = str(raw_input("Enter User ID:"))

    #user_feature = [1]*21
    user_feature = get_user_feature(user_id)
    user_feature = np.asarray(user_feature, dtype=float)
    user_feature = user_feature.reshape((21, 1))

    res_ids_dist = get_knn(cod)

    res_features = get_res_features(res_ids_dist[:,0])
    res_features = np.asarray(res_features, dtype=float)

    ratings = np.dot(res_features, user_feature)

    scaled_dist = scale_dist(res_ids_dist[:,1])
    ratings = ratings - scaled_dist

    res_ids_dist = zip(res_ids_dist[:,0], ratings)

    #Sort in descending order of distances
    #res_ids_dist = np.asarray(res_ids_dist)
    #res_ids_dist = res_ids_dist[np.argsort(res_ids_dist[:, 1])]
    sorted_by_second = sorted(res_ids_dist, key=lambda tup: tup[1])

    #print sorted_by_second

    display_top_res(sorted_by_second)



