#! /bin/bash -e


projectBeginDate=$(git log --reverse --format=%as . | head -1)
projectEndDate=$(git log --format="%as" . | head -1)
projectBeginYear=$(echo $projectBeginDate | awk -F- '{print $1}')
projectEndYear=$(echo $projectEndDate | awk -F- '{print $1}')
# echo "projectYear $projectBeginYear-$projectEndYear"


# sed -i "1s/^/$header/" ios.hpp
# echo -e "$header\n$(cat $file)" > $file

cd $(git rev-parse --show-toplevel)

file=src/core/ios.hpp
# for file in $(find $(cat scripts/source_dirs.txt) -type f \( -name "*.hpp" \)); do
# for file in $(find src/ -type f \( -name "*.hpp" \)); do

	echo "$file"

	# file="ios.hpp"
	authorName=$(git log --reverse --format=%an $file | head -1)
	authorEmail=$(git log --reverse --format=%ae  $file | head -1)
	# authorDate=$(git log --reverse --format=%as -n 1 $file | head -1 | sed 's/-/\//g')
	authorDate=$(git log --reverse --format="%ah" $file | head -1)
	# echo "authorName $authorName"
	# echo "authorEmail $authorEmail"
	# echo "authorDate $authorDate"
	#
	
	pragmaLine=$(cat $file | grep -n '#pragma once' | awk -F: '{print $1}')
	tail -n +"$pragmaLine" $file > tmp.txt
	mv tmp.txt $file


	header="/// © $projectBeginYear-$projectEndYear Hub, All Rights Reserved
/// @author $authorName <$authorEmail>
/// @date $authorDate
	"

	echo "$header"
	echo -e "$header\n$(cat $file)" > $file

	# clang-format-11 -i --style=file $file
# done
