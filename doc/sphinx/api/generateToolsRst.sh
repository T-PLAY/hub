#! /bin/bash -e


headers=("-" "*" "#")

echo "
..
	generated by generateToolsRst.sh script on : $(date)

Tools
=====

Most of the tools in the project bin \`folder <https://github.com/T-PLAY/hub/-/tree/main/bin>\`__.


"

printHeaders() {
	title=$1
	level=$2
	if [ $level -gt 2 ]; then
		echo "level must be between 0 and 2"
		echo "level=$level"
		exit 3
	fi
	# echo "title = '$title'"
	# echo "level = '$level'"
	if ! expr $level + 1 > /dev/null; then
		echo "level : $level is not number"
		exit 1
	fi

	strLength=$(expr length "$title")
	# echo "strLength = $strLength"

	echo
	echo $title
	# echo "headers = $headers"
	for i in `seq 1 $strLength`; do
		echo -n "${headers[$level]}"
	done
	echo
	echo
}

# printHeaders "\`$index-$exampleName <https://gitlab.irit.fr/storm/repos/projects/private/dcs/plateforme-experimentale/hub/-/blob/documentation/examples/$exampleFilepath>\`__" 0
# exit 0

parseDir() {
	if [ ! -d "$1" ]; then
		echo "$1 is not a dir"
	fi
	dir=$1
	depth=$2
	if ! expr $depth + 1 > /dev/null; then
		exit 2
	fi

	# echo "parsing dir : $1"

	for exampleFilepath in $(find $dir -maxdepth 1 -mindepth 1 | sort); do
		dir=$1
		depth=$2


		exampleFileName=$(basename $exampleFilepath)
		exampleFileNameWithoutExtension=$(echo $exampleFileName | cut -d. -f1)
		# echo "$exampleFileName"
		exampleName=$(echo $exampleFileName | cut -d. -f1 | cut -d- -f1 --complement)
		# exampleName=$(echo $exampleFileName | cut -d. -f1 | awk -F- '{print $NF}')
		extension=$(echo $exampleFileName | cut -d. -f2)

		if [ "$exampleName" = "doxygen" ]  \
			|| [ "$exampleName" = "sphinx" ]  \
			|| [ "$exampleName" = "CMakeLists.txt" ]  \
			|| [ "$exampleName" = "readme" ] \
			|| [ "$exampleName" = "wip" ]; 
				then

			continue
		fi

		# echo $extension

		if [ $extension == "txt" ]; then
			continue
		fi

		# echo "exampleName = $exampleName"
		# echo $exampleName
		# continue

		if [ -d $exampleFilepath ]; then
			if [ -e $exampleFilepath/main.cpp ]; then
				exampleFilepath=$exampleFilepath/main.cpp
			else

				# printHeaders $exampleName $depth
				parseDir $exampleFilepath $(expr $depth + 1)
				continue
			fi
		fi

		# echo "exampleName=$exampleName"
		# echo "extension=$extension"

		# if ! cat $exampleFilepath | grep 'return 0' > /dev/null; then
		#     echo "return 0 not found in $exampleFilepath"
		#     exit 1
		# fi
        #
		# if ! cat $exampleFilepath | grep '\\file' > /dev/null; then
		#     echo "\file not found in $exampleFilepath"
		#     exit 1
		# fi

		# continue

		# exampleFilepath=$(echo $exampleFilepath | cut -c1-18 --complement)
		# echo "exampleFilepath=$exampleFilepath"


		index=$iExample
		if [ $iExample -lt 10 ]; then
			index="0$iExample"
		fi

		echo

		# echo "exampleName=$exampleName"

		printHeaders "\`$index-$exampleName <https://github.com/T-PLAY/hub/blob/main/bin/$exampleFilepath>\`__" "$depth"

		# printHeaders "Includes" "$(expr $depth + 1)"

#         echo "
# \`$index-$exampleName <https://gitlab.irit.fr/storm/repos/projects/private/dcs/plateforme-experimentale/hub/-/blob/documentation/examples/$exampleFilepath>\`__
# ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# Includes
# ********

		echo "./hub-$exampleName"

# .. literalinclude:: $exampleFilepath
#    :language: cpp
#    :end-before: \file
# "
#
	# printHeaders "Source Code" "$(expr $depth + 1)"

		# Source Code
		# ***********

		# echo "
# .. doxygenfile:: $exampleFileName
		echo "
.. literalinclude:: $exampleFilepath
   :language: cpp
   :start-after: Description
   :end-before: End description
   :tab-width: 2
"

		iExample=$(expr $iExample + 1)
		
	done
}

iExample=0

parseDir "../../../bin" 0
