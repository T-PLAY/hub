#ifndef POLHEMUS_MESSAGESTRUCTURES_H_
#define POLHEMUS_MESSAGESTRUCTURES_H_

namespace Polhemus {
	/**
	 * Data from tracker is not aligned. Writing data from tracker to structs
	 * without extensive parsing requires structs to be aligned to one byte.
	 */
	#pragma pack(push, 1)

	/**
	 * ASCII carriage return, linefeed
	 * Parameter Value 1. See TAP Cmd IF Spec. ‘O’ – Output Data List
	 */
	struct ASCIICarriageReturn {
		char carriageReturnLineFeed[2];
	};

	/**
	 * X, Y, Z Cartesian coordinates of position
	 * Parameter Value 2. See TAP Cmd IF Spec. ‘O’ – Output Data List
	 */
	struct CartesianCoordinate {
		float x;
		float y;
		float z;
	};

	/**
	 * Az, El, Rl Euler orientation angles
	 * Parameter Value 4. See TAP Cmd IF Spec. ‘O’ – Output Data List
	 */
	struct EulerOrientationAngles {
		float azimuth;
		float elevation;
		float roll;
	};

	/**
	 * Binary Response Format Header
	 * See TAP Cmd IF Spec. 2.2.3 Response Format Notes
	 */
	struct BinaryResponseHeader {
		unsigned char frameTag1;
		unsigned char frameTag2;
		unsigned char stationNumber;
		unsigned char initiatingCommand;
		unsigned char errorIndicator;
		unsigned char reserved;
		signed short responseSize;
	};

	struct WhoAmIBinaryResponse : BinaryResponseHeader {
		unsigned char sensorCount;
		unsigned char trackerType;
		unsigned char reserved2;
		unsigned char unknown[6];
		ASCIICarriageReturn carriageReturnLineFeed1;
		char trackerName[16];
		ASCIICarriageReturn carriageReturnLineFeed2;
		ASCIICarriageReturn carriageReturnLineFeed3;
		char bootLoaderVersionText[20];
		char bootVersion[9];
		ASCIICarriageReturn carriageReturnLineFeed4;
		char firmwareVersionText[28];
		char systemControllerVersion[8];
		ASCIICarriageReturn carriageReturnLineFeed5;
		char ioProcessorVersionText[22];
		char ioProcessorVersionNumber[10];
		ASCIICarriageReturn carriageReturnLineFeed6;
	};

	struct SingleDataBinaryResponse : BinaryResponseHeader {
		char binaryBuffer[84];
	};

	struct SingleDataBinaryResponse2_4_1 : BinaryResponseHeader {
		CartesianCoordinate cartesianCoordiante;
		EulerOrientationAngles eulerOrientationAngles;
		ASCIICarriageReturn carriageReturnLineFeed;
	};

	struct OutputFormatBinaryResponse : BinaryResponseHeader {
		signed int asciiOrBinary;
	};

	struct UnitsBinaryResponse : BinaryResponseHeader {
		signed int inchesOrCentimeters;
	};

	struct ActiveStationStateBinaryResponse : BinaryResponseHeader {
		signed int activeStationBitmap;
	};
	#pragma pack(pop)
}

#endif /* POLHEMUS_MESSAGESTRUCTURES_H_ */
