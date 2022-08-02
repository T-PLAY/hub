

/* NativeStream.cpp : Defines the exported functions for the DLL.
 */

#include "NativeStream.h"

#ifndef NO_NATIVE

#include <iostream>
#include <stream.h>

// static std::map<int, std::unique_ptr<InputStream>> s_id2inputStream;
// int createInputStream( const char* sensorName ) {
//    int id = 0;
//    while ( s_id2inputStream.find( id ) != s_id2inputStream.end() )
//        ++id;

//    assert( s_id2inputStream.find( id ) == s_id2inputStream.end() );
//    s_id2inputStream[id] = std::make_unique<InputStream>( ClientSocket( sensorName, "" ) );

//    return id;
//}

// int getAcquisitionSize( int id ) {
//    assert( s_id2inputStream.find( id ) != s_id2inputStream.end() );

//    return s_id2inputStream.at( id )->getAcquisitionSize();
//}

// bool getAcquisition( int id, long long* start, long long* end, unsigned char* data ) {
//    assert( s_id2inputStream.find( id ) != s_id2inputStream.end() );

//    try {
//        const auto& inputStream = s_id2inputStream.at( id );
//        const auto& acq         = inputStream->getAcquisition();
//        *start                  = acq.mBackendTimestamp;
//        *end                    = acq.mBackendTimeOfArrival;
//        memcpy( data, acq.mData, inputStream->getAcquisitionSize() );
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[NativeStream] catch exception : " << e.what() << std::endl;
//        return false;
//    }
//    return true;
//}

// extern "C" NATIVECPPLIBRARY_API InputStream* createInputStream();
// extern "C" NATIVECPPLIBRARY_API void freeInputStream(InputStream* instance);
// extern "C" NATIVECPPLIBRARY_API int getAcquisition(InputStream* instance, Stream::Acquisition*
// acq);
InputStream* createInputStream( const char* sensorName ) {
    std::cout << "[NativeStream] createInputStream( " << sensorName << ")" << std::endl;
    InputStream* inputStream;
    try {
        inputStream = new InputStream( sensorName, "" );
    }
    catch ( std::exception& e ) {
        std::cout << "[NativeStream] createInputStream : catch exception : " << e.what()
                  << std::endl;
        return nullptr;
    }

    return inputStream;
}

void freeInputStream( InputStream* inputStream ) {
    assert( inputStream != nullptr );

    std::cout << "[NativeStream] freeinputStream( " << inputStream << ")" << std::endl;
    delete inputStream;
}

int getAcquisitionSize( InputStream* inputStream ) {
    assert( inputStream != nullptr );

    return static_cast<int>(inputStream->getAcquisitionSize());
}

bool getData( InputStream* inputStream, unsigned char* data ) {
    assert( inputStream != nullptr );

//    for (int i = 0; i <28; ++i) {
//        data[i] = i;
//    }

    std::cout << "[NativeStream] getData( " << inputStream << ")" << std::endl;

    try {
        auto acq = inputStream->getAcquisition();
        //    Acq acq;
//        *start = acq.mBackendTimestamp;
//        *end   = acq.mBackendTimeOfArrival;
        //    data = acq.mData;
        std::cout << "[NativeStream] get acq : " << acq << std::endl;

        std::cout << "[NativeStream] copying data " << std::endl;
        memcpy( data, acq.mData, inputStream->getAcquisitionSize() );
    }
    catch ( std::exception& e ) {
        std::cout << "[NativeStream] getAcquisition : catch exception : " << e.what() << std::endl;
        return false;
    }

    //    acq = new Stream::Acquisition(inputStream->getAcquisition());
    //    return inputStream->Foo(a);
    //    return acq;
    return true;
}

// NATIVECPPLIBRARY_API Acq getAcquisition( InputStream* inputStream ) {
bool
getAcquisition( InputStream* inputStream, long long* start, long long* end, unsigned char* data ) {
    assert( inputStream != nullptr );

//    *start = 1;
//    *end   = 2;
//    for ( int i = 0; i < 10; ++i ) {
//        data[i] = i;
//    }
//    return true;
//    Stream::Acquisition* acq = nullptr;

    std::cout << "[NativeStream] getAcquisition( " << inputStream << ")" << std::endl;
    //    *acq = inputStream->getAcquisition().clone();
    //    *acq = std::move(acq.clone());
    try {
        auto acq = inputStream->getAcquisition();
        //    Acq acq;
        *start = acq.mBackendTimestamp;
        *end   = acq.mBackendTimeOfArrival;
        //    data = acq.mData;
        memcpy( data, acq.mData, inputStream->getAcquisitionSize() );
    }
    catch ( std::exception& e ) {
        std::cout << "[NativeStream] getAcquisition : catch exception : " << e.what() << std::endl;
        return false;
    }

    //    acq = new Stream::Acquisition(inputStream->getAcquisition());
    //    return inputStream->Foo(a);
    //    return acq;
    return true;
}

#endif
