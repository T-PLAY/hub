
#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>
// #include <Viewer.hpp>

// #include <Server.hpp>

// #include <iostream>

#include <native/Native.hpp>
#include <server/Server.hpp>

// needs server running

// void onNewStreamer( const char* sensorName, const hub::sensor::SensorSpec* sensorSpec ) {

// void onDelStreamer( const char* sensorName, const hub::sensor::SensorSpec* sensorSpec ) {

/// \file

TEST_CASE( "Native test" ) {
    //    constexpr int delay = 1000; // ms

    //    const std::string ipv4 = "127.0.0.1";
    const int port = GET_RANDOM_PORT;
    //    const int port         = 1883;
    hub::Server server( port );
    server.asyncRun();

    // const hub::sensor::Resolution ref_resolution( { 1 }, hub::sensor::Format::BGR8 );
    const auto ref_resolution        = hub::make_matrix<hub::sensor::format::BGR8>();
    const std::string ref_sensorName = "sensorName";
    hub::sensor::SensorSpec::MetaData ref_metaData;
    ref_metaData["a"] = 0;
    ref_metaData["b"] = "string";
    ref_metaData["c"] = 2.0;
    // const hub::data::Mat4 ref_mat4( 0.0 );
    const hub::sensor::format::MAT4 ref_mat4( 0.0 );

    std::cout << "ref_mat4: " << ref_mat4 << std::endl;
    ref_metaData["d"] = ref_mat4;
    const hub::sensor::SensorSpec ref_sensorSpec(
        ref_sensorName, { ref_resolution }, ref_metaData );
    // const std::string ref_streamName = FILE_NAME;
    // unsigned char ref_data[3]        = { 0, 1, 2 };
    // const hub::sensor::Acquisition ref_acq =
    // std::move( hub::sensor::Acquisition( 0, 1 ) << hub::Measure( ref_data, 3, ref_resolution ) );
    auto ref_acq    = hub::sensor::make_acquisition( ref_resolution );
    ref_acq.start() = 0;
    ref_acq.end()   = 1;
    auto& bgr8      = ref_acq.get<hub::sensor::format::BGR8&>();
    bgr8.b          = 0;
    bgr8.g          = 1;
    bgr8.r          = 2;

    auto onServerNotFound = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerNotFound " << ipv4 << " " << port << std::endl;
    };
    auto onServerConnected = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerConnected " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerDisconnected " << ipv4 << " " << port << std::endl;
    };
    static int nStreamerInited = 0;
    auto onNewStreamer = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
        //        return false;
        // const hub::sensor::Resolution ref_resolution( { 1 }, hub::sensor::Format::BGR8 );
        const auto ref_resolution        = hub::make_matrix<hub::sensor::format::BGR8>();
        const std::string ref_sensorName = "sensorName";
        hub::sensor::SensorSpec::MetaData ref_metaData;
        ref_metaData["a"] = 0;
        ref_metaData["b"] = "string";
        ref_metaData["c"] = 2.0;
        // const hub::data::Mat4 ref_mat4( 0.0 );
        const hub::sensor::format::MAT4 ref_mat4( 0.0 );
        ref_metaData["d"] = ref_mat4;

        std::cout << "[Example][Viewer] onNewStreamer : " << streamName << std::endl;
        char sensorName[80] = { 0 };
        int strLen          = 0;
        hub::native::sensorSpec_getSensorName( sensorSpec, sensorName, &strLen );
        CHECK( ref_sensorName == sensorName );

        // std::cout << "[Example][Viewer] sensorName : '" << sensorName << "' size = " << strLen
        //           << std::endl;
        // const int resolutionsSize = hub::native::sensorSpec_getResolutionsSize( sensorSpec );
        // std::cout << "[Example][Viewer] resolutions size : " << resolutionsSize << std::endl;

        // const auto& resolutions = sensorSpec->getResolutions();
        // CHECK( resolutions.size() == resolutionsSize );

        // for ( int iResolution = 0; iResolution < resolutionsSize; ++iResolution ) {
        //     const auto& ref_resolution2 = resolutions.at( iResolution );
        //     const auto& ref_format      = ref_resolution2.second;
        //     const auto& ref_dims        = ref_resolution2.first;

        //     const auto& format = hub::native::sensorSpec_getFormat( sensorSpec, iResolution );
        //     std::cout << "[Example][Viewer] resolutions[" << iResolution << "] format : " <<
        //     format
        //               << std::endl;
        //     CHECK( format == static_cast<int>( ref_format ) );

        //     const int resolutionSize =
        //         hub::native::sensorSpec_getResolutionSize( sensorSpec, iResolution );
        //     CHECK( resolutionSize ==
        //            hub::sensor::resolution::computeAcquisitionSize( ref_resolution ) );

        //     const int dimensionsSize =
        //         hub::native::sensorSpec_getDimensionsSize( sensorSpec, iResolution );
        //     std::cout << "[Example][Viewer] resolutions[" << iResolution
        //               << "] dimensions size : " << dimensionsSize << std::endl;
        //     CHECK( hub::sensor::resolution::computeAcquisitionSize( ref_resolution ) ==
        //            hub::sensor::resolution::computeAcquisitionSize( ref_format, ref_dims ) );
        //     CHECK( dimensionsSize == ref_dims.size() );

        //     for ( int iDimension = 0; iDimension < dimensionsSize; ++iDimension ) {
        //         const auto dim =
        //             hub::native::sensorSpec_getDimension( sensorSpec, iResolution, iDimension );

        //         std::cout << "[Example][Viewer] resolutions[" << iResolution << "] dimensions["
        //                   << iDimension << "] size : "
        //                   << hub::native::sensorSpec_getDimension(
        //                          sensorSpec, iResolution, iDimension )
        //                   << std::endl;

        //         CHECK( dim == ref_dims.at( iDimension ) );
        //     }
        // }
        // std::cout << "[Example][Viewer] acquisitionsSize : "
        //           << hub::native::sensorSpec_getAcquisitionSize( sensorSpec ) << std::endl;

        // char resolutionsStr[80] = { 0 };
        // hub::native::sensorSpec_getResolutionsStr( sensorSpec, resolutionsStr );
        // std::cout << "[Example][Viewer] resolutionsStr : '" << resolutionsStr << "'" <<
        // std::endl;

        char metaDataStr[256] = { 0 };
        hub::native::sensorSpec_getMetaDataStr( sensorSpec, metaDataStr );
        std::cout << "[Example][Viewer] metaDataStr : '" << metaDataStr << "'" << std::endl;

        const hub::sensor::SensorSpec* sensorSpecCopy = hub::native::sensorSpec_copy( sensorSpec );
        CHECK( *sensorSpecCopy == *sensorSpec );

        const hub::sensor::SensorSpec::MetaData* metaData =
            hub::native::sensorSpec_getMetaData( sensorSpec );
        CHECK( hub::native::metaData_exists( metaData, "a" ) );
        CHECK( hub::native::metaData_getInt( metaData, "a" ) == ref_metaData.at( "a" ).get<int>() );
        CHECK( hub::native::metaData_getDouble( metaData, "c" ) ==
               ref_metaData.at( "c" ).get<double>() );
        CHECK( !hub::native::metaData_exists( metaData, "z" ) );
        float* data_mat4 = new float[16];
        hub::native::metaData_getMat4( metaData, "d", data_mat4 );
        // hub::data::Mat4 mat4( data_mat4 );
        hub::sensor::format::MAT4 mat4( data_mat4 );
        CHECK( mat4 == ref_mat4 );

        CHECK( !hub::native::metaData_getMat4( metaData, "fake", nullptr ) );

        char metaDataString[80] = { 0 };
        int len;
        hub::native::metaData_getString( metaData, "b", metaDataString, &len );
        const char* ref_str = ref_metaData.at( "b" ).get<const char*>();
        std::cout << "ref_str = " << ref_str << std::endl;
        CHECK( len == strlen( ref_str ) );
        CHECK( !strcmp( ref_str, metaDataString ) );

        ++nStreamerInited;
        return true;
    };
    auto onDelStreamer = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
        std::cout << "[Example][Viewer] onDelStreamer " << streamName << std::endl;
    };
    static int nReceiveAcq = 0;
    auto onNewAcquisition  = []( const char* streamName, const hub::sensor::Acquisition* acq ) {
        CHECK( !strcmp( streamName, FILE_NAME.c_str() ) );
        std::cout << "[Example][Viewer] onNewAcquisition " << streamName << " " << *acq
                  << std::endl;
        ++nReceiveAcq;
    };
    auto onSetProperty =
        []( const char* streamName, const char* objectName, int property, const hub::Any* value ) {
            std::cout << "[Example][Viewer] onSetProperty " << streamName << " " << objectName
                      << " " << property << " " << value << std::endl;
        };
    auto onLogMessage = []( const char* logMessage ) {
        std::cout << "[Example][Viewer] onLogMessage " << logMessage << " " << *logMessage
                  << std::endl;
    };

    auto* viewerHandler = hub::native::createViewerHandler( onServerNotFound,
                                                            onServerConnected,
                                                            onServerDisconnected,
                                                            onNewStreamer,
                                                            onDelStreamer,
                                                            onNewAcquisition,
                                                            onSetProperty,
                                                            onLogMessage );

    //    hub::native::viewer_setIpv4( viewer, ipv4.c_str() );
    //    hub::native::viewer_setPort( viewer, port );

    auto* viewer =
        hub::native::createViewer( FILE_NAME.c_str(), viewerHandler, "127.0.0.1", port);
                                   //                                              ipv4.c_str(),
                                   //        port
        // );

    // todo test
    //    if ( false )
    {
        std::cout << "[Test] ############################### server start" << std::endl;
        //        hub::Server server( port );
        //        server.setMaxClients( 2 );
        //        server.asyncRun();
        int iTryConnect = 0;
        while ( !viewer->isConnected() && iTryConnect < 10 ) {
            std::cout << "[test] waiting for viewer connected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTryConnect;
        }
        CHECK( viewer->isConnected() );
        assert( viewer->isConnected() );
        CHECK( hub::native::viewer_isConnected( viewer ) );
        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;

            hub::sensor::OutputSensor* outputSensor = new hub::sensor::OutputSensor(
                //                ref_sensorSpec, ref_streamName, hub::net::ClientSocket( ipv4, port
                //                ) ); ref_sensorSpec, ref_streamName, ipv4, port );
                ref_sensorSpec,
                hub::output::OutputStream( FILE_NAME, port ) );

            while ( nStreamerInited == 0 ) {
                std::cout << "[Test] waiting for streamer inited" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }

            ////////////////////////////////////////////////////

            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            *outputSensor << ref_acq;

            while ( nReceiveAcq == 0 ) {
                std::cout << "[Test] waiting for acq received" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                *outputSensor << ref_acq;
            }

            //            std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

            //            if ( false )
            //            {

            std::cout << "[Test] ############################### inputSensor start" << std::endl;

            //                auto* inputSensor =
            //                    hub::native::createInputSensor( ref_streamName.c_str());

            //                auto acquisitionSize = hub::native::getAcquisitionSize( inputSensor );
            //                CHECK( ref_acq.getSize() == acquisitionSize );

            //                auto* acq = hub::native::getAcquisition( inputSensor );
            //                CHECK( ref_acq == *acq );

            //                unsigned char data[256];
            //                hub::native::acquisition_getMeasure( acq, data, 0 );
            //                CHECK( !memcmp( ref_data, data, 3 ) );

            //                auto start = hub::native::acquisition_getStart( acq );
            //                CHECK( start == ref_acq.getStart() );

            //                hub::native::freeAcquisition( acq );

            //                char buff[256];
            //                int len;
            //                hub::native::acquisition_to_string( acq, buff, &len );

            //                hub::native::freeInputSensor( inputSensor );

            //                const auto* inputSensor2 =
            //                    hub::native::createInputSensor( "nonConnected" );
            //                CHECK( inputSensor2 == nullptr );

            //                auto& output = outputSensor->getOutput();
            //                output.close();
            //                delete outputSensor;

            // todo native

            std::cout << "[Test] ############################### inputSensor end" << std::endl;
            //            }

            delete outputSensor;

            std::cout << "[Test] ############################### outputSensor end" << std::endl;
        }
        //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        std::cout << "[Test] ############################### server end" << std::endl;
    }

    hub::native::freeViewer( viewer );
}
