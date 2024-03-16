#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done

cd $(git rev-parse --show-toplevel)

scripts/source-remove-empty-comments.sh
scripts/source-format-all.sh

scripts/cmake-remove-empty-comments.sh
scripts/cmake-format-all.sh


