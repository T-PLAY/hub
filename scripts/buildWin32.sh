
#/bin/bash -e

if [ $(basename `pwd`) != "hub" ]; then
	echo "execute this script in root dir of hub"
	exit 0
fi

installDir="install"

for mode in "Debug" "Release"; do
	for arch in "Win32"; do
		buildDir="build-$arch-$mode"
		mkdir -p $buildDir
		cmake -B $buildDir . -A $arch -DCMAKE_BUILD_TYPE=$mode -DHUB_BUILD_STATIC_LIBRARY=ON -DCMAKE_INSTALL_PREFIX=$installDir

		cmake --build $buildDir --config $mode
		cmake -B $buildDir --install --config $mode --parallel
		cmake --install $buildDir --config $mode

		# cmake --build $buildDir --config Release
		# cmake -B $buildDir --install --config Release
	done
done

