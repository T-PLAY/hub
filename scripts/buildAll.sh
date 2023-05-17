
#/bin/bash -e

if [ $(basename `pwd`) != "hub" ]; then
	echo "execute this script in root dir of hub"
	exit 0
fi

rootDir="buildAll"
mkdir -p $rootDir

for arch in "x64" "Win32" "ARM64" "ARM"; do
#for arch in "x64" "Win32"; do
#for arch in "x64" "Win32"; do
#for arch in "x64"; do
#for arch in "Itanium"; do
#for arch in "Win32"; do
	for static in "ON" "OFF"; do
	#for static in "ON"; do
	#for static in "OFF"; do
		#for mode in "Debug" "Release"; do
		#for mode in "Debug" "Release" "RelWithDebInfo"; do
		for mode in "Release"; do
		#for mode in "Release"; do
		#for mode in "RelWithDebInfo"; do
			#for system in "defaultSystem"; do
			for system in "default" "UWP"; do
			#for system in "UWP"; do
			#for mode in "Debug"; do
				installDir="$rootDir/install-$system"
				buildDir="$rootDir/$arch-$mode-static_$static_$system"
				# buildDir="build"
				mkdir -p $buildDir
	# 	cmake -B $buildDir . -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -A $arch
				if [ "$system" == "UWP" ]; then
					cmake -B $buildDir . -A $arch -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_BUILD_TYPE=$mode -DHUB_BUILD_STATIC_LIBRARY=$static -DCMAKE_INSTALL_PREFIX=$installDir
				else
					cmake -B $buildDir . -A $arch -DCMAKE_BUILD_TYPE=$mode -DHUB_BUILD_STATIC_LIBRARY=$static -DCMAKE_INSTALL_PREFIX=$installDir
				fi
			#	cmake -B $buildDir . -A $arch -DHUB_BUILD_STATIC_LIBRARY=$static -DCMAKE_INSTALL_PREFIX=$installDir -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0
				cmake --build $buildDir --config $mode --parallel
				cmake --install $buildDir --config $mode

				# cmake --build $buildDir --config Release
				# cmake -B $buildDir --install --config Release
			done
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

