#!/usr/bin/python

import sys

import mimetypes
import os
import struct

mimetypes.init()

def write_string_to_file(f, s):
    f.write(struct.pack('i', len(s) + 1))
    f.write(s)
    f.write(struct.pack('B', 0))

def write_file_to_file(f, path):
    size = os.path.getsize(path)
    f.write(struct.pack('i', size))

    infile = open(path, "rb")

    nwritten = 0
    while nwritten < size:
        buf = infile.read(4096)
        if len(buf) == 0: break
        f.write(buf)
        nwritten += len(buf)
    infile.close()

    return nwritten == size
    
def makestack(stackfile_path, dir):
    stackfile = open(stackfile_path, 'wb')
    for root, dirs, files in os.walk(dir):
        uri_base = ''
        if root != dir:
            uri_base = '/' + os.path.relpath(root, dir)
        for f in files:
            uri = uri_base + '/' + f
            write_string_to_file(stackfile, 'uri')
            write_string_to_file(stackfile, uri)
            path = os.path.join(root, f)
            (ctype, encoding) = mimetypes.guess_type(path)
            if ctype == None:
                tcype = 'application/octet-stream'
            write_string_to_file(stackfile, 'content-type')
            write_string_to_file(stackfile, ctype)
            write_string_to_file(stackfile, 'data')
            write_file_to_file(stackfile, path)
            write_string_to_file(stackfile, 'end_file')
    write_string_to_file(stackfile, 'end_stack')
    stackfile.close()

def main(argv):
    if len(argv) < 3:
        print "Usage: makestack.py stackfile dir"
        sys.exit(1)
    makestack(argv[1], argv[2])

if __name__ == "__main__":
    main(sys.argv)

