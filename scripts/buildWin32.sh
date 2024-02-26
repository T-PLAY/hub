
#/bin/bash -e

# if [ $(basename `pwd`) != "hub" ]; then
# 	echo "execute this script in root dir of hub"
# 	exit 0
# fi

cd $(git rev-parse --show-toplevel)

installDir="install"

rootDir="builds"
mkdir -p $rootDir

for mode in "Debug" "Release"; do
#for mode in "Debug"; do
	for arch in "Win32"; do
		buildDir="$rootDir/$arch-$mode"
		mkdir -p $buildDir
		# cmake -B $buildDir . -A $arch -DHUB_BUILD_SHARED=OFF -DCMAKE_BUILD_TYPE=$mode -DCMAKE_INSTALL_PREFIX=$installDir
		cmake -B $buildDir . -A $arch -DCMAKE_BUILD_TYPE=$mode -DCMAKE_INSTALL_PREFIX=$installDir

		cmake --build $buildDir --config $mode
		cmake -B $buildDir --install --config $mode --parallel
		cmake --install $buildDir --config $mode

		# cmake --build $buildDir --config Release
		# cmake -B $buildDir --install --config Release
	done
done

