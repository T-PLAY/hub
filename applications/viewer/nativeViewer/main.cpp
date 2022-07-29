

//#include <cassert>
#include <iostream>

#include <chrono>
//#include <socket.h>
//#include <stream.h>

#include <NativeStream.h>

int main( int argc, char* argv[] ) {

    //    InputStream inputStream(ClientSocket("Keyboard", ""));
    auto inputStream = createInputStream( "Keyboard" );

    //    InputStream inputStream("Keyboard");
    //    const Stream::MetaData& metaData = inputStream.getMetaData();
    //    std::cout << "metadata : " << Stream::to_string(metaData) << std::endl;

    //     InputStream inputStream("L500 Depth Sensor (Depth)");

    //    const size_t acquisitionSize = inputStream.getAcquisitionSize();
    //    std::cout << "acquisitionSize = " << acquisitionSize << std::endl;
    // const int width = inputStream.getDims().at(0);

    //    Stream::Acquisition acq;

    const auto size = getAcquisitionSize( inputStream );
    assert(size == 28);
    auto data       = new unsigned char[size];

    while ( true ) {
        const auto start = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < 10; ++i ) {
            //            inputStream >> acq;

            //            auto acq = inputStream.getAcquisition();
            long long start, end;
            //            Stream::Acquisition * acq;
            auto good = getAcquisition( inputStream, &start, &end, data );
            if ( !good ) exit( 0 );

            //            auto acq = nativeGetAcquisition(inputStream);

            std::cout << "receive acq ";


            float* translation = (float*)data;
            float* orientation = (float*)&data[12]; // x, y, z, w

            for (int i = 0; i <3; ++i) {
                std::cout << translation[i] << " ";
            }
            std::cout << "  ";
            for (int i = 0; i <4; ++i) {
                std::cout << orientation[i] << " ";
            }
            std::cout << std::endl;

            //            for (int i = 0; i <std::min(size, 20); ++i) {
            //                std::cout << (int)data[i] << " ";
            //            }
            //            std::cout << std::endl;
            //            std::cout << *acq << std::endl;

            // const int dec = acq.mData[0];
            // for (size_t i = 0; i < acquisitionSize; ++i) {
            // }
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto fps =
            ( 10.0 * 1'000'000 ) /
            std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
        std::cout << "fps : " << fps << std::endl;
    }
}
