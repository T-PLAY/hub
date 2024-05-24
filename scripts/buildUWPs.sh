
#/bin/bash -e

# if [ $(basename `pwd`) != "hub" ]; then
# 	echo "execute this script in root dir of hub"
# 	exit 0
# fi

cd $(git rev-parse --show-toplevel)

rootDir="builds"
mkdir -p $rootDir
installDir="install"

for mode in "Debug" "Release"; do
#for mode in "Debug"; do
	for arch in "x64"; do
		#buildDir="build-$arch"
		buildDir="$rootDir/$arch-$mode"
		mkdir -p $buildDir
		cmake -B $buildDir . -A $arch -DCMAKE_BUILD_TYPE=$mode -DCMAKE_INSTALL_PREFIX=$installDir -DHUB_BUILD_STATIC=OFF
		cmake --build $buildDir --config $mode --parallel
		cmake --install $buildDir --config $mode
	done

	for arch in "x64" "ARM64"; do
	#for arch in "x64"; do
			buildDir="$rootDir/UWP-$arch-$mode"
			mkdir -p $buildDir
			# cmake -B $buildDir . -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -A $arch
			# cmake --build $buildDir --config Debug
			# cmake --build $buildDir --config Release
			cmake -B $buildDir . -A $arch -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_BUILD_TYPE=$mode -DCMAKE_INSTALL_PREFIX=$installDir -DHUB_BUILD_STATIC=OFF
			cmake --build $buildDir --config $mode --parallel
			cmake --install $buildDir --config $mode
	done

done
