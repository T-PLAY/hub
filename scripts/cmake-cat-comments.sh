#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

for file in $(find $(cat scripts/source_dirs.txt) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
	echo "-------------------------------- $file"
	
	cat $file | grep -n '^ *#[^ ].*$' || sleep 0
	cat $file | grep -n '^ *# [^A-Z^#].*$' || sleep 0
done

