#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

for file in $(find $(cat scripts/source_dirs.txt) -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo $file
	cat $file | grep '^ *//[^/]' || sleep 0
done
