

//#include <cassert>
#include <iostream>

#include <chrono>
//#include <socket.h>
//#include <stream.h>

#include <NativeStream.h>

int main( int argc, char* argv[] ) {

    auto inputStream = createInputStream( "Keyboard" );



    // const int width = inputStream.getDims().at(0);


    const auto size = getAcquisitionSize( inputStream );
    assert( size == 28 );
    auto data = new unsigned char[size];

    while ( true ) {
        const auto start = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < 10; ++i ) {

            long long start, end;
            auto good = getAcquisition( inputStream, &start, &end, data );
            if ( !good ) exit( 0 );


            std::cout << "receive acq ";

            float* translation = (float*)data;
            float* orientation = (float*)&data[12]; // x, y, z, w

            for ( int i = 0; i < 3; ++i ) {
                std::cout << translation[i] << " ";
            }
            std::cout << "  ";
            for ( int i = 0; i < 4; ++i ) {
                std::cout << orientation[i] << " ";
            }
            std::cout << std::endl;


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
