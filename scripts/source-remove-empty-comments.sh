#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath

cd $(git rev-parse --show-toplevel)

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*') -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo "$file"
	
	sed -i '/^ *\/\/[^ ^\/].*$/d' $file #         //a
	# sed -i '/^ *\/\/ [ ].*$/d' $file
	sed -i '/^ *\/\/ [^A-Z^c].*$/d' $file # // auou, protect // cppcheck-suppress and // clang-format
	sed -i '/^ *\/\/ *$/d' $file # // remove empty comment

done
