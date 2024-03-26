#! /bin/bash -e

cd $(git rev-parse --show-toplevel)


projectBeginDate=$(git log --full-history --reverse --format=%as . | head -1)
projectEndDate=$(git log --full-history --format="%as" . | head -1)
projectBeginYear=$(echo $projectBeginDate | awk -F- '{print $1}')
projectEndYear=$(echo $projectEndDate | awk -F- '{print $1}')
# echo "projectYear $projectBeginYear-$projectEndYear"


# sed -i "1s/^/$header/" ios.hpp
# echo -e "$header\n$(cat $file)" > $file

v2Files="Anyable.hpp Archive.hpp Array.hpp AskerClient2.hpp AskerInterface.hpp AskerMqtt.hpp AskerServer2.hpp Buffer.hpp Client2.hpp ClientSocketBoost.hpp ClientSocketI.hpp Configuration.hpp Format.hpp Header.hpp InputBase.hpp InputFile.hpp InputOutputBase.hpp InputOutputSocket.hpp InputStreamMqtt.hpp InputStreamServer2.hpp ios.hpp MatrixBase.hpp Matrix.hpp MatrixT.hpp MatrixTs.hpp MatrixXD.hpp Node.hpp OutputBase.hpp OutputFile.hpp OutputStreamMqtt.hpp OutputStreamServer2.hpp SerializerBoost.hpp Serializer.hpp SerializerI.hpp SerializerImpl2.hpp SerializerImpl.hpp SerializerT.hpp SerializerZppBits.hpp SerializerZpp.hpp ServerImpl2.hpp ServerSocketBoost.hpp ServerSocketI.hpp Span.hpp StreamBase.hpp StreamerClient2.hpp Stream.hpp StreamMqtt.hpp StreamServer2.hpp StreamViewerClient2.hpp Tuple.hpp Vector.hpp ViewerClient2.hpp ViewerHandler.hpp ViewerInterface.hpp ViewerMqtt.hpp ViewerServer2.hpp ViewerStream.hpp"
gaitsContrib="ClientSocketSystem.hpp NetUtilsSystem.hpp ServerSocketSystem.hpp SocketSystem.hpp Configuration.hpp"


# file=src/core/ios.hpp
# for file in $(find $(cat scripts/source_dirs.txt) -type f \( -name "*.hpp" \)); do
for file in $(find src/ -type f \( -name "*.hpp" \)); do

	# echo "$file"
	isV2=0
	isGaits=0
	for v2File in $v2Files; do
		if [ $v2File = $(basename $file) ]; then
			isV2=1
			break;
		fi
	done
	for gaitsFile in $gaitsContrib; do
		if [ $gaitsFile = $(basename $file) ]; then
			isGaits=1
			break;
		fi
	done


	# file="ios.hpp"
	authorName=$(git log --full-history --reverse --format=%an $file | head -1)
	authorEmail=$(git log --full-history --reverse --format=%ae  $file | head -1)
	authorDate=$(git log --full-history --reverse --format=%as $file | head -1 | sed 's/-/\//g')
	# authorDate=$(git log --reverse --format="%ah" $file | head -1)
	# echo "authorName $authorName"
	# echo "authorEmail $authorEmail"
	# echo "authorDate $authorDate"

	gauthierContrib=""
	if [ $isV2 -eq 0 ] ; then
	# if [ $authorEmail = "gauthierbouyjou@aol.com" ]; then
		if [ $authorEmail = "bouyjou@toulouse-tech-transfer.com" ]; then
			gauthierContrib=" (IRIT, Toulouse Tech Transfer)"
			# gauthierContrib="(IRIT, Toulouse Tech Transfer)"
		elif [ $authorEmail = "Gauthier.Bouyjou@irit.fr" ]; then
			gauthierContrib=" (IRIT)"
		elif [ $authorEmail = "gauthierbouyjou@aol.com" ]; then
			gauthierContrib=" (IRIT)"
		fi
	fi

	if [ $isV2 -eq 1 ] ; then
		if [ $isGaits -eq 1 ]; then
			echo "$file is v2 gaits $gauthierContrib"
		else
			echo "$file is v2 $gauthierContrib"
		fi
	else
		if [ $isGaits -eq 1 ]; then
			echo "$file gaits $gauthierContrib"
		else
			echo "$file $gauthierContrib"
		fi
	fi

	pragmaLine=$(cat $file | grep -n '#pragma once' | awk -F: '{print $1}')
	tail -n +"$pragmaLine" $file > /tmp/tmp.txt
	# mv /tmp/tmp.txt $file
	cat /tmp/tmp.txt > $file

	headerCommon="* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:"

	if [ $isV2 -eq 1 ] ; then
		if [ $isGaits -eq 1 ]; then
	header="/*******************************************************************************
$headerCommon
*   - Gauthier Bouyjou
*   - François Gaits (IRIT)
*******************************************************************************/
"
		else
	header="/*******************************************************************************
$headerCommon
*   - Gauthier Bouyjou
*******************************************************************************/
"
		fi
	else
		if [ $isGaits -eq 1 ]; then
	header="/*******************************************************************************
$headerCommon
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou$gauthierContrib
*   - François Gaits (IRIT)
*******************************************************************************/
"
		else
	header="/*******************************************************************************
$headerCommon
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou$gauthierContrib
*******************************************************************************/
"
		fi
	fi

	echo "$header"
	# continue

	# echo -e "$header\n$(cat $file)" > $file

	echo "$header" > /tmp/tmp.txt
	cat $file >> /tmp/tmp.txt
	mv /tmp/tmp.txt $file

	# clang-format-11 -i --style=file $file
done

