#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
#
# cd $rootPath

cd $(git rev-parse --show-toplevel)

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*') -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo "-------------------------------- $file"

	cat $file | grep '^ *//[^ ^/].*$' || sleep 0
	cat $file | grep '^ *// [ ].*$' || sleep 0
done
