/*
 * Polhemus Patriot tracker support  Copyright (C) 2009 Nikolaj Sheller
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */

#ifndef POLHEMUS_SENSOR_H_
#define POLHEMUS_SENSOR_H_

#include "BinaryPosition.h"
#include "Position.h"

namespace Polhemus {
class Patriot;

class Sensor
{
    char mStationNumber;
    Patriot* mTracker;
    bool isActiveBool;

  public:
    Sensor( Patriot* tracker, char stationNumber );
    char getStationNumber() const;

    /**
     * Get sensor position in Cartesian coordinates
     */
    const Position getPosition() const;

    /**
     * Get sensor position in Euler Orientation Angles
     */
    const Orientation getOrientation() const;

    void updateData( void* data ) const;

    /**
     * Is sensor connected and active
     */
    bool isActive() const;
    void setActive( bool isActive );

    void setAlignmentReferenceOrigin();
};
} // namespace Polhemus

#endif /* POLHEMUS_SENSOR_H_ */
