#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath

cd $(git rev-parse --show-toplevel)

CI_CMAKE_FORMAT_VERSION="0.6.13"

# echo "using cmake-format : $(cmake-format --version)"
CMAKE_FORMAT_VERSION=$(cmake-format --version)
if [ "$CMAKE_FORMAT_VERSION" != "$CI_CMAKE_FORMAT_VERSION" ]; then
	echo "cmake-format need to be compatible with CI version : $CI_CMAKE_FORMAT_VERSION"
	exit 1
fi

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*' ) -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \)); do
	echo "$file"

	cmake-format -i $file
done

