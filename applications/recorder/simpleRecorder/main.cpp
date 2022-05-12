
#include <chrono>
#include <filesystem>
#include <fstream>
#include <stream.h>
//#include <format>
#include <FileIO.h>
#include <cassert>
#include <socket.h>
#include <thread>

#include <constants.h>

// static std::vector<std::fstream> gFiles;
static bool gStop = false;

// static void signalHandler(int signum)
//{

//}

void onEnter()
{
    while (true) {
        getchar();
        gStop = true;
        break;
    }
}

int main(int argc, char* argv[])
{
    std::thread keyBoardCommands(onEnter);

    const std::string sensorNames[2] = { g_probePoseSensorName, g_probeScanSensorName };
    const std::string sensorSyncNames[2] = { g_probeScanSensorName, "" };

    std::filesystem::current_path(PROJECT_DIR);
    std::filesystem::create_directories("records");
    std::filesystem::current_path("records");

    std::string fileFolder;

    std::vector<std::unique_ptr<InputStream>> inputStreams;
    std::vector<std::unique_ptr<OutputStream>> outputFileStreams;

    for (int i = 0; i < 2; ++i) {
        inputStreams.push_back(std::make_unique<InputStream>(ClientSocket(sensorNames[i], sensorSyncNames[i])));
    }

    char buffer[64] = { 0 };
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeinfo);
    fileFolder = buffer;
    std::cout << "create directory " << fileFolder << std::endl;
    std::filesystem::create_directories(fileFolder);

//    std::fstream files[2];
    for (int i = 0; i < 2; ++i) {
        auto file = std::fstream(fileFolder + "/" + sensorNames[i] + ".txt", std::ios::binary | std::ios::out);
        assert(file.is_open());

        // here
        outputFileStreams.push_back(std::make_unique<OutputStream>(sensorNames[i], inputStreams[i]->getFormat(), inputStreams[i]->getDims(), FileIO(std::move(file)), inputStreams[i]->getMetaData()));
    }
    try {

        while (!gStop) {
            Stream::Acquisition acq;
            *inputStreams[0] >> acq;
            std::cout << "[" << 0 << "] read/write acq : " << acq << std::endl;
            *outputFileStreams[0] << acq;

            *inputStreams[1] >> acq;
            std::cout << "[" << 1 << "] read/write acq : " << acq << std::endl;
            *outputFileStreams[1] << acq;
        }

    } catch (std::exception& e) {
        std::cout << "catch exception : " << e.what() << std::endl;
        gStop = true;
    }

//    for (int i = 0; i < 2; ++i) {
//        files[i].close();
//    }

    std::filesystem::create_directories("latest");
    for (const auto& dirEntry : std::filesystem::directory_iterator("latest")) {
        std::filesystem::remove(dirEntry);
    }
    std::filesystem::copy(fileFolder, "latest", std::filesystem::copy_options::recursive);

    if (keyBoardCommands.joinable())
        keyBoardCommands.join();
}
