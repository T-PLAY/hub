#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

for file in $(find . -type f \( \( -name "CMakeLists.txt" -o -name "*.cmake" \) -not -path "*build*" -not -path "*install*" \)); do
	echo $file
	cmake-format -i $file
done

