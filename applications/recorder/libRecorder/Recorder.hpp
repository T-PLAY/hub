
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>

#include <Configurations.hpp>
#include <filesystem>
#include <thread>

//#include <set>

class Recorder {
public:
    Recorder( const std::string& ipv4 = hub::net::s_defaultServiceIp,
            const int& port         = hub::net::s_defaultServicePort );
    void record(const std::vector<std::pair<std::string, std::string>> & streamNames);
    void stop();
//    void setPort(int port);

private:
    const std::string m_rootPath = PROJECT_DIR "data/";
    bool m_stopThread = false;

    const std::string& m_ipv4;
    const int& m_port;

    std::vector<std::thread> m_threads;
//    int m_port = hub::net::s_defaultServicePort;
    std::string m_recordFolder;
};

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
