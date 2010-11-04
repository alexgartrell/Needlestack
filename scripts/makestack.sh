#!/bin/bash

if [ $# -lt 2 ]
then
    echo "Usage: makestack.sh <stackfile> <base directory>"
    exit 1
fi

stackfile=$1
base=$2
files=

for f in `find $base`
do
    echo $f
    if [ -f $f ]
    then
        files="$files $f"
    fi
done

echo needlestack -c -f $stackfile -b $base $files