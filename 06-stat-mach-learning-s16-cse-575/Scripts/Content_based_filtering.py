
# coding: utf-8

# In[ ]:

# All imports go here

import numpy as np
from numpy import genfromtxt
import csv
from sklearn import linear_model
from sklearn import preprocessing
from sklearn.metrics import mean_squared_error
from math import sqrt
from collections import OrderedDict

USER_RESTR_MATRIX_PATH = '../../Data/Input/User_Restr_Matrix.csv'
BUSINESS_FEATURE_MATRIX_PATH = '../../Data/Business_Feature_Matrix.csv'
PHX_RATING = '../../Data/Input/Phx_Rating.csv'
USER_PARAMS_PATH = '../../Data/User_Params.csv'


# In[ ]:

# Step 1 : Loading the User x Restaurant matrix. Each element is a rating

print "Loading User x Restaurant matrix"
Y = genfromtxt(USER_RESTR_MATRIX_PATH, delimiter=',')
print "Success! Shape of Y: {}".format(Y.shape)


# In[ ]:

# Step 2 : Loading the Restaurant feature matrix

print "Loading User x Restaurant matrix"
X = genfromtxt(BUSINESS_FEATURE_MATRIX_PATH, delimiter=',')
print "Success! Shape of Y: {}".format(X.shape)


# In[ ]:

# Separating in validation and test.

x, y = int(Y.shape[0]), int(Y.shape[1])
count = 0
idx = []
actual = []
print "Splitting into validation and test"
for i in range(0, x):
    for j in range(0, y):
        if r[i, j] != 0:
            count += 1
            if count%5 == 0:
        idx.append([i, j])
        actual.append(Y[i,j])
        Y[i,j] = 0

print "Done"


# In[ ]:

# Imputing mean values for missing features
print "Imputing missing values"

for i in range(X.shape[1]):
    tmp = X[:,i][X[:,i] != -1]
    feature_mean = sum(tmp)/len(tmp)
    X[:,i][X[:,i] == -1] = feature_mean


# In[ ]:

# Learning user parameters for the features
print "Learning user params"
user_params = []
model = linear_model.SGDRegressor()
for i in range(int(Y.shape[0])):
    model.fit(X[0], Y[i])
    user_params.append(np.asarray(model.coef_))
    
user_params =  np.asarray(user_params)
print "Printing user params for user 0"
print user_params[0]


# In[ ]:

# Creating a dictionary of Phoenix users
df = pd.read_csv(PHX_RATING)
user_dict = {}
first = False
j = 0

# Dictionary format ['user_id', matrix row number]
print "Creating User Dictionary"
for index, row in df.iterrows():
    #ignoring 1st row
    if first is False:
        first = True
        continue
    if row[0] not in user_dict:
        user_dict[j] = row[0]
        j = j + 1    

print "Completed: Total no. of unique users = %d" % len(user_dict)        


# In[ ]:

# Converting to ordered dict
ordered_user_dict = OrderedDict(sorted(user_dict.items(), key=lambda t: t[0]))


# In[ ]:

# Saving user params to file

with open(USER_PARAMS_PATH) as user_params_file:
    user_params_writer = csv.writer(user_params_file)
    row_num = 0
    all_rows = []
    for row in user_params.tolist():
        all_rows.append(ordered_user_dict[row_num] + row)
    print "Saving user params to file"
    user_params_writer.writerows(all_rows)
print "Done"


# In[ ]:

# Scale the params between (0-5)

print "Scale the params between (0-5)"
minmaxscaler = preprocessing.MinMaxScaler(feature_range=(0, 5), copy=True)
scaled_params = []
for i in range(len(user_params)):
    temp = user_params[i]
    temp = np.array(temp).reshape((len(temp), 1))
    scaled_params.append(minmaxscaler.fit_transform(temp).reshape(len(user_params[i],)))
scaled_params = np.asarray(scaled_params)

#print "Scaled_params for first user"
#print scaled_params[0]


# In[ ]:

# Calculate the predicted ratings
predicted_ratings= scaled_params.dot(X.T)

print "Predicted ratings for first user"
print predicted_ratings[0]


# In[ ]:

# Scale the predicted ratings just in case they go above (0-5)

scaled_ratings = []
for i in range(len(predicted_ratings)):
    # Passing 1D array throws warning, hence reshaping the array
    temp = predicted_ratings[i]
    temp = np.array(temp).reshape((len(temp), 1))
    scaled_ratings.append(minmaxscaler.fit_transform(temp).reshape(len(predicted_ratings[i],)))


# In[ ]:

def getRoundedThreshold(a, MinClip=0.5):
    return np.round(np.array(a, dtype=float) / MinClip) * MinClip

scaled_ratings = np.asarray(scaled_ratings)
rounded_ratings = getRoundedThreshold(scaled_ratings)
predicted = []
predicted2 = []
for i,j in idx:
    predicted2.append(rounded_ratings[i, j])
    predicted.append(scaled_ratings[i, j])


# In[ ]:

print "Actual Ratings: {}".format(actual[:5])
print "Predicted Ratings: {}".format(predicted[:5])


# In[ ]:

# Calculate RMSE

rms = sqrt(mean_squared_error(actual, predicted))
rms2 = sqrt(mean_squared_error(actual, predicted2))
print rms
print rms2

