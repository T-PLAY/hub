#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath

cd $(git rev-parse --show-toplevel)

# echo "using cmake-format : $(cmake-format --version)"

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*' ) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
	echo "$file"

	cmake-format -i $file
done

