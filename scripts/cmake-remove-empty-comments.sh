#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath

cd $(git rev-parse --show-toplevel)

for file in $(find $(cat scripts/source_dirs.txt) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
	echo "-------------------------------- $file"

	sed -i '/^ *#[^ ].*$/d' $file
	sed -i '/^ *# [^A-Z^#].*$/d' $file
done
