#! /bin/bash -e


headers=("-" "*" "#")

echo "
..
	generated by generateExamplesRst.sh script on : $(date)

Examples
========

Most of the examples in the examples \`directory <https://github.com/T-PLAY/hub/-/tree/main/examples>\`__.


"

printHeaders() {
	title=$1
	level=$2
	if [ $level -gt 2 ]; then
		echo "level must be between 0 and 2"
		echo "level=$level"
		exit 3
	fi

	if ! expr $level + 1 > /dev/null; then
		echo "level : $level is not number"
		exit 1
	fi

	strLength=$(expr length "$title")

	echo
	echo $title
	for i in `seq 1 $strLength`; do
		echo -n "${headers[$level]}"
	done
	echo
	echo
}


parseDir() {
	if [ ! -d "$1" ]; then
		echo "$1 is not a dir"
	fi
	dir=$1
	depth=$2
	if ! expr $depth + 1 > /dev/null; then
		exit 2
	fi

	for exampleFilepath in $(find $dir -maxdepth 1 -mindepth 1 | sort); do
		dir=$1
		depth=$2


		exampleFileName=$(basename $exampleFilepath)
		exampleName=$(echo $exampleFileName | cut -d. -f1 | cut -d- -f1 --complement)
		extension=$(echo $exampleFileName | cut -d. -f2)

		if [ "$exampleName" = "doxygen" ]  \
			|| [ "$exampleName" = "sphinx" ]  \
			|| [ "$exampleName" = "CMakeLists.txt" ]  \
			|| [ "$exampleName" = "readme" ] \
			|| [ "$exampleName" = "wip" ]; 
				then

			continue
		fi

		if [ $extension == "txt" ]; then
			continue
		fi

		if [ -d $exampleFilepath ]; then
			if [ -e $exampleFilepath/main.cpp ]; then
				exampleFilepath=$exampleFilepath/main.cpp
			else

				parseDir $exampleFilepath $(expr $depth + 1)
				continue
			fi
		fi

		if ! cat $exampleFilepath | grep 'return 0' > /dev/null; then
			echo "return 0 not found in $exampleFilepath"
			exit 1
		fi
		if ! cat $exampleFilepath | grep '\\file' > /dev/null; then
			echo "\file not found in $exampleFilepath"
			exit 1
		fi


		index=$iExample
		if [ $iExample -lt 10 ]; then
			index="0$iExample"
		fi

		echo


		printHeaders "\`$index-$exampleName <https://github.com/T-PLAY/hub/blob/main/$exampleFilepath>\`__" "$depth"


		echo "
.. literalinclude:: ../../../$exampleFilepath
   :language: cpp
   :end-before: \file
"

		echo "
.. literalinclude:: ../../../$exampleFilepath
   :language: cpp
   :start-after: main(
   :end-before: return 0
   :tab-width: 2
"

		iExample=$(expr $iExample + 1)
		
	done
}

iExample=0

cd ../../..
parseDir "examples" 0
