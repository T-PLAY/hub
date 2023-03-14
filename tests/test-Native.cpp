
#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>
// #include <Viewer.hpp>

// #include <Server.hpp>

// #include <filesystem>
// #include <iostream>

#include <Native.hpp>
#include <Server.hpp>

// needs server running

// void onNewStreamer( const char* sensorName, const hub::SensorSpec* sensorSpec ) {

//}
// void onDelStreamer( const char* sensorName, const hub::SensorSpec* sensorSpec ) {

//}

/// \file

// int main() {
TEST_CASE( "Native test" ) {
    constexpr int delay = 1000; // ms

    const std::string ipv4 = "127.0.0.1";
    //    constexpr int port     = 10'001;
//    srand( (unsigned)time( NULL ) );
//    const int port = 6000 + rand() % (65535 - 6000);
    const int port = getRandomPort();

    const hub::Resolution ref_resolution( { 1 }, hub::Format::BGR8 );
    const std::string ref_sensorName = "sensorName";
    hub::SensorSpec::MetaData ref_metaData;
    ref_metaData["a"] = 0;
    ref_metaData["b"] = "string";
    ref_metaData["c"] = 2.0;
    const hub::data::Mat4 ref_mat4(0.0);
    std::cout << "ref_mat4: " << ref_mat4 << std::endl;
    ref_metaData["d"] = ref_mat4;
    const hub::SensorSpec ref_sensorSpec( ref_sensorName, { ref_resolution }, ref_metaData );
    const std::string ref_streamName = "streamName";
    unsigned char ref_data[3]        = { 0, 1, 2 };
    const hub::Acquisition ref_acq =
        std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( ref_data, 3, ref_resolution ) );

    auto onNewStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        const hub::Resolution ref_resolution( { 1 }, hub::Format::BGR8 );
        const std::string ref_sensorName = "sensorName";
        hub::SensorSpec::MetaData ref_metaData;
        ref_metaData["a"] = 0;
        ref_metaData["b"] = "string";
        ref_metaData["c"] = 2.0;
    const hub::data::Mat4 ref_mat4(0.0);
        ref_metaData["d"] = ref_mat4;

        std::cout << "[Example][Viewer] onNewStreamer : " << streamName << std::endl;
        char sensorName[80] = { 0 };
        int strLen          = 0;
        hub::native::sensorSpec_getSensorName( sensorSpec, sensorName, &strLen );
        CHECK( ref_sensorName == sensorName );

        std::cout << "[Example][Viewer] sensorName : '" << sensorName << "' size = " << strLen
                  << std::endl;
        const int resolutionsSize = hub::native::sensorSpec_getResolutionsSize( sensorSpec );
        std::cout << "[Example][Viewer] resolutions size : " << resolutionsSize << std::endl;
        //        CHECK(hub::computeAcquisitionSize(ref_resolution) == resolutionsSize);

        const auto& resolutions = sensorSpec->getResolutions();
        CHECK( resolutions.size() == resolutionsSize );

        for ( int iResolution = 0; iResolution < resolutionsSize; ++iResolution ) {
            const auto& ref_resolution = resolutions.at( iResolution );
            const auto& ref_format     = ref_resolution.second;
            const auto& ref_dims       = ref_resolution.first;

            const auto& format = hub::native::sensorSpec_getFormat( sensorSpec, iResolution );
            std::cout << "[Example][Viewer] resolutions[" << iResolution << "] format : " << format
                      << std::endl;
            CHECK( format == static_cast<int>( ref_format ) );

            const int resolutionSize =
                hub::native::sensorSpec_getResolutionSize( sensorSpec, iResolution );
            CHECK( resolutionSize == hub::computeAcquisitionSize( ref_resolution ) );

            const int dimensionsSize =
                hub::native::sensorSpec_getDimensionsSize( sensorSpec, iResolution );
            std::cout << "[Example][Viewer] resolutions[" << iResolution
                      << "] dimensions size : " << dimensionsSize << std::endl;
            CHECK( hub::computeAcquisitionSize( ref_resolution ) ==
                   hub::computeAcquisitionSize( ref_format, ref_dims ) );
            //            CHECK(dimensionsSize == hub::computeAcquisitionSize(ref_resolution));
            CHECK( dimensionsSize == ref_dims.size() );

            for ( int iDimension = 0; iDimension < dimensionsSize; ++iDimension ) {
                const auto dim =
                    hub::native::sensorSpec_getDimension( sensorSpec, iResolution, iDimension );

                std::cout << "[Example][Viewer] resolutions[" << iResolution << "] dimensions["
                          << iDimension << "] size : "
                          << hub::native::sensorSpec_getDimension(
                                 sensorSpec, iResolution, iDimension )
                          << std::endl;

                CHECK( dim == ref_dims.at( iDimension ) );
            }
        }
        std::cout << "[Example][Viewer] acquisitionsSize : "
                  << hub::native::sensorSpec_getAcquisitionSize( sensorSpec ) << std::endl;

        char resolutionsStr[80] = { 0 };
        hub::native::sensorSpec_getResolutionsStr( sensorSpec, resolutionsStr );
        std::cout << "[Example][Viewer] resolutionsStr : '" << resolutionsStr << "'" << std::endl;

        char metaDataStr[80] = { 0 };
        hub::native::sensorSpec_getMetaDataStr( sensorSpec, metaDataStr );
        std::cout << "[Example][Viewer] metaDataStr : '" << metaDataStr << "'" << std::endl;

        hub::SensorSpec* sensorSpecCopy = hub::native::sensorSpec_copy( sensorSpec );
        CHECK( *sensorSpecCopy == *sensorSpec );

        const hub::SensorSpec::MetaData* metaData =
            hub::native::sensorSpec_getMetaData( sensorSpec );
        CHECK( hub::native::metaData_exists( metaData, "a" ) );
        CHECK( hub::native::metaData_getInt( metaData, "a" ) == ref_metaData.at( "a" ).getInt() );
        CHECK( hub::native::metaData_getDouble( metaData, "c" ) ==
               ref_metaData.at( "c" ).getDouble() );
        CHECK( !hub::native::metaData_exists( metaData, "z" ) );
        //        CHECK(ref_metaData == metaData);
        float * data_mat4 = new float[16];
        hub::native::metaData_getMat4(metaData, "d", data_mat4);
        hub::data::Mat4 mat4(data_mat4);
        CHECK(mat4 == ref_mat4);

        CHECK(! hub::native::metaData_getMat4(metaData, "fake", nullptr));

        char metaDataString[80] = { 0 };
        int len;
        hub::native::metaData_getString( metaData, "b", metaDataString, &len );
        const char* ref_str = ref_metaData.at( "b" ).getConstCharPtr();
        std::cout << "ref_str = " << ref_str << std::endl;
        CHECK( len == strlen( ref_str ) );
        CHECK( !strcmp( ref_str, metaDataString ) );

        return true;
    };
    auto onDelStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "[Example][Viewer] onDelStreamer " << streamName << std::endl;
    };
    auto onServerConnected = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerConnected " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerDisconnected " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const char* streamName, const hub::Acquisition* acq ) {
        CHECK( !strcmp( streamName, "streamName" ) );
        std::cout << "[Example][Viewer] onNewAcquisition " << streamName << " " << *acq
                  << std::endl;
    };
    auto onLogMessage = []( const char* logMessage ) {
        std::cout << "[Example][Viewer] onLogMessage " << logMessage << " " << *logMessage
                  << std::endl;
    };

    auto* viewer = hub::native::createViewer( onNewStreamer,
                                              onDelStreamer,
                                              onServerConnected,
                                              onServerDisconnected,
                                              onNewAcquisition,
                                              ipv4.c_str(),
                                              port,
                                              onLogMessage );
    hub::native::viewer_setIpv4( viewer, ipv4.c_str() );
    hub::native::viewer_setPort( viewer, port );
    CHECK( !viewer->isConnected() );
    CHECK(! hub::native::viewer_isConnected(viewer));

    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    {
        std::cout << "[Test] ############################### server start" << std::endl;
        Server server( port );
        server.setMaxClients( 5 );
        //    server.setAcqPing( false );
        server.asyncRun();
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
        CHECK( viewer->isConnected() );
        CHECK(hub::native::viewer_isConnected(viewer));
        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;

            hub::OutputSensor* outputSensor = new hub::OutputSensor(
                ref_sensorSpec,
                hub::io::OutputStream( ref_streamName, hub::net::ClientSocket( ipv4, port ) ) );

            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
            {
                *outputSensor << ref_acq;

                std::cout << "[Test] ############################### inputSensor start"
                          << std::endl;

                auto* inputSensor =
                    hub::native::createInputSensor( ref_streamName.c_str(), ipv4.c_str(), port );

                auto acquisitionSize = hub::native::getAcquisitionSize( inputSensor );
                CHECK( ref_acq.getSize() == acquisitionSize );

                auto* acq = hub::native::getAcquisition( inputSensor );
                CHECK( ref_acq == *acq );

                unsigned char data[256];
                hub::native::acquisition_getMeasure( acq, data, 0 );
                CHECK( !memcmp( ref_data, data, 3 ) );

                auto start = hub::native::acquisition_getStart( acq );
                CHECK( start == ref_acq.m_start );

                hub::native::freeAcquisition( acq );

                char buff[256];
                int len;
                hub::native::acquisition_to_string( acq, buff, &len );
                //                CHECK(len == ref_acq.w)

                hub::native::freeInputSensor( inputSensor );

                //                try {
                auto* inputSensor2 =
                    hub::native::createInputSensor( "nonConnected", ipv4.c_str(), port );
                CHECK( inputSensor2 == nullptr );
                //                }
                //                catch ( std::exception& ex ) {
                //                    CHECK( true );
                //                }

                auto & output = outputSensor->getOutput();
                output.close();
                delete outputSensor;

            std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

//                try {
            // todo native
//                    hub::Acquisition* acq2 = hub::native::getAcquisition( inputSensor );
//                CHECK(acq2 == nullptr);
//                    CHECK( false );
//                }
//                catch ( std::exception& ex ) {
//                    CHECK( true );
//                }

                std::cout << "[Test] ############################### inputSensor end" << std::endl;
            }

            //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            //            *outputSensor << ref_acq;

            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            //          hub::native::createInputSensor()

            std::cout << "[Test] ############################### outputSensor end" << std::endl;
        }
        std::cout << "[Test] ############################### server end" << std::endl;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    hub::native::freeViewer( viewer );

    //    return 0;
}
