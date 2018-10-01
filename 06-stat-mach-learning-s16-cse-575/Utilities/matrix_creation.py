
# coding: utf-8

# In[17]:

# Creating a matrix User x Business for content based filtering
# Dimensions - 64968 * 2926
import pandas as pd
import numpy as np
import csv

# Storing all Phoenix-specific reviews
df = pd.read_csv('phx_rating.csv')

print df.head()


# In[2]:

# 1. Creating a dictionary of Phoenix restaurants
phx_business = open('../Data files/Phoenix/Restaurants_Phoenix.csv')
phx_csv = csv.reader(phx_business)
phx_data = list(phx_csv)
restr_dict = {}
# Dictionary format ['business_id', matrix column number]
i = 0 
# Corrected to ignore header
header = True
while i < len(phx_data) - 1:
    if header:
        header = False
        continue
    restr_dict[phx_data[i + 1][15]] = i
    i = i + 1
print "Total no. of unique restaurants = %d" % len(restr_dict)


# In[15]:

# Sanity check: Should throw error
restr_dict['=-h-q6zTIdPlkz9BDP11sBg']
# restr_dict['#NAME?']


# In[4]:

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
    if row[0] not in user_dict:
        user_dict[row[0]] = j
        j = j + 1    
print "Total no. of unique users = %d" % len(user_dict)
# A sample entry
# print user_dict['t95D1tnWvAOy2sxXnI3GUA']


# In[5]:

# 3. Create a numpy matrix

users_restr = np.zeros((int(len(user_dict)),int(len(restr_dict))))
print users_restr.shape


# In[18]:

# 4. Run through Phoenix reviews, create the matrix
for index, row in df.iterrows():
    try:
        users_restr[int(user_dict[row[0]]) , int(restr_dict[row[2]])] = row[3]
    except Exception as e:
        print e
        continue


# In[20]:

print users_restr.shape


# In[39]:

print len(users_restr[64967])
print type(users_restr[0][0])



# In[40]:

# 5. Creating a CSV file of the matrix
# takes ~ 4GB
np.savetxt("user_restr_matrix.csv", users_restr, delimiter=",") #Throwing error


# In[38]:

# 5. Creating CSV file using csv writer

outfile = open('user_restr_mat_csv.csv', 'wb+')
csvwriter = csv.writer(outfile)
csvwriter.writerows(users_restr)
# for row in users_restr:
#     csvwriter.writerow(row)


# In[24]:

# Creating the business feature matrix.
# 1. Sort the business_id based on position in numpy array
import operator

ordered_restr_dict = sorted(restr_dict.items(), key=operator.itemgetter(1))


# In[25]:

ordered_restr_dict


# In[28]:

# 2. Build a dictionary with business_id as key

# Tried with csv to load, failed for some values
# phx_business_dense = open('../Data files/Phoenix/restaurants_dense.csv')
# reader = csv.reader(phx_business_dense)

# #Skip header
# reader.next()
# # restr_ dict {'business_id': All dense features(Entire row)}
# restr_dict = {}

# for row in reader:
#     #row[5] is business_id
#     restr_dict[row[5]] = row


# Trying with dataframe

import pandas as pd

business_feature_df = pd.read_csv('../Data files/Phoenix/restaurants_dense.csv')


# In[29]:

business_feature_df.head()
# estr_ dict {'business_id': All dense features(Entire row)}
restr_dict = {}
for idx, row in business_feature_df.iterrows():
    restr_dict[row['business_id']] = row


# In[30]:

print len(restr_dict.items())
# print business_feature_df['business_id'][22]

# 2878 which is less than the total number of restaurants
# Missed ids (hash collisions?)
"""
'-lOSaCuBRAvX5JBifx-EMw'
'-AAig9FG0s8gYE4f8GfowQ'
'-S7aL8dVIiXjCdbTZVn8uA'
'-yPdEze6bYRV2Sm1t4XJhg'
'-SS0C3OoPhVVBFCFfDWB2A'
'-nHYKkSJuQ7zzFZ1veRfPg'
'-KZA7UoULw_pM3jBBUzb8A'
'-d1I0dwtCg1IIjnI3DZ38Q'
'-Ogv7rpcgUHkFaSy3vD8Sw'
'-gefwOTDqW9HWGDvWBPSMQ'
'-mz0Zr0Dw6ZASg7_ah1R8A'
'-KEU36ohRQb19mrbA65Y3Q'
'-yzl8Zm-MmSqx31VN7vB0Q'
'-yxfBYGB6SEqszmxJxd97A'
'-zcZNlO0JwZHppan8rGkBQ'
'-hzkQ8YIkExQse3vX0HOcg'
'-sHaV6At__T1RJQXiR6d0g'
'-xBv8p9jzOkMyGlY07FMTA'
'-f5EhKQb7jPtDl1eYDwV0w'
'-WZIxGXJHMGidZXRhKxP3w'
'-_npP9XdyzILAjtFfX8UAQ'
'-ftQeUsqwDkExRg6IYrubQ'
'-XlBQrxN_ZB3MZKxLBeAjA'
'-sC66z4SO3tR7nFCjfQwuQ'
'-JpZiiGPKOuCEiODGNyovw'
'-aDuOZelvBHWKh-NrXooow'
'-szHsdbd5-J0by2d62T3Hg'
'-q_VodbABJygOSuv86LOtw'
'-vaGR2CXNS4NZrMdctSkGA'
'-h-q6zTIdPlkz9BDP11sBg'
'-w0g2HkH2Ncxbs_QDtsf-A'
'-NOet8xJLGhX-eUzSIDc2Q'
'-dy88UgxJPIAkgVPkoNU-w'
'-UT6IHfVW_2yzz1bf8WI5g'
'-HnNSLlmNbesJ7NF5CmgFg'
'-fA9y44FrK8nTRxjyntfTw'
'-TulfYOMTsrqVVvjJRk4Fg'
'-yTdxWCadi8Kn1H05getKQ'
'-MaIpPCz4153Uh1pnzI3AA'
'-hWBlyI2k95yjU-cgwCKJg'
'-Nc6vbO6nXoefulN1Knl9A'
'-PF0u3x21vsTjmHfEFRQhg'
'-tKlJDgHrJDkU7EL9c8f3A'
'-PyniZNMQuB5u99nr6dYVw'
'-K5a0_06H7FYRqqiREHjtA'
'-jbHH3mB9SsSDb8nOGvMSw'
'-Dpp6uTdNmVCrX5GGJhk1w'
'-_jLCD1NWODEXfgEAKfUAg'
"""


# In[31]:

# Doing some sanity check.

import pandas as pd

restr_df = pd.read_csv('../Data files/Phoenix/restaurants_dense.csv')

print "Number of records {}".format(len(restr_df['business_id']))

print "Number of unique records {}".format(len(set(restr_df['business_id'])))



# In[32]:

# 3. Constructing the feature matrix in the order the businesses appear in user_restr_matrix.csv

def find_features(business_id):
    phx_business_dense = open('../Data files/Phoenix/restaurants_dense.csv')
    reader = csv.reader(phx_business_dense)
    #Skip header
    reader.next()
    for row in reader:
        if row[5] == business_id:
            return [toInt(row[1]),toInt(row[2]),toInt(row[3]),
                    toInt(row[4]),toInt(row[8]),toInt(row[11])]

def toInt(x):
    try:
        return int(x)
    except:
        # What to do if value is missing. Impute?
        return -1
    

restr_feature_vector = []
# Here restaurant = (business_id, idx + 1)
for restaurant in ordered_restr_dict:
    # first record is header
    if restaurant[0] == 'business_id': continue
    try:
        all_features = restr_dict[restaurant[0]]
        features = [toInt(all_features[1]),toInt(all_features[2]),toInt(all_features[3]),
                    toInt(all_features[4]),toInt(all_features[8]),toInt(all_features[11])]
        restr_feature_vector.append(features)
    except Exception as e:
        print e
#         features = find_features(restaurant[0])
#         if features is None:
#             print restaurant[0]
#         restr_feature_vector.append(features)

        
restr_np_array = np.asarray(restr_feature_vector)


# In[33]:

print restr_np_array.shape


# In[34]:

# 4. Save the ordered restaurant feature vector

np.savetxt("restr_feature_matrix.csv", restr_np_array, delimiter=",")


# In[ ]:



