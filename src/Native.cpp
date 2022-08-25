#include "Native.hpp"

/* Native.cpp : Defines the exported functions for the DLL.
 */

#include <iostream>

#include "IO/Stream.hpp"
#include "Net/ClientSocket.hpp"
#include <cstring>

namespace hub {

InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port ) {
    std::cout << "[Native] createInputSensor( " << streamName << ")" << std::endl;
    InputSensor* inputSensor = nullptr;
    try {
        inputSensor =
            new InputSensor( io::InputStream( streamName, "", net::ClientSocket( ipv4, port ) ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] createInputSensor : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return inputSensor;
}

void freeInputSensor( InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    std::cout << "[Native] freeinputSensor( " << inputSensor << ")" << std::endl;
    delete inputSensor;
}

int getAcquisitionSize( InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    return static_cast<int>( inputSensor->m_spec.m_acquisitionSize );
}

bool getData( InputSensor* inputSensor, unsigned char* data ) {
    assert( inputSensor != nullptr );

    std::cout << "[Native] getData( " << inputSensor << ")" << std::endl;

    try {
        auto acq = inputSensor->getAcquisition();
        std::cout << "[Native] get acq : " << acq << std::endl;

        std::cout << "[Native] copying data " << std::endl;
        memcpy( data, acq.m_data, inputSensor->m_spec.m_acquisitionSize );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return false;
    }

    return true;
}

// bool getAcquisition( InputSensor* inputSensor,
//                      long long* start,
//                      long long* end,
//                      unsigned char* data ) {
//     assert( inputSensor != nullptr );

//    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
//    try {
//        auto acq = inputSensor->getAcquisition();
//        //    Acq acq;
//        *start = acq.start;
//        *end   = acq.mBackendTimeOfArrival;
//        //    data = acq.mData;
//        memcpy( data, acq.mData, inputSensor->spec.acquisitonSize );
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
//        return false;
//    }

//    return true;
//}

} // namespace hub
