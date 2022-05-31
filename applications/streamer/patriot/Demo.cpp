/*
 * Polhemus Patriot tracker support  Copyright (C) 2009 Nikolaj Sheller
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */
#include <iostream>

#include "Sensor.h"
//#include "GenericTracker.h"
#include "Patriot.h"

using namespace std;


int main() {
	try {
        Polhemus::Patriot tracker;

		cout << "Starting Patriot Demo" << endl;

		if (!tracker.connect()) {
			cout << "Error connecting to tracker" << endl;
			return -1;
		} else
			cout << "Connected to tracker" << endl;

        Polhemus::Sensor allSensors(&tracker, '*');
        Polhemus::Position positiveX(1.0f, 0.0f, 0.0f);
		tracker.enableHemisphereTracking(allSensors, positiveX);
		tracker.putTrackerInMetricUnits();

		if(tracker.getSensors()[0]->isActive())
			cout << "Sensor 1 active" << endl;

		if(tracker.getSensors()[1]->isActive())
			cout << "Sensor 2 active" << endl;

        const Polhemus::TrackerVersionInfo* tvi = tracker.getVersionInfo();
		cout << "Sensor count reported: " << tvi->getSensorCount() << "\n";
		delete tvi;

        Polhemus::Sensor* sensor = tracker.getSensors()[0];
        const Polhemus::Position pos = sensor->getPosition();
		tracker.resetAlignmentFrame(sensor->getStationNumber());

		//tracker.setAlignmentReferenceOrigin(sensor->getStationNumber(), *pos);

//		delete pos;

		for (int c = 0; c < 500; c++) {
            Polhemus::Position pos = sensor->getPosition();
            Polhemus::Orientation ori = sensor->getOrientation();
			cout.setf(ios::floatfield);
//			cout << "x= " << fixed << pos->getXpos() <<
//					" y= " <<  pos->getYpos() <<
//					" z= " <<  pos->getZpos() << endl;

            cout << "az= " << fixed << ori.getAzimuth() <<
                    " ele= " <<  ori.getElevation() <<
                    " roll= " <<  ori.getRoll()<< endl;

//			delete ori;
//			delete pos;
		}

		return 0;
	} catch (...) {
		cout << "Error caught." << endl;
	}
}
