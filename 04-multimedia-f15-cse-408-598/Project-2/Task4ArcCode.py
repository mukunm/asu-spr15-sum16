__author__ = 'aaron'
import arcode
import bitfile
import sys

#Used python package modules arcode 0.1 and Bitfile
#https://pypi.python.org/pypi/arcode/0.1
#http://michael.dipperstein.com/bitlibs/
def SelectCompression():
    print("Press 1 for Compression"
      "\nPress 2 for Variable Length Encoding"
      "\nPress 3 for LZW Encoding"
      "\nPress 4 for Arithmetic Encoding")
    n = input("Choice: ")

    if n == 1:
        print("\n###Selected 1 for No Compression###")
    elif n == 2:
        print("\n###Selected 2 for Variable Length Encoding###")
    elif n == 3:
        print("\n###Selected 3 for LZW Encoding###")
    elif n == 4:
        print("\n###Selected 4 for Arithmetic Encoding###")
        #ArcEncode
        print "Enter filename for encode and decode: "
        input_file = str(raw_input())
        output_file = input_file + 'ACencode.tpv'
        print "encoded file saved as:", output_file
        ar = arcode.ArithmeticCode(use_static_model)
        ar.encode_file(input_file, output_file)

        #ArCdecode
        input_file1 = output_file
        output_file2 = 'ACdecode.tpv'
        print "decoded file saved as:", output_file2
        ar = arcode.ArithmeticCode(use_static_model)
        ar.decode_file(input_file1, output_file2)
    else:
        print("\n###Exited###")
        sys.exit()
encode = None
use_static_model = True
SelectCompression()