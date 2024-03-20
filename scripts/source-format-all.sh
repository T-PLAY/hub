#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath

cd $(git rev-parse --show-toplevel)

export PATH="/opt/clang-format-static:$PATH"

# echo "using clang-format : $(clang-format --version)"

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*') -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo "$file"

	# clang-format-11 -i --style=file $file
	clang-format -i --style=file $file
done
