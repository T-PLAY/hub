
#/bin/bash -e

if [ $(basename `pwd`) != "hub" ]; then
	echo "execute this script in root dir of hub"
	exit 0
fi

for arch in "Win32"; do
	buildDir="build-$arch"
	mkdir -p $buildDir
	cmake -B $buildDir . -A $arch -DHUB_BUILD_STATIC_LIBRARY=ON
	cmake --build $buildDir --config Debug
	cmake -B $buildDir --install --config Debug

	cmake --build $buildDir --config Release
	cmake -B $buildDir --install --config Release
done

