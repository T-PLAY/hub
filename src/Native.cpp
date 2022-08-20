#include "Native.hpp"

/* Native.cpp : Defines the exported functions for the DLL.
 */

#ifndef NO_NATIVE

#    include <iostream>

#    include "Net/Socket.hpp"

namespace hub {
// static std::map<int, std::unique_ptr<hub::InputSensor>> s_id2inputSensor;
// int createhub::InputSensor( const char* sensorName ) {
//    int id = 0;
//    while ( s_id2inputSensor.find( id ) != s_id2inputSensor.end() )
//        ++id;

//    assert( s_id2inputSensor.find( id ) == s_id2inputSensor.end() );
//    s_id2inputSensor[id] = std::make_unique<hub::InputSensor>( ClientSocket( sensorName, "" ) );

//    return id;
//}

// int getAcquisitionSize( int id ) {
//    assert( s_id2inputSensor.find( id ) != s_id2inputSensor.end() );

//    return s_id2inputSensor.at( id )->getAcquisitionSize();
//}

// bool getAcquisition( int id, long long* start, long long* end, unsigned char* data ) {
//    assert( s_id2inputSensor.find( id ) != s_id2inputSensor.end() );

//    try {
//        const auto& inputSensor = s_id2inputSensor.at( id );
//        const auto& acq         = inputSensor->getAcquisition();
//        *start                  = acq.mBackendTimestamp;
//        *end                    = acq.mBackendTimeOfArrival;
//        memcpy( data, acq.mData, inputSensor->getAcquisitionSize() );
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[Native] catch exception : " << e.what() << std::endl;
//        return false;
//    }
//    return true;
//}

// extern "C" NATIVECPPLIBRARY_API hub::InputSensor* createhub::InputSensor();
// extern "C" NATIVECPPLIBRARY_API void freehub::InputSensor(hub::InputSensor* instance);
// extern "C" NATIVECPPLIBRARY_API int getAcquisition(hub::InputSensor* instance,
// Stream::Acquisition* acq);
InputSensor* createInputSensor( const char* sensorName, const char* ipv4 ) {
    std::cout << "[Native] createInputSensor( " << sensorName << ")" << std::endl;
    InputSensor* inputSensor;
    try {
//        inputSensor = new InputSensor( ClientSocket( sensorName, "", ipv4, SERVICE_PORT ) );
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

    return static_cast<int>( inputSensor->spec.acquisitonSize );
}

bool getData( InputSensor* inputSensor, unsigned char* data ) {
    assert( inputSensor != nullptr );

    //    for (int i = 0; i <28; ++i) {
    //        data[i] = i;
    //    }

    std::cout << "[Native] getData( " << inputSensor << ")" << std::endl;

    try {
        auto acq = inputSensor->getAcquisition();
        //    Acq acq;
        //        *start = acq.mBackendTimestamp;
        //        *end   = acq.mBackendTimeOfArrival;
        //    data = acq.mData;
        std::cout << "[Native] get acq : " << acq << std::endl;

        std::cout << "[Native] copying data " << std::endl;
        memcpy( data, acq.mData, inputSensor->spec.acquisitonSize );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return false;
    }

    //    acq = new Stream::Acquisition(inputSensor->getAcquisition());
    //    return inputSensor->Foo(a);
    //    return acq;
    return true;
}

// NATIVECPPLIBRARY_API Acq getAcquisition( InputSensor* inputSensor ) {
bool getAcquisition( InputSensor* inputSensor,
                     long long* start,
                     long long* end,
                     unsigned char* data ) {
    assert( inputSensor != nullptr );

    //    *start = 1;
    //    *end   = 2;
    //    for ( int i = 0; i < 10; ++i ) {
    //        data[i] = i;
    //    }
    //    return true;
    //    Stream::Acquisition* acq = nullptr;

    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
    //    *acq = inputSensor->getAcquisition().clone();
    //    *acq = std::move(acq.clone());
    try {
        auto acq = inputSensor->getAcquisition();
        //    Acq acq;
        *start = acq.mBackendTimestamp;
        *end   = acq.mBackendTimeOfArrival;
        //    data = acq.mData;
        memcpy( data, acq.mData, inputSensor->spec.acquisitonSize );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return false;
    }

    //    acq = new Stream::Acquisition(inputSensor->getAcquisition());
    //    return inputSensor->Foo(a);
    //    return acq;
    return true;
}

#endif

} // namespace hub
