
__author__ = 'Bharat'

import csv
from sets import Set


def get_phx_restaurants():
    with open('Restaurant.csv', 'rb') as inp, open('phx.csv', 'wb') as out:
        writer = csv.writer(out)
        count = 0
        for row in csv.reader(inp):
            if row[58] == 'Phoenix' or row[58] == 'Pheonix' or row[58] == 'Phoenix Sky Harbor Center' or count == 0:
                writer.writerow(row)
                count += 1


def get_phx_user():

    user_file = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\PHX_Users.csv'
    review_file = 'C:\Bharat\Masters Material\GIT Repo\RecommendationSystem\Data Files\Phoenix\Top500businessInPhx\\reviews_top_500_phx.csv'
    with open(review_file, 'rb') as inp1,open(user_file) as inp2, open('phx_users_500.csv', 'wb') as out:
        writer = csv.writer(out)

        phx_users = Set([])
        for row in csv.reader(inp1):
            print row[1]
            phx_users.add(row[1])

        count = 0
        for row in csv.reader(inp2):
            if row[16] in phx_users or count == 0:
                print row[16]
                writer.writerow(row)
                count += 1

    print count



if __name__ == '__main__':
    #get_phx_restaurants()
    get_phx_user()