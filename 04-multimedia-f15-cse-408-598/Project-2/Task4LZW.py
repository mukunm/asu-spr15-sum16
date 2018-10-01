import lzw

infile = lzw.readbytes("3_1.spc")
compressed = lzw.compress(infile)
lzw.writebytes("3_1.spc.compressed", compressed)
infile.close()

infile = lzw.readbytes("3_1.spc.compressed")
uncompressed = lzw.decompress(infile)
lzw.writebytes("3_1.spc.decompressed", uncompressed)
infile.close()
