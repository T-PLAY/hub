#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

for file in $(find $(cat var/source_dirs.txt) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
	echo $file
	cat $file | grep '^#' || sleep 0
done

