#! /bin/bash -e

cd $(git rev-parse --show-toplevel)

bfg --strip-blobs-bigger-than 1M .

echo "---------------------------------------------------"

git reflog expire --expire=now --all && git gc --prune=now --aggressive
