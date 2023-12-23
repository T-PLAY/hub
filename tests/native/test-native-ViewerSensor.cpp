
#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

// #include <InputSensor.hpp>
// #include <NativeViewer.hpp>

// #include <Server.hpp>

// #include <iostream>

#include <native/Native.hpp>
#include <server/Server.hpp>

// #include <sensor/OutputSensor.hpp>

// needs server running

// void onNewStreamer( const char* sensorName, const hub::sensor::SensorSpec* sensorSpec ) {

// void onDelStreamer( const char* sensorName, const hub::sensor::SensorSpec* sensorSpec ) {

/// \file

TEST_CASE( "Native Viewer" ) {

    return;

    TEST_BEGIN()

    const int port = GET_RANDOM_PORT;

    // static const hub::io::Header header_ref { sizeof(int) };

    // using Resolution = hub::sensor::format::BGR8;
    // hub::MetaData metaData;
    // metaData["user"] = "gauthier";
    // const hub::sensor::SensorSpec sensorSpec_ref {
    //     "sensorName", hub::make_matrix<Resolution>(), metaData };

    {

        // hub::client::ViewerHandler viewerHandler;

        static int nNewStreamer                     = 0;
        static int nDelStreamer                     = 0;
        static int nServerNotFound                  = 0;
        static int nServerConnected                 = 0;
        static std::atomic<int> nServerDisconnected = 0;
        static int nNewData                         = 0;

        auto onServerNotFound = []( const char* ipv4, int port ) {
            std::cout << "[test-client-NativeViewer] onServerNotFound : " << ipv4 << " " << port
                      << std::endl;
            ++nServerNotFound;
        };
        auto onServerConnected = []( const char* ipv4, int port ) {
            std::cout << "[test-client-NativeViewer] onServerConnected : " << ipv4 << " " << port
                      << std::endl;
            ++nServerConnected;
        };
        auto onServerDisconnected = []( const char* ipv4, int port ) {
            std::cout << "[test-client-NativeViewer] onServerDisconnected : " << ipv4 << " " << port
                      << std::endl;
            ++nServerDisconnected;
        };
        auto onNewStream = []( const char* streamName, const hub::io::Header* header ) {
            std::cout << "[test-client-NativeViewer] onNewStream : " << streamName << ", " << header
                      << std::endl;
            assert( streamName == FILE_NAME );
            // assert( header_ref == *header );
            ++nNewStreamer;
            return true;
        };
#ifndef HUB_NON_BUILD_SENSOR
        auto onNewSensor = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
            assert( false );
            std::cout << "[test-client-NativeViewer] onNewStream : " << streamName << ", "
                      << sensorSpec << std::endl;
            assert( streamName == FILE_NAME );
            // assert( header_ref == *sensorSpec );
            ++nNewStreamer;
            return true;
        };
#endif
        // hub::io::Memory memory;
        auto onNewData = []( const char* streamName,
                             // hub::input::InputStream& inputStream ) {
                             const hub::Datas_t* datas ) {
            assert( streamName == FILE_NAME );
            // assert( inputStream.getHeader() == header_ref );
            int a;
            memcpy( &a, datas->data(), datas->size() );
            // memory.write(datas.data(), datas.size());
            // memory.read(a);
            assert( a == nNewData );
            // inputStream.read( a );
            // std::cout << "[test-client-NativeViewer] onNewData : " << a << std::endl;
            std::cout << "[test-client-NativeViewer] onNewData : " << *datas << std::endl;
            ++nNewData;
        };
#ifndef HUB_NON_BUILD_SENSOR
        auto onNewAcq = []( const char* streamName, const hub::sensor::Acquisition* acq ) {
            assert( false );
        };
#endif
        auto onDelStream = []( const char* streamName ) {
            assert( streamName == FILE_NAME );
            std::cout << "[test-client-NativeViewer] onDelStream : " << streamName << std::endl;
            ++nDelStreamer;
        };
        auto onSetProperty = []( const char* streamName,
                                 const char* objectName,
                                 int property,
                                 const hub::Any* value ) {
            assert( streamName == FILE_NAME );
            std::cout << "[test-client-NativeViewer] onSetProperty " << streamName << std::endl;
        };
        auto onLogMessage = []( const char* logMessage ) {
            std::cout << "[test-client-NativeViewer] onLogMessage '" << logMessage << "'"
                      << std::endl;
        };

        auto* viewerHandler = hub::native::createViewerHandler( onServerNotFound,
                                                                onServerConnected,
                                                                onServerDisconnected,
                                                                onNewStream,
#ifndef HUB_NON_BUILD_SENSOR
                                                                onNewSensor,
#endif
                                                                onNewData,
#ifndef HUB_NON_BUILD_SENSOR
                                                                onNewAcq,
#endif
                                                                onDelStream,
                                                                onSetProperty,
                                                                onLogMessage );

        CONSTRUCT_BEGIN( "NativeViewer" );
        // hub::client::Viewer viewer { FILE_NAME, std::move( viewerHandler ), "127.0.0.1", port };
        auto* viewer =
            hub::native::createViewer( FILE_NAME.c_str(), viewerHandler, "127.0.0.1", port );
        CONSTRUCT_END( "NativeViewer" );

        assert( nServerDisconnected == 0 );
        assert( !hub::native::viewer_isConnected( viewer ) );
        while ( nServerNotFound == 0 ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[test] waiting for server not found" << std::endl;
        }
        int iTry = 0;

        {
            CONSTRUCT_BEGIN( "Server" );
            hub::Server server( port );
            CONSTRUCT_END( "Server" );
            server.asyncRun();

            while ( !hub::native::viewer_isConnected( viewer ) && iTry < 20 ) {
                std::cout << "[test] waiting for viewer connected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 20 );
            assert( hub::native::viewer_isConnected( viewer ) );
            assert( nServerConnected == 1 );

            {
                assert( hub::native::viewer_nStream( viewer ) == 0 );
                assert( hub::native::viewer_nStreaming( viewer ) == 0 );
                CONSTRUCT_BEGIN( "OutputStream" );
                // hub::output::OutputStream outputStream( FILE_NAME, port, "127.0.0.1", header_ref
                // ); auto * outputStream =  hub::native::createOutputStream(&header_ref,
                // FILE_NAME.c_str(), port, "127.0.0.1");
                CONSTRUCT_END( "OutputStream" );
                assert( hub::native::viewer_nStream( viewer ) == 1 );
                assert( hub::native::viewer_nStreaming( viewer ) == 1 );

                for ( int i = 0; i < 10; ++i ) {
                    // outputStream.write( i );
                    // hub::native::outputStream_write_int(outputStream, i);
                }

                iTry = 0;
                while ( nNewData < 10 && iTry < 10 ) {
                    std::cout << "[test] waiting for 10 acqs received : " << nNewData << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                assert( iTry != 10 );
                assert( nNewData == 10 );

                // hub::native::freeOutputStream(outputStream);

                DESTRUCT_BEGIN( "OutputStream" );
            } // end outputStream
            DESTRUCT_END( "OutputStream" );

            iTry = 0;
            // while ( hub::native::viewer_nStreaming(viewer) != 0 && iTry < 10 ) {
            while ( hub::native::viewer_nStream( viewer ) != 0 && iTry < 10 ) {
                // while ( nDelStreamer == 0 && iTry < 10 ) {
                std::cout << "[test] waiting for outputStream disconnected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 10 );
            assert( hub::native::viewer_nStream( viewer ) == 0 );
            assert( hub::native::viewer_nStreaming( viewer ) == 0 );
            assert( nDelStreamer == 1 );
            assert( nServerDisconnected == 0 );

            DESTRUCT_BEGIN( "Server" );
        } // end server
        DESTRUCT_END( "Server" );

        iTry = 0;
        while ( nServerDisconnected == 0 && iTry < 20 ) {
            std::cout << "[test] waiting for viewer disconnected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTry;
        }
        assert( iTry != 20 );
        assert( !hub::native::viewer_isConnected( viewer ) );
        std::cout << "[test] nServerDisconnected : " << nServerDisconnected << std::endl;
        assert( nServerDisconnected == 1 );

        hub::native::freeViewer( viewer );

        DESTRUCT_BEGIN( "NativeViewer" );
    } // end viewer
    DESTRUCT_END( "NativeViewer" );

    TEST_END()
}

// TEST_CASE( "Native test" ) {
//    constexpr int delay = 1000; // ms

// //    const std::string ipv4 = "127.0.0.1";
// const int port = GET_RANDOM_PORT;
// //    const int port         = 1883;
// hub::Server server( port );
// server.asyncRun();

// // const hub::sensor::Resolution ref_resolution( { 1 }, hub::sensor::Format::BGR8 );
// const auto ref_resolution        = hub::make_matrix<hub::sensor::format::BGR8>();
// const std::string ref_sensorName = "sensorName";
// hub::sensor::SensorSpec::MetaData ref_metaData;
// ref_metaData["a"] = 0;
// ref_metaData["b"] = "string";
// ref_metaData["c"] = 2.0;
// // const hub::data::Mat4 ref_mat4( 0.0 );
// const hub::sensor::format::MAT4 ref_mat4( 0.0 );

// std::cout << "ref_mat4: " << ref_mat4 << std::endl;
// ref_metaData["d"] = ref_mat4;
// const hub::sensor::SensorSpec ref_sensorSpec(
//     ref_sensorName, { ref_resolution }, ref_metaData );
// // const std::string ref_streamName = FILE_NAME;
// // unsigned char ref_data[3]        = { 0, 1, 2 };
// // const hub::sensor::Acquisition ref_acq =
// // std::move( hub::sensor::Acquisition( 0, 1 ) << hub::Measure( ref_data, 3, ref_resolution ) );
// auto ref_acq    = hub::sensor::make_acquisition( ref_resolution );
// ref_acq.start() = 0;
// ref_acq.end()   = 1;
// auto& bgr8      = ref_acq.get<hub::sensor::format::BGR8&>();
// bgr8.b          = 0;
// bgr8.g          = 1;
// bgr8.r          = 2;

// auto onServerNotFound = []( const char* ipv4, int port ) {
//     std::cout << "[test][NativeViewer] onServerNotFound " << ipv4 << " " << port << std::endl;
// };
// auto onServerConnected = []( const char* ipv4, int port ) {
//     std::cout << "[test][NativeViewer] onServerConnected " << ipv4 << " " << port << std::endl;
// };
// auto onServerDisconnected = []( const char* ipv4, int port ) {
//     std::cout << "[test][NativeViewer] onServerDisconnected " << ipv4 << " " << port <<
//     std::endl;
// };
// static int nStreamerInited = 0;
// auto onNewStreamer = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
//     //        return false;
//     // const hub::sensor::Resolution ref_resolution( { 1 }, hub::sensor::Format::BGR8 );
//     const auto ref_resolution        = hub::make_matrix<hub::sensor::format::BGR8>();
//     const std::string ref_sensorName = "sensorName";
//     hub::sensor::SensorSpec::MetaData ref_metaData;
//     ref_metaData["a"] = 0;
//     ref_metaData["b"] = "string";
//     ref_metaData["c"] = 2.0;
//     // const hub::data::Mat4 ref_mat4( 0.0 );
//     const hub::sensor::format::MAT4 ref_mat4( 0.0 );
//     ref_metaData["d"] = ref_mat4;

//     std::cout << "[test][NativeViewer] onNewStreamer : " << streamName << std::endl;
//     char sensorName[80] = { 0 };
//     int strLen          = 0;
//     hub::native::sensorSpec_getSensorName( sensorSpec, sensorName, &strLen );
//     CHECK( ref_sensorName == sensorName );

//     // std::cout << "[test][NativeViewer] sensorName : '" << sensorName << "' size = " << strLen
//     //           << std::endl;
//     // const int resolutionsSize = hub::native::sensorSpec_getResolutionsSize( sensorSpec );
//     // std::cout << "[test][NativeViewer] resolutions size : " << resolutionsSize << std::endl;

//     // const auto& resolutions = sensorSpec->getResolutions();
//     // CHECK( resolutions.size() == resolutionsSize );

//     // for ( int iResolution = 0; iResolution < resolutionsSize; ++iResolution ) {
//     //     const auto& ref_resolution2 = resolutions.at( iResolution );
//     //     const auto& ref_format      = ref_resolution2.second;
//     //     const auto& ref_dims        = ref_resolution2.first;

//     //     const auto& format = hub::native::sensorSpec_getFormat( sensorSpec, iResolution );
//     //     std::cout << "[test][NativeViewer] resolutions[" << iResolution << "] format : " <<
//     //     format
//     //               << std::endl;
//     //     CHECK( format == static_cast<int>( ref_format ) );

//     //     const int resolutionSize =
//     //         hub::native::sensorSpec_getResolutionSize( sensorSpec, iResolution );
//     //     CHECK( resolutionSize ==
//     //            hub::sensor::resolution::computeAcquisitionSize( ref_resolution ) );

//     //     const int dimensionsSize =
//     //         hub::native::sensorSpec_getDimensionsSize( sensorSpec, iResolution );
//     //     std::cout << "[test][NativeViewer] resolutions[" << iResolution
//     //               << "] dimensions size : " << dimensionsSize << std::endl;
//     //     CHECK( hub::sensor::resolution::computeAcquisitionSize( ref_resolution ) ==
//     //            hub::sensor::resolution::computeAcquisitionSize( ref_format, ref_dims ) );
//     //     CHECK( dimensionsSize == ref_dims.size() );

//     //     for ( int iDimension = 0; iDimension < dimensionsSize; ++iDimension ) {
//     //         const auto dim =
//     //             hub::native::sensorSpec_getDimension( sensorSpec, iResolution, iDimension );

//     //         std::cout << "[test][NativeViewer] resolutions[" << iResolution << "] dimensions["
//     //                   << iDimension << "] size : "
//     //                   << hub::native::sensorSpec_getDimension(
//     //                          sensorSpec, iResolution, iDimension )
//     //                   << std::endl;

//     //         CHECK( dim == ref_dims.at( iDimension ) );
//     //     }
//     // }
//     // std::cout << "[test][NativeViewer] acquisitionsSize : "
//     //           << hub::native::sensorSpec_getAcquisitionSize( sensorSpec ) << std::endl;

//     // char resolutionsStr[80] = { 0 };
//     // hub::native::sensorSpec_getResolutionsStr( sensorSpec, resolutionsStr );
//     // std::cout << "[test][NativeViewer] resolutionsStr : '" << resolutionsStr << "'" <<
//     // std::endl;

//     char metaDataStr[256] = { 0 };
//     hub::native::sensorSpec_getMetaDataStr( sensorSpec, metaDataStr );
//     std::cout << "[test][NativeViewer] metaDataStr : '" << metaDataStr << "'" << std::endl;

//     const hub::sensor::SensorSpec* sensorSpecCopy = hub::native::sensorSpec_copy( sensorSpec );
//     CHECK( *sensorSpecCopy == *sensorSpec );

//     const hub::sensor::SensorSpec::MetaData* metaData =
//         hub::native::sensorSpec_getMetaData( sensorSpec );
//     CHECK( hub::native::metaData_exists( metaData, "a" ) );
//     CHECK( hub::native::metaData_getInt( metaData, "a" ) == ref_metaData.at( "a" ).get<int>() );
//     CHECK( hub::native::metaData_getDouble( metaData, "c" ) ==
//            ref_metaData.at( "c" ).get<double>() );
//     CHECK( !hub::native::metaData_exists( metaData, "z" ) );
//     float* data_mat4 = new float[16];
//     hub::native::metaData_getMat4( metaData, "d", data_mat4 );
//     // hub::data::Mat4 mat4( data_mat4 );
//     hub::sensor::format::MAT4 mat4( data_mat4 );
//     CHECK( mat4 == ref_mat4 );

//     CHECK( !hub::native::metaData_getMat4( metaData, "fake", nullptr ) );

//     char metaDataString[80] = { 0 };
//     int len;
//     hub::native::metaData_getString( metaData, "b", metaDataString, &len );
//     const char* ref_str = ref_metaData.at( "b" ).get<const char*>();
//     std::cout << "ref_str = " << ref_str << std::endl;
//     CHECK( len == strlen( ref_str ) );
//     CHECK( !strcmp( ref_str, metaDataString ) );

//     ++nStreamerInited;
//     return true;
// };
// auto onDelStreamer = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
//     std::cout << "[test][NativeViewer] onDelStreamer " << streamName << std::endl;
// };
// static int nReceiveAcq = 0;
// auto onNewAcquisition  = []( const char* streamName, const hub::sensor::Acquisition* acq ) {
//     CHECK( !strcmp( streamName, FILE_NAME.c_str() ) );
//     std::cout << "[test][NativeViewer] onNewAcquisition " << streamName << " " << *acq
//               << std::endl;
//     ++nReceiveAcq;
// };
// auto onSetProperty =
//     []( const char* streamName, const char* objectName, int property, const hub::Any* value ) {
//         std::cout << "[test][NativeViewer] onSetProperty " << streamName << " " << objectName
//                   << " " << property << " " << value << std::endl;
//     };
// auto onLogMessage = []( const char* logMessage ) {
//     std::cout << "[test][NativeViewer] onLogMessage " << logMessage << " " << *logMessage
//               << std::endl;
// };

// auto* viewerHandler = hub::native::createViewerHandler( onServerNotFound,
//                                                         onServerConnected,
//                                                         onServerDisconnected,
//                                                         onNewStream,
//                                                         onNewData,
//                                                         onDelStream,
//                                                         onSetProperty,
//                                                         onLogMessage );

// //    hub::native::viewer_setIpv4( viewer, ipv4.c_str() );
// //    hub::native::viewer_setPort( viewer, port );

// auto* viewer =
//     hub::native::createViewer( FILE_NAME.c_str(), viewerHandler, "127.0.0.1", port);
//                                //                                              ipv4.c_str(),
//                                //        port
//     // );

// // todo test
// //    if ( false )
// {
//     std::cout << "[Test] ############################### server start" << std::endl;
//     //        hub::Server server( port );
//     //        server.setMaxClients( 2 );
//     //        server.asyncRun();
//     int iTryConnect = 0;
//     while ( !viewer->isConnected() && iTryConnect < 10 ) {
//         std::cout << "[test] waiting for viewer connected" << std::endl;
//         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//         ++iTryConnect;
//     }
//     CHECK( viewer->isConnected() );
//     assert( viewer->isConnected() );
//     CHECK( hub::native::viewer_isConnected( viewer ) );
//     {
//         std::cout << "[Test] ############################### outputSensor start" << std::endl;

//         hub::sensor::OutputSensor* outputSensor = new hub::sensor::OutputSensor(
//             //                ref_sensorSpec, ref_streamName, hub::net::ClientSocket( ipv4, port
//             //                ) ); ref_sensorSpec, ref_streamName, ipv4, port );
//             ref_sensorSpec,
//             hub::output::OutputStream( FILE_NAME, port ) );

//         while ( nStreamerInited == 0 ) {
//             std::cout << "[Test] waiting for streamer inited" << std::endl;
//             std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//         }

//         ////////////////////////////////////////////////////

//         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//         *outputSensor << ref_acq;

//         while ( nReceiveAcq == 0 ) {
//             std::cout << "[Test] waiting for acq received" << std::endl;
//             std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//             *outputSensor << ref_acq;
//         }

//         //            std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

//         //            if ( false )
//         //            {

//         std::cout << "[Test] ############################### inputSensor start" << std::endl;

//         //                auto* inputSensor =
//         //                    hub::native::createInputSensor( ref_streamName.c_str());

//         //                auto acquisitionSize = hub::native::getAcquisitionSize( inputSensor );
//         //                CHECK( ref_acq.getSize() == acquisitionSize );

//         //                auto* acq = hub::native::getAcquisition( inputSensor );
//         //                CHECK( ref_acq == *acq );

//         //                unsigned char data[256];
//         //                hub::native::acquisition_getMeasure( acq, data, 0 );
//         //                CHECK( !memcmp( ref_data, data, 3 ) );

//         //                auto start = hub::native::acquisition_getStart( acq );
//         //                CHECK( start == ref_acq.getStart() );

//         //                hub::native::freeAcquisition( acq );

//         //                char buff[256];
//         //                int len;
//         //                hub::native::acquisition_to_string( acq, buff, &len );

//         //                hub::native::freeInputSensor( inputSensor );

//         //                const auto* inputSensor2 =
//         //                    hub::native::createInputSensor( "nonConnected" );
//         //                CHECK( inputSensor2 == nullptr );

//         //                auto& output = outputSensor->getOutput();
//         //                output.close();
//         //                delete outputSensor;

//         // todo native

//         std::cout << "[Test] ############################### inputSensor end" << std::endl;
//         //            }

//         delete outputSensor;

//         std::cout << "[Test] ############################### outputSensor end" << std::endl;
//     }
//     //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
//     std::cout << "[Test] ############################### server end" << std::endl;
// }

// hub::native::freeViewer( viewer );
// }
