#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath

cd $(git rev-parse --show-toplevel)

# echo -n '#
# #should be deleted
# # should be deleted
# #  should be deleted
#  #
#  #should be deleted
#  # should be deleted
#     #
#     #should be deleted
#     # should be deleted
# ' >> "scripts/test/CMakeLists.txt"

scripts/cmake-format-all.sh

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*') -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
	echo "$file"

	sed -i '/^[ \t]*# *$/d' $file
	# sed -i '/^ *#[^ ].*$/d' $file
	# sed -i '/^ *# [^A-Z^#].*$/d' $file
	sed -i '/^[ \t]*# [^A-Z^#^c].*$/d' $file
done
