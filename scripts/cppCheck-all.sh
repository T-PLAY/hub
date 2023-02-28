#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done


cd $rootPath

cppcheck --enable=all --error-exitcode=1 core/src tests server -I core/include -I server/libServer/ --suppress=toomanyconfigs --suppress=missingIncludeSystem


# for file in $(find $(cat var/source_dirs.txt) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
#     echo $file
#     cmake-format -i $file
# done
#
