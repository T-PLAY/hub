#! /bin/bash -e


cd $(git rev-parse --show-toplevel)


gitVersion=$(git describe --tags --abbrev=0)


sed -i "s/version = '.*'/version = '$gitVersion'/g" doc/sphinx/conf.py
