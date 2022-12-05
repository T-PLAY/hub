#! /bin/bash -e

rootPath=$(pwd)
while [ $(basename "$rootPath") != "hub" ]; do
	rootPath=$(dirname "$rootPath")
done

$rootPath/scripts/source-remove-empty-comments.sh
$rootPath/scripts/source-format-all.sh

$rootPath/scripts/cmake-remove-empty-comments.sh
$rootPath/scripts/cmake-format-all.sh

