from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import chi2
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestRegressor
from sklearn.feature_selection import SelectFromModel
from collections import OrderedDict
import csv
clf = RandomForestRegressor()
RESTR_DENSE_FILE_PATH = 'restaurants_dense.csv'
FEATURE_FILE_PATH = 'feature_matrix.csv'
features_df = pd.read_csv(RESTR_DENSE_FILE_PATH)
#features_df.info()
business_ids= features_df['business_id'].tolist()
df_num = features_df.select_dtypes(include=[np.float])
all_features_num_df = df_num.fillna(df_num.mean())
features_num_df = all_features_num_df.drop(['latitude', 'longitude'], axis=1)
#header = features_num_df.columns.tolist()
X_df = features_num_df.drop('stars', axis=1)
X = X_df.as_matrix()
header = X_df.columns.tolist()
y_df = features_num_df['stars']
y = y_df.as_matrix()
print "X Shape {}".format(X.shape)
#X_new = SelectKBest(chi2, k=5).fit_transform(X, y)
clf = clf.fit(X, y)
print "Important features "
restr_map = {}
for key, value in zip(header, clf.feature_importances_):
    restr_map[key] = value

print clf.feature_importances_
model = SelectFromModel(clf, prefit=True)
X_new = model.transform(X)

print X_new
print "New Shape: {}".format(X_new.shape)

with open(FEATURE_FILE_PATH, 'wb') as outfile:
    writer = csv.writer(outfile)
    new_features = X_new.tolist()
    rows = []
    for id in range(len(business_ids)):
        rows.append([business_ids[id]] + new_features[id])
    writer.writerows(rows)