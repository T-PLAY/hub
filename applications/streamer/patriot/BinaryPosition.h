#ifndef POLHEMUS_BINARYPOSITION_H_
#define POLHEMUS_BINARYPOSITION_H_

#include "MessageStructures.h"
#include "Orientation.h"
#include "Position.h"

namespace Polhemus {
	class BinaryPosition {
		CartesianCoordinate cartesianCoordiante;
		EulerOrientationAngles eulerOrientationAngle;

	public:
		BinaryPosition(SingleDataBinaryResponse2_4_1& binaryPositionData) {
			// TODO Endianess conversion if built on platform with different
			// endianess than Polhemus see #include <netinet/in.h>. detection of
			// platform endianess required
			cartesianCoordiante.x = binaryPositionData.cartesianCoordiante.x;
			cartesianCoordiante.y = binaryPositionData.cartesianCoordiante.y;
			cartesianCoordiante.z = binaryPositionData.cartesianCoordiante.z;

			eulerOrientationAngle.azimuth =
					binaryPositionData.eulerOrientationAngles.azimuth;
			eulerOrientationAngle.elevation =
					binaryPositionData.eulerOrientationAngles.elevation;
			eulerOrientationAngle.roll =
					binaryPositionData.eulerOrientationAngles.roll;
		}

		Position getPosition() {
			return Position(
					cartesianCoordiante.x,
					cartesianCoordiante.y,
					cartesianCoordiante.z);
		}

		Orientation getOrientation() {
			return Orientation(
					eulerOrientationAngle.azimuth,
					eulerOrientationAngle.elevation,
					eulerOrientationAngle.roll);
		}
	};
}

#endif /* POLHEMUS_BINARYPOSITION_H_ */
