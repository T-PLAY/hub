
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>

#include <Configurations.hpp>
#include <filesystem>
#include <thread>

//#include <set>

int main( int argc, char* argv[] ) {

    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

    bool stopThread = false;

    //    std::string posStreamName = "Polhemus Patriot (sensor 2)";

    const std::string rootPath = PROJECT_DIR "data/";

    // creating new record folder
    char folderName[64] = { 0 };
    time_t rawtime;
    time( &rawtime );
    const auto timeinfo = localtime( &rawtime );
    strftime( folderName, sizeof( folderName ), "%Y-%m-%d_%H-%M-%S", timeinfo );
    const std::string newRecordFolder = rootPath + "records/" + folderName + "/";
    std::cout << "create directory " << newRecordFolder << std::endl;
    assert( !std::filesystem::exists( newRecordFolder ) );
    std::filesystem::create_directories( newRecordFolder );

    //    const std::string imageStreamName = "ProceduralStreamer";
    //    const std::string posStreamName = "Keyboard";

    //    std::vector<std::string> streamNames { posStreamName, imageStreamName };
    //    std::vector<std::string> streamNames { "Keyboard", "ProceduralStreamer" };
    //    std::vector<std::string> streamNames { "Polhemus Patriot (sensor 2)" };
    //    std::vector<std::string> streamNames { "Polhemus Patriot (sensor 1)", "Polhemus Patriot
    //    (sensor 2)" };
    std::vector<std::pair<std::string, std::string>> streamNames {
//        { "Polhemus Patriot (sensor 2)", "" }, { "ULA-OP 256", "" }
        { "Polhemus Patriot (sensor 2)", "ULA-OP 256" }
    };
    std::vector<std::thread> threads;
    //    threads.resize(streamNames.size());

    //        std::string imageStreamName = "ULA-OP 256";

//    std::set<long long> starts;

    for ( const auto& streamName : streamNames ) {

        threads.push_back( std::thread( [=, &stopThread]() {
            hub::InputSensor inputSensor( hub::io::InputStream(
                streamName.first, streamName.second, hub::net::ClientSocket( hub::net::s_defaultServiceIp, port ) ) );

            std::fstream recordFile( newRecordFolder + streamName.first + ".txt",
                                     std::ios::out | std::ios::binary | std::ios::trunc );
            assert( recordFile.is_open() );

            hub::OutputSensor outputSensor( inputSensor.m_spec,
                                            hub::io::File( std::move( recordFile ) ) );

            int nAcq = 0;
            long long previousStart = -1;
            while ( !stopThread ) {
                auto acq = inputSensor.getAcquisition();
                //                std::cout << "[" << streamName << "] record acq : " << acq <<
                //                std::endl;
                // ping acq
                if (acq.m_start == previousStart)
                    continue;

                assert(acq.m_start > previousStart);

                outputSensor << acq;
                std::cout << "+" << std::flush;
                ++nAcq;
//                starts.emplace(acq.m_start);
//                starts.insert(acq.m_start);
                previousStart = acq.m_start;
            }
            std::cout << std::endl;
            std::cout << "[" << streamName.first << "] " << nAcq << " acq recorded" << std::endl;
        } ) );
    }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    getchar();
    stopThread = true;

    for ( auto& thread : threads ) {
        assert( thread.joinable() );
        thread.join();
    }

    // copy this record into latest folder
    {
        std::string latestFolder = rootPath + "records/latest/";
        std::filesystem::create_directories( latestFolder );
        for ( const auto& dirEntry : std::filesystem::directory_iterator( latestFolder ) ) {
            std::filesystem::remove( dirEntry );
        }
        std::filesystem::copy(
            newRecordFolder, latestFolder, std::filesystem::copy_options::recursive );
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
