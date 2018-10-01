__author__ = 'Bharat'


import csv

cus = dict([])
cus_file = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\cuisine_names.csv'
res_file = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\\restaurants_cuisines_phx.csv'

with open(cus_file, 'rb') as inp:
    for row in csv.reader(inp):
        cus[row[0]] = 0

#print cus

with open(res_file, 'rb') as inp2:
    for row in csv.reader(inp2):
        if row[1] in cus:
            cus[row[1]] += 1

final_cus = dict([])

count = 1
for key, value in cus.iteritems():
    if value >= 59:
        final_cus[key] = count
        count += 1
        #final_cus.add(key)

print len(final_cus)

out_file = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Utilities\\cuisine_matrix.csv'

with open(out_file, 'wb') as out:
    writer = csv.writer(out)
    row = ['B ID']

    for key in final_cus:
        row.append(key)

    writer.writerow(row)

    with open(res_file, 'rb') as inp2:
        row = [0 for i in range(0, 22)]

        res = ''

        count = 0
        for row_inp in csv.reader(inp2):
            if count == 0:
                count +=1
                continue

            if res != row_inp[0]:
                if row[0] != 0:
                    writer.writerow(row)

                row = [0 for i in range(0, 22)]
                res = row_inp[0]
                row[0] = res

            if row_inp[1] in final_cus:
                row[final_cus[row_inp[1]]] = 1

    #print row

