__author__ = 'Bharat'

"""
Filtering Out relevent data from Jsons
Relevent Data Includes:
1. Businesses which are restaurants.
"""

import argparse
import collections
import simplejson as json


def read_and_write_file(json_file_path, new_json_file_path):
    """Read in the json dataset file and write it out to a csv file, given the column names."""
    rests = list()
    with open(json_file_path, "r") as old:
        for line in old:
            line_contents = json.loads(line)
            #print line_contents['categories']
            if 'Restaurants' in line_contents['categories']:
                rests.append(line_contents)
                #print "True"

    json.JSONEncoder().encode(rests)
    with open(new_json_file_path, "w+") as newf:
        json.dump(rests, newf)


if __name__ == '__main__':

    parser = argparse.ArgumentParser(
            description='Makes Subset of the input JSON',
            )

    parser.add_argument(
            'json_file',
            type=str,
            help='The json file to convert.',
            )

    args = parser.parse_args()

    json_file = args.json_file
    new_json_file = '{0}_rel.json'.format(json_file.split('.json')[0])

    read_and_write_file(json_file, new_json_file)