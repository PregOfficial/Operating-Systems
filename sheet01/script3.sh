#!/bin/bash
if [ "$#" -ne 2 ]
then
	echo "usage: [input file] [output file]"
	exit
fi

INFILE=$1
OUTFILE=$2

if test ! -w "$OUTFILE"
then
	echo "$OUTFILE is not writable!" > error.log 
fi

if [ -e "$INFILE" ]
then
    cat "$INFILE" >> "$OUTFILE"
fi

# the script copies the content of the INFILE (1st input argument)
# and appends it to the OUTFILE (2nd input argument)
