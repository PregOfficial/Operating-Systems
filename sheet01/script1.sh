#!/bin/bash
# loops through the given parameters
for FN in "$@"
# start of a for loop
do
	# give owner read, write and execute right
	# give group read and execute right
	# give others no right
	chmod 0750 "$FN"
# end of a for loop
done

# execute without input arguments:
#	script does nothing

# execute with input arguments:
#	script loops through the files and directories, which are given
#	as input parameters and set the file and directory rights to 0750
