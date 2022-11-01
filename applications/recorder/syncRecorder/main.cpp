
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
    //    std::string posStreamName = "Polhemus Patriot (sensor 2)";

    std::string imageStreamName = "ProceduralStreamer";
    //        std::string imageStreamName = "ULA-OP 256";

    std::vector<hub::Acquisition> acqs;

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    hub::SensorSpec sensorSpec;

    {

        hub::InputSensor inputSensor(
            hub::io::InputStream( posStreamName,
                                  imageStreamName,
                                  hub::net::ClientSocket( hub::net::s_defaultServiceIp, port ) ) );

        //        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {

            //            hub::InputSensor inputSensor2(hub::io::InputStream(
            //                imageStreamName,
            //                "",
            //                hub::net::ClientSocket(hub::net::s_defaultServiceIp, port)));

            std::cout << inputSensor.m_spec << std::endl;
            sensorSpec = inputSensor.m_spec;
            //            std::cout << inputSensor2.m_spec << std::endl;
            //            sensorSpecSum = inputSensor.m_spec + inputSensor2.m_spec;
            //            std::cout << sensorSpecSum << std::endl;

            long long lastAcqStart = -1;
            //            long long lastAcqStart2 = -1;
            auto thread = std::thread( [&]() {
                while ( !stopThread ) {
                    auto acq = std::make_unique<hub::Acquisition>( inputSensor.getAcquisition() );
                    //                    auto acq = inputSensor.getAcquisition();
                    while ( acq->m_start == lastAcqStart ) {
                        acq = std::make_unique<hub::Acquisition>( inputSensor.getAcquisition() );
                        //                        acq = inputSensor.getAcquisition();
                        std::cout << "remove ping for inputSensor" << std::endl;
                    }
                    lastAcqStart = acq->m_start;
                    assert( acq->getMeasures().size() == 2 );
                    //                    const auto& measure = acq->getMeasures().front();

                    //                    auto acq2 =
                    //                    std::make_unique<hub::Acquisition>(inputSensor2.getAcquisition());
                    //                    while (acq2->m_start == lastAcqStart2) {
                    //                        acq2 =
                    //                        std::make_unique<hub::Acquisition>(inputSensor2.getAcquisition());
                    //                        std::cout << "remove ping for inputSensor2" <<
                    //                        std::endl;
                    //                    }
                    //                    lastAcqStart2 = acq2->m_start;
                    //                    assert(acq2->getMeasures().size() == 1);
                    //                    const auto& measure2 = acq2->getMeasures().front();

                    //                    auto acq3 = std::move(hub::Acquisition { acq->m_start,
                    //                    acq->m_end }
                    //                        << hub::Measure { measure.m_data, measure.m_size }
                    //                        << hub::Measure { measure2.m_data, measure2.m_size });

                    acqs.push_back( std::move( *acq ) );

                    std::cout << "+" << std::flush;
                }
            } );
            std::cout << std::endl;

            //        std::this_thread::sleep_for( std::chrono::milliseconds( 6000 ) );
            auto ret = getchar();
            (void)ret;
            stopThread = true;

            assert( thread.joinable() );
            thread.join();

        } // end inputSensor2

        //    inputSensor.getInterface().close();
        std::cout << "inputSensor2 closed" << std::endl;
        //        getchar();
        //        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    } // end inputSensor

    //    inputSensor2.getInterface().close();
    std::cout << "inputSensor closed" << std::endl;
    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //        getchar();

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
        //        outputSensor.getInterface().close();
    }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // play record
    {
        std::string recordPath = PROJECT_DIR "data/";
        //        std::fstream recordFile( recordPath + "latest.txt", std::ios::in |
        //        std::ios::binary | std::ios::beg  );
        std::fstream recordFile( recordPath + "latest.txt", std::ios::in | std::ios::binary );
        assert( recordFile.is_open() );

        hub::InputSensor inputSensor( hub::io::File( std::move( recordFile ) ) );

        auto acqs2 = inputSensor.getAllAcquisitions();

        // play
        //        hub::SensorSpec sensorSpec = sensorSpec;
        //                auto& metaData             = sensorSpec.m_metaData;
        //                metaData["type"]           = "record";
        //                metaData["nAcqs"]          = (unsigned int)acqs.size();
        //        hub::OutputSensor outputSensor2(sensorSpec, hub::io::OutputStream("Player
        //        (record)"));
        hub::OutputSensor outputSensor2( inputSensor.m_spec,
                                         hub::io::OutputStream( "Player (record)" ) );

        for ( const auto& acq : acqs2 ) {
            outputSensor2 << acq;
        }

        //        std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
        while ( true ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        //    }
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
//     while (true) {
//         getchar();
//         gStop = true;
//         break;
//     }
// }

// int main(int argc, char* argv[])
//{
//     std::cout << "[main]" << std::endl;
//     //    std::thread keyBoardCommands(onEnter);

//    //    const std::string sensorNames[2] = { g_probePoseSensorName, g_probeScanSensorName };
//    //    const std::string sensorSyncNames[2] = { g_probeScanSensorName, "" };

//    //    std::filesystem::current_path(PROJECT_DIR);
//    //    std::filesystem::create_directories("data");
//    //    std::filesystem::current_path("data");

//    Recorder recorder(PROJECT_DIR "data/");

//    //    recorder.record({{g_probePoseSensorName + " (record)", g_probeScanSensorName + "
//    (record)"}, {g_probeScanSensorName + " (record)", ""}});

////    recorder.record({ { g_probePoseSensorName, ""}, { g_probePoseSensorName,
/// g_probeScanSensorName }, { g_probeScanSensorName, "" } });
//    recorder.record({ { "L500 Depth Sensor (Depth)", ""}, { "L500 Depth Sensor (Infrared)", "" },
//    { "L500 RGB Camera", "" } });

////    recorder.record({ { g_probePoseSensorName, ""}, { g_probeScanSensorName, "" } });

//    //    while (true) {
//    //        int a;
//    //        std::cin >> a;
//    auto ret = getchar();
//    (void)ret;
//    ////        gStop = true;
//    //        break;
//    //    }
//    //    std::this_thread::sleep_for(std::chrono::seconds(2));

//    recorder.stop();

//    //    if (keyBoardCommands.joinable())
//    //        keyBoardCommands.join();
//}
