#! /bin/bash -e


if [ $# -ne 3 -a $# -ne 4 ]; then
	echo "usage $0: coverageDir declineDir name [hash]"
	exit 1
fi

# echo "param1: $1"
if ! [ -e $1  ]; then
	echo "'$1' not found"
	exit 2
fi

if ! [ -d $1  ]; then
	echo "$1 is not a directory"
	exit 3
fi

if ! [ -f "$1/index.html"  ]; then
	echo "$1 is not a coverage build directory"
	exit 4
fi

emptyDeclineDir=0

if ! [ -d $2  ]; then
	echo "$2 coverage dir does not exist"
	emptyDeclineDir=1
fi


coverageRefFile=""
if ((! $emptyDeclineDir)); then

	if [ -f "$2/$3.log"  ]; then
		coverageRefFile="$2/$3.log"
		# echo "coverage file '$3' not found in $2"
		# exit 3
	fi

	# if ! [ -d $2/index.html  ]; then
	#     echo "$2 is not a coverage build directory"
	#     # echo "$2 coverage dir is empty"
	#     # emptyDeclineDir=1
	#     exit 4
	# fi
fi

index=$1/index.html
gitHash=""
if [ $# -eq 4  ]; then
	gitHash=$4
fi
# indexRef=$2/$3.log

lines=$(cat $index | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | head -n 1)
functions=$(cat $index | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | tail -n 1)

# if (($emptyDeclineDir)); then
if [ -z $coverageRefFile ]; then
	linesRef=0
	functionsRef=0
else
	linesRef=$(cat $coverageRefFile | head -n 1 | awk '{print $2}')
	functionsRef=$(cat $coverageRefFile | head -n 1 | awk '{print $3}')
fi

echo "lines coverage = $lines% >= $linesRef%"
echo "functions coverage = $functions% >= $functionsRef%"

if (( $(echo "$lines < $linesRef" | bc -l) )) ; then
	echo "lines coverage must be superior as $linesRef%"
	exit 1
fi

if (( $(echo "$functions < $functionsRef" | bc -l) )); then
	echo "functions coverage must be superior as $functionsRef%"
	exit 1
fi

############################## SAVING/OVERWRITE #################################

if (($emptyDeclineDir)); then
	mkdir -p $2
fi

# gitHash=$(git log -1 --format="%H")
if ! [ -z $gitHash ]; then
	echo "'$gitHash $lines $functions' added in $2/$3.log file."
	echo "$gitHash $lines $functions" >> $2/$3.log
fi


exit 0
