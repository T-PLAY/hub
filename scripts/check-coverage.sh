#! /bin/bash -e




lines=$(cat $1/index.html | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | head -n 1)
functions=$(cat $1/index.html | grep headerCovTableEntryLo | awk -F '[>&]' '{print $2}' | tail -n 1)

echo "lines coverage = $lines %"
echo "functions coverage = $functions %"

linesMin=53.8
functionsMin=48.8

if (( $(echo "$lines < $linesMin" | bc -l) )) ; then
	echo "lines coverage must be superior as $linesMin %"
	exit 1
fi

if (( $(echo "$functions < $functionsMin" | bc -l) )); then
	echo "functions coverage must be superior as $functionsMin %"
	exit 1
fi

exit 1
