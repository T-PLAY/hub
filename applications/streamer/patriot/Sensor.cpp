/*
 * Polhemus Patriot tracker support  Copyright (C) 2009 Nikolaj Sheller
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */

#include "Sensor.h"
#include "Patriot.h"
#include "BinaryPosition.h"

namespace Polhemus {
	Sensor::Sensor(Patriot* tracker, char stationNumber) {
		mTracker = tracker;
		mStationNumber = stationNumber;
	}

	char Sensor::getStationNumber() const {
		return mStationNumber;
	}

	/**
	 * Get sensor position in Cartesian coordinates
	 */
	const Position Sensor::getPosition() const {
		BinaryPosition* bp = mTracker->getMeasurement();
		Position position = bp->getPosition();
		delete bp;
		return position;
	}

	const Orientation Sensor::getOrientation() const {
		BinaryPosition* bp = mTracker->getMeasurement();
		Orientation orientation = bp->getOrientation();
		delete bp;
		return orientation;
	}

	bool Sensor::isActive() const {
		return isActiveBool;
	}

	void Sensor::setActive(bool isActive) {
		isActiveBool = isActive;
	}

	void Sensor::setAlignmentReferenceOrigin() {
		//Position* pos = getPosition();
		mTracker->resetAlignmentFrame(getStationNumber());
		//mTracker->setAlignmentReferenceOrigin(getStationNumber(), *pos);
		//delete pos;
	}
}
