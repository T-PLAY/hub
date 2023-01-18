#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done

export PATH="/opt/clang-format-static:$PATH"

cd $rootPath
for file in $(find $(cat scripts/sources.txt) -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo $file
	clang-format-11 -i --style=file $file
done
