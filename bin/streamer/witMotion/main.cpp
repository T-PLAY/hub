#include "REG.h"
#include "serial.h"
#include "wit_c_sdk.h"
#include <stdint.h>
#include <iostream>

#include <sensor/OutputSensor.hpp>
// #include <native/native_OutputSensor.hpp>

#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80

static int fd, s_iCurBaud = 9600;
static volatile char s_cDataUpdate = 0;

const int c_uiBaud[] = { 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 };

static void AutoScanSensor( const char* dev );
static void SensorDataUpdata( uint32_t uiReg, uint32_t uiRegNum );
static void Delayms( uint16_t ucMs );

int main( int argc, char* argv[] ) {

    std::string deviceName = "/dev/ttyUSB0";

    // if ( argc < 2 ) {
        // std::cout << "please input dev name\n" << std::endl;
        // return 0;
    // }

    if ( ( fd = serial_open( (unsigned char*)deviceName.c_str(), 9600 ) < 0 ) ) {
        std::cout << "open failed : '" << deviceName << "' not found" << std::endl;
        return 0;
    }
    else
        std::cout << "open " <<  deviceName << " successed" << std::endl;

    float fAcc[3], fGyro[3], fAngle[3];
    int i, ret;
    char cBuff[1];

    WitInit( WIT_PROTOCOL_NORMAL, 0x50 );
    WitRegisterCallBack( SensorDataUpdata );

    std::cout << "\r\n********************** wit-motion Normal example  ************************\r\n" << std::endl;
    AutoScanSensor( deviceName.c_str() );

    using Resolution =
        hub::MatrixTs<hub::sensor::format::Position, hub::sensor::format::Orientation>;
    hub::sensor::SensorSpec::MetaData metaData;
    metaData["manufactor"] = "Shenzhen CO";
    hub::sensor::SensorSpec sensorSpec( "WitMotion", metaData );
    // hub::sensor::OutputSensorT<Resolution> outputSensor(
        // sensorSpec, hub::output::OutputStream( "WitMotionStream" ) );
    // auto acq          = outputSensor.acqMsg();
    // auto& start       = acq.start();
    // auto& end         = acq.end();
    // auto& position    = acq.get<hub::sensor::format::Position&>();
    // auto& orientation = acq.get<hub::sensor::format::Orientation&>();

    while ( 1 ) {

        while ( serial_read_data( fd, (unsigned char*)cBuff, 1 ) ) {
            WitSerialDataIn( cBuff[0] );
        }
        std::cout << "\n" << std::endl;
        Delayms( 500 );

        if ( s_cDataUpdate ) {
            // start = hub::sensor::getClock();

            for ( i = 0; i < 3; i++ ) {
                fAcc[i]   = sReg[AX + i] / 32768.0f * 16.0f;
                fGyro[i]  = sReg[GX + i] / 32768.0f * 2000.0f;
                fAngle[i] = sReg[Roll + i] / 32768.0f * 180.0f;
            }

            if ( s_cDataUpdate & ACC_UPDATE ) {
                std::cout << "acc:" << fAcc[0] << " " << fAcc[1] << " " << fAcc[2] << std::endl;
                s_cDataUpdate &= ~ACC_UPDATE;

                // position.x = fAcc[0];
                // position.y = fAcc[1];
                // position.z = fAcc[2];
            }
            if ( s_cDataUpdate & GYRO_UPDATE ) {
                std::cout << "gyro:" << fGyro[0] << " " << fGyro[1] << " " << fGyro[2] << std::endl;
                s_cDataUpdate &= ~GYRO_UPDATE;
            }
            if ( s_cDataUpdate & ANGLE_UPDATE ) {
                std::cout << "angle:" << fAngle[0] << " " << fAngle[1] << " " << fAngle[2] << std::endl;
                s_cDataUpdate &= ~ANGLE_UPDATE;

                // orientation.rx = fAngle[0];
                // orientation.ry = fAngle[1];
                // orientation.rz = fAngle[2];
            }
            if ( s_cDataUpdate & MAG_UPDATE ) {
                std::cout << "mag:" << sReg[HX] << " " << sReg[HY] << " " << sReg[HZ] << std::endl;
                s_cDataUpdate &= ~MAG_UPDATE;
            }

            // end = hub::sensor::getClock();
            // outputSensor << acq;
        }
    }

    serial_close( fd );
    return 0;
}

static void SensorDataUpdata( uint32_t uiReg, uint32_t uiRegNum ) {
    int i;
    for ( i = 0; i < uiRegNum; i++ ) {
        switch ( uiReg ) {
            //            case AX:
            //            case AY:
        case AZ:
            s_cDataUpdate |= ACC_UPDATE;
            break;
            //            case GX:
            //            case GY:
        case GZ:
            s_cDataUpdate |= GYRO_UPDATE;
            break;
            //            case HX:
            //            case HY:
        case HZ:
            s_cDataUpdate |= MAG_UPDATE;
            break;
            //            case Roll:
            //            case Pitch:
        case Yaw:
            s_cDataUpdate |= ANGLE_UPDATE;
            break;
        default:
            s_cDataUpdate |= READ_UPDATE;
            break;
        }
        uiReg++;
    }
}

static void Delayms( uint16_t ucMs ) {
    usleep( ucMs * 1000 );
}

static void AutoScanSensor(const char *dev ) {
    int i, iRetry;
    char cBuff[1];

    for ( i = 1; i < 10; i++ ) {
        serial_close( fd );
        s_iCurBaud = c_uiBaud[i];
        fd         = serial_open( (unsigned char*)dev, c_uiBaud[i] );

        iRetry = 2;
        do {
            s_cDataUpdate = 0;
            WitReadReg( AX, 3 );
            Delayms( 200 );
            while ( serial_read_data( fd, (unsigned char*)cBuff, 1 ) ) {
                WitSerialDataIn( cBuff[0] );
            }
            if ( s_cDataUpdate != 0 ) {
                std::cout << "sensor baud : " << c_uiBaud[i] << std::endl;
                return;
            }
            iRetry--;
        } while ( iRetry );
    }
    std::cout << "can not find sensor" << std::endl;
    std::cout << "please check your connection" << std::endl;
}
