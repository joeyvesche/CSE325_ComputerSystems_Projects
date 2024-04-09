#!/bin/bash         
# first line of any bash script file should be "#!/bin/bash"
# btw bash is an acronym for "Bourne Again Shell"

num_param=1

# command-line parameters are designated by a '$' followed immediately by 
# a number indicating the parameter's relative position.
#
# $1 --> number (0, 1, or 2)
if [ $# -ne $num_param ]
then      
	#    Let user know what is acceptable usage
	echo ""
	echo ""
	echo "Usage:"
	echo " run.sh <number>"
	echo " Where:"
	echo "    <number> = number of process (0, 1, or 2)"
	echo " "
else
	# conduct the experiments by repeatedly invoking cache-perf
	tracefile=trace$1.txt
	
	rm -rf trace$1.txt
	rm -rf trace.txt     # If a copy of the output file exists, delete it.
                    # If the file is not deleted the succeeding output will be concatenated
                    # to the existing file.

	number=$1

	./lab2 $number $tracefile

if [ $number -eq 2 ]
then      
	echo USER: $USER >> trace.txt
	echo HOST: $HOST >> trace.txt
	echo DATE: `date` >> trace.txt
	echo " " >> trace.txt
	sleep 4
	cat trace0.txt trace1.txt trace2.txt | sort >> trace.txt
	rm trace0.txt trace1.txt trace2.txt 

fi



fi   # end if-else statement

exit 0

