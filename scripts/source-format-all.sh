#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath
for file in $(find . -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" -not -path "*build*" -not -path "*install*" \)); do
	echo $file
	clang-format -i --style=file $file
done
