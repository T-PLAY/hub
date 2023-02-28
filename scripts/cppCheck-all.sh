#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

cppcheck --enable=all src tests server -I include -I server/libServer/ --suppress=toomanyconfigs --suppress=missingIncludeSystem


# for file in $(find $(cat scripts/sources.txt) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
#     echo $file
#     cmake-format -i $file
# done
#
