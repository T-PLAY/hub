
/* NativeStream.cpp : Defines the exported functions for the DLL.
 */

#include "NativeStream.h"

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
NATIVECPPLIBRARY_API InputStream* createInputStream( const char* sensorName ) {
    std::cout << "[NativeStream] createInputStream( " << sensorName << ")" << std::endl;
    return new InputStream( sensorName, "" );
}

NATIVECPPLIBRARY_API void freeinputStream( InputStream* inputStream ) {
    std::cout << "[NativeStream] freeinputStream( " << inputStream << ")" << std::endl;
    delete inputStream;
}

NATIVECPPLIBRARY_API int getAcquisitionSize( InputStream* inputStream ) {
    return inputStream->getAcquisitionSize();
}

// NATIVECPPLIBRARY_API Acq getAcquisition( InputStream* inputStream ) {
NATIVECPPLIBRARY_API bool
getAcquisition( InputStream* inputStream, long long* start, long long* end, unsigned char* data ) {
    *start = 1;
    *end = 2;
    for (int i = 0; i <10; ++i) {
        data[i] = i;
    }
    return true;
    std::cout << "[NativeStream] getAcquisition( " << inputStream << ")" << std::endl;
    //    *acq = inputStream->getAcquisition().clone();
    //    *acq = std::move(acq2.clone());
    auto acq2 = inputStream->getAcquisition();

    //    Acq acq;
    *start = acq2.mBackendTimestamp;
    *end   = acq2.mBackendTimeOfArrival;
    //    data = acq2.mData;
    memcpy( data, acq2.mData, inputStream->getAcquisitionSize() );
    //    acq = new Stream::Acquisition(inputStream->getAcquisition());
    //    return inputStream->Foo(a);
    //    return acq;
    return true;
}
