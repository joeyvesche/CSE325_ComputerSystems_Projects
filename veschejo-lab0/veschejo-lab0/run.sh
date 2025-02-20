#!/bin/bash         
# first line of any bash script file should be "#!/bin/bash"
# btw bash is an acronym for "Bourne Again Shell"

num_param=1

# command-line parameters are designated by a '$' followed immediately by 
# a number indicating the parameter's relative position.
#
# $1 --> MSU netid of user
if [ $# -ne $num_param ]
then      
	#    Let user know what is acceptable usage
	echo ""
	echo ""
	echo "Usage:"
	echo " run.sh <your-MSU-netid>"
	echo " "
else
	# run the program
	rm -rf trace.txt     # If a copy of the output file exists, delete it.
	echo USER: $USER >> trace.txt
	echo HOST: $HOST >> trace.txt
	echo DATE: `date` >> trace.txt
	echo " " >> trace.txt
	./lab0 $1 trace.txt 
fi   # end if-else statement

exit 0

