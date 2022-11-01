
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>

#include <Configurations.hpp>
#include <thread>

int main( int argc, char* argv[] ) {

    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

    bool stopThread = false;

    std::string posStreamName = "Keyboard";

    std::string imageStreamName = "ProceduralStreamer";

    std::vector<hub::Acquisition> acqs;

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    hub::SensorSpec sensorSpec;

    {

        hub::InputSensor inputSensor(
            hub::io::InputStream( posStreamName,
                                  imageStreamName,
                                  hub::net::ClientSocket( hub::net::s_defaultServiceIp, port ) ) );

        {


            std::cout << inputSensor.m_spec << std::endl;
            sensorSpec = inputSensor.m_spec;

            long long lastAcqStart = -1;
            auto thread = std::thread( [&]() {
                while ( !stopThread ) {
                    auto acq = std::make_unique<hub::Acquisition>( inputSensor.getAcquisition() );
                    while ( acq->m_start == lastAcqStart ) {
                        acq = std::make_unique<hub::Acquisition>( inputSensor.getAcquisition() );
                        std::cout << "remove ping for inputSensor" << std::endl;
                    }
                    lastAcqStart = acq->m_start;
                    assert( acq->getMeasures().size() == 2 );



                    acqs.push_back( std::move( *acq ) );

                    std::cout << "+" << std::flush;
                }
            } );
            std::cout << std::endl;

            auto ret = getchar();
            (void)ret;
            stopThread = true;

            assert( thread.joinable() );
            thread.join();

        } // end inputSensor2

        std::cout << "inputSensor2 closed" << std::endl;

    } // end inputSensor

    std::cout << "inputSensor closed" << std::endl;

    // record file
    {
        std::string recordPath = PROJECT_DIR "data/";
        std::fstream recordFile( recordPath + "latest.txt",
                                 std::ios::out | std::ios::binary | std::ios::trunc );
        assert( recordFile.is_open() );

        auto& metaData    = sensorSpec.m_metaData;
        metaData["type"]  = "record";
        metaData["nAcqs"] = (unsigned int)acqs.size();

        hub::OutputSensor outputSensor( sensorSpec, hub::io::File( std::move( recordFile ) ) );
        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
    }


    // play record
    {
        std::string recordPath = PROJECT_DIR "data/";
        std::fstream recordFile( recordPath + "latest.txt", std::ios::in | std::ios::binary );
        assert( recordFile.is_open() );

        hub::InputSensor inputSensor( hub::io::File( std::move( recordFile ) ) );

        auto acqs2 = inputSensor.getAllAcquisitions();

        // play
        hub::OutputSensor outputSensor2( inputSensor.m_spec,
                                         hub::io::OutputStream( "Player (record)" ) );

        for ( const auto& acq : acqs2 ) {
            outputSensor2 << acq;
        }

        while ( true ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
    }
}

//#include <chrono>
//#include <filesystem>
//#include <fstream>
//#include <cassert>
//#include <thread>

//#include <Recorder.h>
//#include <constants.h>

//#include "IO/File.hpp"
//#include "Socket.hpp"
//#include "Sensor.hpp"

// static std::vector<std::fstream> gFiles;
// static bool gStop = false;

// static void signalHandler(int signum)
//{

//}

// void onEnter()
//{
// }

// int main(int argc, char* argv[])
//{





/// g_probeScanSensorName }, { g_probeScanSensorName, "" } });




//}
