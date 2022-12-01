

#include <cassert>
#include <iostream>

#include <chrono>
//#include <socket.h>
//#include <stream.h>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>

int main( int argc, char* argv[] ) {
    std::string sensorNames[3]       = { "proceduralStreamer", "proceduralStreamer2", "Keyboard" };
    std::string sensorMasterNames[3] = { "", "proceduralStreamer", "" };
    // int i = atoi(argv[1]);
    int iSensor = 2;

    std::cout << "inputStream" << std::endl;
    hub::InputSensor inputStream(
        hub::io::InputStream( sensorNames[iSensor], sensorMasterNames[iSensor] ) );
    std::cout << "proceduralStreamer inited" << std::endl;
    const auto& header                        = inputStream.m_spec;
    const hub::SensorSpec::MetaData& metaData = header.getMetaData();

    std::cout << "metadata : " << hub::SensorSpec::metaData2string( metaData ) << std::endl;
    if ( iSensor != 2 ) {
        assert( std::any_cast<double>( metaData.at( "depth" ) ) == 3.0 );
        std::string name( std::any_cast<const char*>( metaData.at( "name" ) ) );
        assert( name == "L533" );
        assert( std::string( std::any_cast<const char*>( metaData.at( "name" ) ) ) == "L533" );
    }

    // hub::InputSensor inputStream("L500 Depth Sensor (Depth)");

    const size_t acquisitionSize = header.getAcquisitionSize();
    std::cout << "acquisitionSize = " << acquisitionSize << std::endl;
    // const int width = inputStream.getDims().at(0);

    while ( true ) {
        const auto start = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < 10; ++i ) {
            auto acq = inputStream.getAcquisition();

            std::cout << acq << std::endl;

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
