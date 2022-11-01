/*
 * Polhemus Patriot tracker support  Copyright (C) 2009 Nikolaj Sheller
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */
#include <iostream>

#include "Sensor.h"
//#include "GenericTracker.h"
#include "Patriot.h"

#include "BinaryPosition.h"
#include "MessageStructures.h"
#include <cassert>
#include <cstring>

using namespace std;

int main() {
    try {
        Polhemus::Patriot tracker;

        cout << "Starting Patriot Demo" << endl;

        if ( !tracker.connect() ) {
            cout << "Error connecting to tracker" << endl;
            return -1;
        }
        else
            cout << "Connected to tracker" << endl;

        Polhemus::Sensor allSensors( &tracker, '*' );

        Polhemus::Position positiveX( 1.0f, 0.0f, 0.0f );
        tracker.enableHemisphereTracking( allSensors, positiveX );
        tracker.putTrackerInMetricUnits();

        if ( tracker.getSensors()[0]->isActive() ) cout << "Sensor 1 active" << endl;

        if ( tracker.getSensors()[1]->isActive() ) cout << "Sensor 2 active" << endl;

        //		cout << "Sensor count reported: " << tvi->getSensorCount() << "\n";
        //		delete tvi;

        Polhemus::Sensor* sensor = tracker.getSensors()[0];
        tracker.resetAlignmentFrame( sensor->getStationNumber() );

        // tracker.setAlignmentReferenceOrigin(sensor->getStationNumber(), *pos);

        //		delete pos;

        float data[7];

        constexpr int N = 1;
        for ( int c = 0; c < N; ++c ) {

            Polhemus::SingleDataBinaryResponse2_4_1 singleDataBinaryResponse;

            assert( tracker.isConnected() );


            tracker.clearPendingData( "" );

            tracker.requestBinaryPositionData();
            bool received = tracker.receive( singleDataBinaryResponse );
            assert( received );


            std::cout << "x = " << data[0] << ", y = " << data[1] << ", z = " << data[2]
                      << std::endl;


            //			delete ori;
            //			delete pos;
        }

        return 0;
    }
    catch ( ... ) {
        cout << "Error caught." << endl;
    }
}
