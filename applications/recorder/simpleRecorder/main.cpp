
//#include <InputSensor.hpp>
//#include <OutputSensor.hpp>

//#include <IO/File.hpp>
//#include <IO/Stream.hpp>

//#include <Configurations.hpp>
//#include <filesystem>
//#include <thread>

#include <iostream>
#include <Recorder.hpp>


//#include <set>

int main( int argc, char* argv[] ) {

    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

//    bool stopThread = false;

//    const std::string rootPath = PROJECT_DIR "data/";
    std::vector<std::pair<std::string, std::string>> streamNames {
              { "ULA-OP 256", "Polhemus Patriot (sensor 2)"} };
//            { "Polhemus Patriot (sensor 2)", ""}, {"ULA-OP 256", ""} };
    //        { "Polhemus Patriot (sensor 2)", ""}};
    //        { "Polhemus Patriot (sensor 2)", ""}, {"Polhemus Patriot (sensor 1)", ""}};

    Recorder recorder(hub::net::s_defaultServiceIp, port);
//    recorder.setPort(port);
    recorder.record(streamNames);

    getchar();
    std::cout << "stop record " << std::endl;

    recorder.stop();

//    // creating new record folder
//    char folderName[64] = { 0 };
//    time_t rawtime;
//    time( &rawtime );
//    const auto timeinfo = localtime( &rawtime );
//    strftime( folderName, sizeof( folderName ), "%Y-%m-%d_%H-%M-%S", timeinfo );
//    const std::string newRecordFolder = rootPath + "records/" + folderName + "/";
//    std::cout << "create directory " << newRecordFolder << std::endl;
//    assert( !std::filesystem::exists( newRecordFolder ) );
//    std::filesystem::create_directories( newRecordFolder );

//    std::vector<std::pair<std::string, std::string>> streamNames {
//          { "Polhemus Patriot (sensor 2)", ""}, { "ULA-OP 256", "" } };
////        { "Polhemus Patriot (sensor 2)", ""}, {"ULA-OP 256", ""} };
////        { "Polhemus Patriot (sensor 2)", ""}};
////        { "Polhemus Patriot (sensor 2)", ""}, {"Polhemus Patriot (sensor 1)", ""}};
//    std::vector<std::thread> threads;

//    for ( const auto& streamName : streamNames ) {

//        std::this_thread::sleep_for(std::chrono::milliseconds(100));

//        threads.push_back( std::thread( [=, &stopThread]() {
//            hub::InputSensor inputSensor( hub::io::InputStream(
//                streamName.first,
//                streamName.second,
//                hub::net::ClientSocket( hub::net::s_defaultServiceIp, port ) ) );

//            std::fstream recordFile( newRecordFolder + streamName.first + ".txt",
//                                     std::ios::out | std::ios::binary | std::ios::trunc );
//            assert( recordFile.is_open() );

//            hub::OutputSensor outputSensor( inputSensor.m_spec,
//                                            hub::io::File( std::move( recordFile ) ) );

//            int nAcq                = 0;
//            long long previousStart = -1;
//            while ( !stopThread ) {
//                auto acq = inputSensor.getAcquisition();
//                // ping acq
//                if ( acq.m_start == previousStart ) continue;

//                assert( acq.m_start > previousStart );

//                outputSensor << acq;
//                std::cout << "+" << std::flush;
//                ++nAcq;
//                previousStart = acq.m_start;
//            }
//            std::cout << std::endl;
//            std::cout << "[" << streamName.first << "] " << nAcq << " acq recorded" << std::endl;
//        } ) );
//    }

//    getchar();
//    stopThread = true;

//    for ( auto& thread : threads ) {
//        assert( thread.joinable() );
//        thread.join();
//    }

    // copy this record into latest folder
//    {
//        std::string latestFolder = rootPath + "records/latest/";
//        std::filesystem::create_directories( latestFolder );
//        for ( const auto& dirEntry : std::filesystem::directory_iterator( latestFolder ) ) {
//            std::filesystem::remove( dirEntry );
//        }
//        std::filesystem::copy(
//            newRecordFolder, latestFolder, std::filesystem::copy_options::recursive );
//    }
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
