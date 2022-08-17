
#include <chrono>
#include <filesystem>
#include <fstream>
#include <stream.h>
//#include <format>
#include <FileIO.h>
#include <cassert>
#include <socket.h>
#include <thread>

#include <Recorder.h>
#include <constants.h>

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

int main(int argc, char* argv[])
{
    std::cout << "[main]" << std::endl;
    //    std::thread keyBoardCommands(onEnter);

    //    const std::string sensorNames[2] = { g_probePoseSensorName, g_probeScanSensorName };
    //    const std::string sensorSyncNames[2] = { g_probeScanSensorName, "" };

    //    std::filesystem::current_path(PROJECT_DIR);
    //    std::filesystem::create_directories("data");
    //    std::filesystem::current_path("data");

    Recorder recorder(PROJECT_DIR "data/");

    //    recorder.record({{g_probePoseSensorName + " (record)", g_probeScanSensorName + " (record)"}, {g_probeScanSensorName + " (record)", ""}});

//    recorder.record({ { g_probePoseSensorName, ""}, { g_probePoseSensorName, g_probeScanSensorName }, { g_probeScanSensorName, "" } });
    recorder.record({ { "L500 Depth Sensor (Depth)", ""}, { "L500 Depth Sensor (Infrared)", "" }, { "L500 RGB Camera", "" } });

//    recorder.record({ { g_probePoseSensorName, ""}, { g_probeScanSensorName, "" } });

    //    while (true) {
    //        int a;
    //        std::cin >> a;
    auto ret = getchar();
    (void)ret;
    ////        gStop = true;
    //        break;
    //    }
    //    std::this_thread::sleep_for(std::chrono::seconds(2));

    recorder.stop();

    //    if (keyBoardCommands.joinable())
    //        keyBoardCommands.join();
}
