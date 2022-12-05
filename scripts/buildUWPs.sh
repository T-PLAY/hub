
#/bin/bash -e

if [ $(basename `pwd`) != "hub" ]; then
	echo "execute this script in root dir of hub"
	exit 0
fi

for arch in "x64" "ARM64"; do
	buildDir="build-UWP-$arch"
	mkdir -p $buildDir
	cmake -B $buildDir . -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -A $arch
	cmake --build $buildDir --config Debug
	cmake --build $buildDir --config Release
done
