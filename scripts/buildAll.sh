
#/bin/bash -e

if [ $(basename `pwd`) != "hub" ]; then
	echo "execute this script in root dir of hub"
	exit 0
fi

mkdir -p build

for arch in "x64" "Win32" "ARM64" "ARM"; do
# for arch in "x64" "Win32"; do
# for arch in "x64"; do
	for static in "ON" "OFF"; do
	# for static in "ON"; do
		for mode in "Debug" "Release"; do
		# for mode in "Debug"; do
			buildDir="build/$arch-$mode-static_$static"
			# buildDir="build"
			mkdir -p $buildDir
			# cmake -B $buildDir . -A $arch -DHUB_BUILD_STATIC_LIBRARY=$static -DCMAKE_INSTALL_PREFIX=install
			cmake -B $buildDir . -A $arch -DHUB_BUILD_STATIC_LIBRARY=$static -DCMAKE_INSTALL_PREFIX=install -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0
			cmake --build $buildDir --config $mode --parallel
			cmake --install $buildDir --config $mode

			# cmake --build $buildDir --config Release
			# cmake -B $buildDir --install --config Release
		done
	done
done

# for arch in "x64"; do
# 	buildDir="build-$arch"
# 	mkdir -p $buildDir
# 	cmake -B $buildDir . -A $arch
# 	cmake --build $buildDir --config Debug
# 	cmake --build $buildDir --config Release
# done

# for arch in "x64" "ARM64"; do
# 	buildDir="build-UWP-$arch"
# 	mkdir -p $buildDir
# 	cmake -B $buildDir . -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -A $arch
# 	cmake --build $buildDir --config Debug
# 	cmake --build $buildDir --config Release
# done

