#!/bin/bash

# defines a function named usage
function usage {
    # prints the usage of the script
    echo "$0: [input file] [output file] [pattern]"
    # exit with status code 1 (failed)
    exit 1
}

# if the first input argument is set => set ARG1 the value
# the shift command shifts the first argument to left, so the 2nd argument is now the first
# if not => call usage function
ARG1=$1; shift || usage
# because of shift the 2nd argument is now the first
# if the argument is set => set ARG2 the value and shift again,
# so the 3rd argument can be accessed by $1
# if the argument is not set => call usage function
ARG2=$1; shift || usage
# because of shift the 3rd argument is now the first
# if the argument is set => set ARG3 the value and shift again,
# if the argument is not set => call usage function
ARG3=$1; shift || usage

# grep [OPTIONS] PATTERN FILE
#   grep searchs for a pattern in a given file
#   e.g. test.txt includes string "hello world"
#       grep "world" test.txt => returns "hello world"
#   grep -n adds line numbers, in which the string stands, to the output

# search in input file for the pattern and save the output the the output file
grep -n "$ARG3" "$ARG1" > "$ARG2"

# execute without input arguments:
#	script shows usage and exits with status code 1

# execute with input arguments:
#   search in input file for the pattern and save the output the the output file