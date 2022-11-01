
#include <cassert>
#include <chrono>

#include <tuple>
//#include <cmath>
#include <utility>

#include <Configurations.hpp>

//#include <IO/Stream.hpp>
//#include <OutputSensor.hpp>

#include <Streamer.hpp>

#define ULA_STREAMER

int main( int argc, char* argv[] ) {
    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

#ifdef ULA_STREAMER
    constexpr int width  = 192;
    constexpr int height = 512;

    hub::SensorSpec::MetaData metaData;
    metaData["parent"]             = "Keyboard";
    metaData["scanWidth"]          = 50.0;
    metaData["scanDepth"]          = 35.0;
    const float localTransform[16] = { 17.5,
                                       0.000000,
                                       0.000000,
                                       0.000000,
                                       0.000000,
                                       -0.000004,
                                       100.000000,
                                       0.000000,
                                       0.000000,
                                       -25.000000,
                                       -0.00000,
                                       0.000000,
                                       30 + 17.5,
                                       0.000000,
                                       140.000000,
                                       1.000000 };
    metaData["transform"]          = localTransform;

    hub::Streamer streamer( hub::net::s_defaultServiceIp, port );
    streamer.addStream( "ProceduralStreamer",
                        { "ProceduralStreamer",
                          { { { width, height }, hub::SensorSpec::Format::Y8 } },
                          metaData } );

    const size_t imgSize = width * height;
    assert( imgSize == width * height );

    auto* data = new unsigned char[imgSize];

    int dec = 0;
    while ( true ) {

        const auto start = std::chrono::high_resolution_clock::now();
        // generate new image

        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                data[i * width + j] = ( i + j + dec ) % 256;
            }
        }

        //		    data[i] = (i / height + dec) % 256;
#    if defined( WIN32 )
        const auto maxFps = 60.0;
#    else
        const auto maxFps = 40.0;
#    endif
        const auto end = start + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );

        const auto& timestampStart =
            std::chrono::duration_cast<std::chrono::microseconds>( start.time_since_epoch() )
                .count();
        const auto& timestampEnd =
            std::chrono::duration_cast<std::chrono::microseconds>( end.time_since_epoch() ).count();
        ++dec;

        streamer.newAcquisition( "ProceduralStreamer",
                                 std::move( hub::Acquisition { timestampStart, timestampEnd }
                                            << hub::Measure { data, imgSize } ) );

        std::this_thread::sleep_until( end );
    }
#else

    const std::string sensorNames[2]          = { "ULA-OP 256", "Polhemus Patriot (probe)" };
    constexpr Stream::Format sensorFormats[2] = { Stream::Format::Y8, Stream::Format::DOF6 };
    const std::vector<int> sensorDims[2]      = { { 512, 192 }, { 1 } };
    constexpr int maxFpss[2]                  = { 40, 60 };
    constexpr size_t sensorDataSize[2]        = { 192 * 512, 28 };
    std::vector<std::thread> threads;

    int nSensor = ( argc == 2 ) ? ( 1 ) : ( 2 );
    for ( int i = 0; i < nSensor; ++i ) {
        const int iSensor = ( nSensor == 1 ) ? ( atoi( argv[1] ) ) : ( i );

        std::thread thread( [=]() {
            unsigned char* data = new unsigned char[sensorDataSize[iSensor]];

            while ( true ) {
                try {

                    hub::OutputSensor proceduralStream = hub::OutputSensor(
                        sensorNames[iSensor], sensorFormats[iSensor], sensorDims[iSensor] );

                    const size_t imgSize = proceduralStream.getAcquisitionSize();
                    assert( imgSize == 28 || imgSize == 98304 );

                    const auto maxFps = maxFpss[iSensor];

                    int dec = 0;
                    while ( true ) {

                        const auto start = std::chrono::high_resolution_clock::now();
                        // generate new image
                        if ( iSensor == 0 ) {
                            for ( size_t i = 0; i < imgSize; ++i ) {
                                data[i] = ( i / 192 + dec ) % 256;
                            }
                        }
                        else {
                            for ( size_t i = 0; i < imgSize / 4; ++i ) {
                                ( (float*)data )[i] = dec;
                            }
                        }
                        ++dec;
                        const auto end = start + std::chrono::nanoseconds( 1'000'000'000 / maxFps );

                        while ( std::chrono::high_resolution_clock::now() < end )
                            ;
                        //{

                        const auto now = std::chrono::high_resolution_clock::now();
                        const auto& timestampStart =
                            std::chrono::duration_cast<std::chrono::microseconds>(
                                start.time_since_epoch() )
                                .count();
                        const auto& timestampEnd =
                            std::chrono::duration_cast<std::chrono::microseconds>(
                                now.time_since_epoch() )
                                .count();
                        Stream::hub::Acquisition acq { timestampStart, timestampEnd, data };
                        acq.mSize = imgSize;
                        proceduralStream << acq;
                        // std::cout << "thread " << iSensor << " send acq " << acq << std::endl;

                    } // while (true)
                }
                catch ( std::exception& e ) {
                    std::cout << "[streamer] catch exception : " << e.what() << std::endl;
                }
            } // while (true)
            delete[] data;
        } );
        threads.emplace_back( std::move( thread ) );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    } // for (int i = 0; i < nSensor; ++i)

    for ( int i = 0; i < nSensor; ++i ) {
        if ( threads[i].joinable() ) threads[i].join();
    }
#endif

    return 0;
}
