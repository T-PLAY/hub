#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath
CLANG_FORMAT_EXE=clang-format

if [ -e "/opt/clang-format-static/clang-format-14" ]; then
	CLANG_FORMAT_EXE="/opt/clang-format-static/clang-format-14"
fi

CI_CLANG_FORMAT_VERSION="14.0.0"

CLANG_FORMAT_VERSION=$($CLANG_FORMAT_EXE --version | awk -F' ' '{print $3'})
if [ "$CLANG_FORMAT_VERSION" != "$CI_CLANG_FORMAT_VERSION" ]; then
	echo "clang-format need to be compatible with CI version : $CI_CLANG_FORMAT_VERSION"
	exit 1
fi

cd $(git rev-parse --show-toplevel)

# export PATH="/opt/clang-format-static:$PATH"

# echo "using clang-format : $(clang-format --version)"

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*') -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo "$file"

	# clang-format-11 -i --style=file $file

	# clang-format on ubuntu ci is on version 14.0.0
	# if [ -e "/opt/clang-format-static/clang-format-14" ]; then
		# clang-format-14 -i --style=file $file
	# else
		# clang-format -i --style=file $file
	# fi
	$CLANG_FORMAT_EXE -i --style=file $file
done
