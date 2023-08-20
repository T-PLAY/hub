
# /bin/bash -e

cd $(git rev-parse --show-toplevel)

for i in $(ls tests/test-*); do
	ls src/$(echo $i | sed 's/-/\//g' | sed 's/tests\/test\///g' | sed 's/cpp/hpp/g')
done


for i in $(find src -name "*cpp"); do
	ls tests/test-$(echo $i | sed 's/src\///g' | sed 's/\//-/g')
done
