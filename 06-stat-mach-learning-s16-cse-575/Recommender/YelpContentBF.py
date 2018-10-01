# Creating a matrix User x Business for content based filtering
# Dimensions - 64968 * 2926
import pandas as pd
import numpy as np
import csv
from sets import Set

# Storing all Phoenix-specific reviews
df = pd.read_csv('C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\Top500businessInPhx\\reviews_top_500_phx.csv')

# 1. Creating a dictionary of Phoenix restaurants
phx_business = open('C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\Top500businessInPhx\\restaurants_top_500_phx.csv')
phx_csv = csv.reader(phx_business)
phx_data = list(phx_csv)
restr_dict = {}

# Dictionary format ['business_id', matrix column number]
i = 0
while i < len(phx_data):
    restr_dict[phx_data[i][0]] = i
    i = i + 1
print "Total no. of unique restaurants = %d" % len(restr_dict)

# 2. Creating a dictionary of Phoenix users
user_dict = {}
first = False
j = 0
# Dictionary format ['user_id', matrix row number]
for index, row in df.iterrows():
    #ignoring 1st row
    if first is False:
        first = True
        continue
    if row[1] not in user_dict:
        user_dict[row[1]] = j
        j = j + 1
print "Total no. of unique users = %d" % len(user_dict)
# A sample entry
# print user_dict['t95D1tnWvAOy2sxXnI3GUA']

x = int(len(user_dict))
y = int(len(restr_dict))
#users_restr = np.zeros((int(len(user_dict)),int(len(restr_dict))))
users_restr = np.random.randint(1, size=(x, y))
print users_restr.shape

# 4. Run through Phoenix reviews, create the matrix
for index, row in df.iterrows():
    users_restr[int(user_dict[row[1]]) , int(restr_dict[row[2]])] = row[3]

#print users_restr[2]

# 5. Creating a CSV file of the matrix
# takes ~ 4GB
np.savetxt("user_restr_matrix.csv", users_restr, delimiter=",")