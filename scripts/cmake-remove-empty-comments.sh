#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

for file in $(find . -type f \( -name "*.cmake" -o -name "CMakeLists.txt" \)); do
	echo $file
	sed -i '/^ *#[^#]*$/d' $file
done
