#! /bin/bash -e


if [ $# -ne 2 ]; then
	echo "usage $0: coverageDir coverageRefDir"
	exit 1
fi

# echo "param1: $1"
if ! [ -d $1  ]; then
	echo "$1 is not a directory"
	exit 2
fi

if ! [ -f "$1/index.html"  ]; then
	echo "$1 is not a coverage build directory"
	exit 3
fi

emptyRefDir=0

if ! [ -d $2  ]; then
	echo "$2 coverage dir does not exist"
	emptyRefDir=1
fi

if ((! $emptyRefDir)); then
	if ! [ -d $2/index.html  ]; then
		echo "$2 is not a coverage build directory"
		# echo "$2 coverage dir is empty"
		# emptyRefDir=1
		exit 4
	fi
fi

index=$1/index.html
indexRef=$2/index.html

lines=$(cat $index | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | head -n 1)
functions=$(cat $index | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | tail -n 1)

if (($emptyRefDir)); then
	linesRef=0
	functionsRef=0
else
	linesRef=$(cat $indexRef | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | head -n 1)
	functionsRef=$(cat $indexRef | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | tail -n 1)
fi

echo "lines coverage = $lines% >= $linesRef%"
echo "functions coverage = $functions% >= $functionsRef%"

if (( $(echo "$lines < $linesRef" | bc -l) )) ; then
	echo "lines coverage must be superior as $linesMin %"
	exit 1
fi

if (( $(echo "$functions < $functionsRef" | bc -l) )); then
	echo "functions coverage must be superior as $functionsMin %"
	exit 1
fi

exit 0
