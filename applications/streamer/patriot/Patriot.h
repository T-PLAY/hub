/*
 * Polhemus Patriot tracker support  Copyright (C) 2009 Nikolaj Sheller
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */

#ifndef POLHEMUS_GENERICTRACKER_H_
#define POLHEMUS_GENERICTRACKER_H_

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "BinaryPosition.h"
#include "MessageStructures.h"
#include "Position.h"
#include "Sensor.h"
#include <libusb-1.0/libusb.h>

namespace Polhemus {
using std::cout;
using std::endl;
using std::exception;
using std::map;
using std::ostringstream;
using std::string;
using std::vector;

#define CTRL_B 0x02
#define CTRL_E 0x05
#define CTRL_K 0x0B
#define CTRL_O 0x0F
#define CTRL_R 0x12
#define CTRL_T 0x14
#define CTRL_U 0x15
#define CTRL_V 0x16
#define CTRL_W 0x17
#define CTRL_X 0x18
#define CTRL_Y 0x19
#define CTRL_Z 0x1A

#define READ_BUFFER_SIZE 1000
#define USB_TIMEOUT 50
//    #define USB_TIMEOUT 500
#define PATRIOT_SENSOR_COUNT 2

#define POLHEMUS_NO_FIRMWARE_PRODUCTID 0xef11

/**
 * Enumerators are assigned values that correspond to the values the
 * trackers return.
 */
enum Trackers { LIBERTY = 0, PATRIOT = 1 };
enum Units { INCHES = 0x00, CENTIMETERS = 0x01 };
enum OutputFormat { ASCII = 0x00, BINARY = 0x01 };
enum ActiveStationState { OFF = 0, ON = 1 };

struct TrackerUSBParameters {
    int vendorID;
    int productID;
    int bulkWriteEndPoint;
    int bulkReadEndPoint;
};

const TrackerUSBParameters USB_PARMS[2] = {
    { 0x0f44, 0xff12, 0x02, 0x82 }, // Liberty
    { 0X0f44, 0xef20, 0x04, 0x88 }  // Patriot HS
                                    //			{0x0f44, 0xef12, 0x02, 0x82}};  	// Patriot
};

struct ErrorCodeComperator {
    bool operator()( const char s1, const char s2 ) const { return s1 < s2; }
};

class TrackerVersionInfo
{
    bool isLibertyBool;
    bool isPatriotBool;
    unsigned char sensorCount;

  public:
    TrackerVersionInfo( WhoAmIBinaryResponse whoAmIBinaryResponse ) {
        isLibertyBool = false;
        isPatriotBool = false;

        string trackerName( whoAmIBinaryResponse.trackerName );

        switch ( whoAmIBinaryResponse.trackerType ) {
        case PATRIOT:
            if ( trackerName.find( "Patriot" ) == trackerName.npos ) throw new exception();
            cout << "\nConnected to Patriot" << endl;
            isPatriotBool = true;
            break;
        case LIBERTY:
            if ( trackerName.find( "Liberty" ) == trackerName.npos ) throw new exception();
            cout << "\nConnected to Liberty" << endl;
            isLibertyBool = true;
            break;
        default:
            cout << "\nUnknown tracker. Tracker returned: " << endl;
            cout << (char*)&whoAmIBinaryResponse << '\n';
        }
    }

    bool isLiberty() { return isLibertyBool; }

    bool isPatriot() { return isPatriotBool; }

    bool getSensorCount() const { return sensorCount; }
};

class Patriot
{
  private:
    TrackerVersionInfo* trackerVerInf = nullptr;

  protected:
  public:
    int readPort, writePort;
    bool connected;
    vector<Sensor*> sensors;
    int sensorCount;
    map<char, string, ErrorCodeComperator> errorDescriptions;

    bool checkResponseData( BinaryResponseHeader& binaryResponsebuffer );

    void requestBinaryPositionData();

    bool receive( SingleDataBinaryResponse2_4_1& binaryResponse );
    bool receive( WhoAmIBinaryResponse& binaryResponse );
    bool receive( SingleDataBinaryResponse& binaryResponse );
    bool receive( OutputFormatBinaryResponse& binaryResponse );
    bool receive( UnitsBinaryResponse& binaryResponse );
    bool receive( ActiveStationStateBinaryResponse& binaryResponse );
    bool receive( void* binaryResponsebuffer, int bufferSize );

    void clearPendingData( string command );

    void send( string data ) const;

  public:
    Patriot();
    ~Patriot();

    /**
     * Request version info from Patriot using '^V' - WhoAmI
     * See TAP Cmd IF Spec.
     */
    void requestVersionInfo();

    /**
     * Set Patriot Binary output format using ‘F’ – Output Format
     * See TAP Cmd IF Spec.
     */
    void putTrackerIntoBinaryMode();
    void putTrackerInMetricUnits();
    void putTrackerInEnglishUnits();
    bool isTrackerInMetricUnits();

    TrackerVersionInfo const* getVersionInfo();

    /**
     * Enable hemisphere tracking. Indicate which hemisphere sensor is in.
     */
    void enableHemisphereTracking( const Sensor& sensor, const Position& origin );

    /**
     * Connect to tracker.
     * Returns true if a tracker is found and connection is established.
     */
    bool connect();

    void updateSensorActiveState( signed int activeStationBitmap );

    bool isConnected() const;
    /**
     * Returns sensors active at connection time
     */
    const vector<Sensor*>& getSensors() const;

    void setAlignmentReferenceOrigin( char sensorID, Position& origin );
    void resetAlignmentFrame( char sensorID );

    /**
     * Returns true is tracker in binary output format
     */
    bool getIsInBinaryOutputMode();

    /**
     * Set Patriot output format using ‘^U’ – Active Station State
     * See TAP Cmd IF Spec.
     */
    signed int getStationActivationState();

    /**
     * Set Patriot output format using ‘F’ – Output Format
     * See TAP Cmd IF Spec.
     */
    void setTrackerBinaryMode( bool setBinaryMode );

    /**
     * Set Patriot ASCII output format using ‘F’ – Output Format
     * See TAP Cmd IF Spec.
     */
    void putTrackerIntoASCIIMode();

    // void resetAlignmentFrame(Sensor& sensor);
    // void setAlignmentReferenceOrigin(Sensor& sensor, Position& origin);

    libusb_device_handle* handle;

    /**
     * Returned value must be deleted by caller after use.
     */
    BinaryPosition getMeasurement();

    //        void * getData();
    //        SingleDataBinaryResponse2_4_1 getData();
    void updateData( void* data );

    //    private:
    //        void * data = nullptr;
};
} // namespace Polhemus

#endif /* POLHEMUS_GENERICTRACKER_H_ */
