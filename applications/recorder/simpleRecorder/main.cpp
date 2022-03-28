
#include <chrono>
#include <filesystem>
#include <fstream>
#include <stream.h>
//#include <format>

// static std::vector<std::fstream> gFiles;
static bool gStop = false;

// static void signalHandler(int signum)
//{
//     std::cout << "signalHandler() Interrupt signal (" << signum << ") received." << std::endl;

//    for (auto& file : gFiles) {
//        if (file.is_open())
//            file.close();
//    }
//    // cleanup and close up stuff here
//    // terminate program
//    exit(signum);
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
    //    signal(SIGINT, signalHandler);
    std::thread keyBoardCommands(onEnter);
    //    keyBoardCommands.detach();

    const std::string sensorNames[2] = { "Polhemus Patriot (probe)", "ULA-OP 256" };
    //    const std::string sensorSyncNames[2] = { "", "" };
    const std::string sensorSyncNames[2] = { "ULA-OP 256", "" };

    //    const std::string sensorNames[2] = { "Polhemus Patriot (probe)", "ULA-OP 256" };
    //    const std::string sensorSyncNames[2] = { "ULA-OP 256", "" };

    std::filesystem::current_path(PROJECT_DIR);
    std::filesystem::create_directories("records");
    std::filesystem::current_path("records");

    //    std::time_t ct = std::time(0);
    std::string fileFolder;

    std::vector<std::unique_ptr<InputStream>> inputStreams;
    std::vector<std::unique_ptr<OutputStream>> outputFileStreams;

    for (int i = 0; i < 2; ++i) {
        inputStreams.push_back(std::make_unique<InputStream>(sensorNames[i], sensorSyncNames[i]));
        //                inputStreams.push_back(std::move(InputStream(sensorNames[i], sensorSyncNames[i])));
    }

    //            InputStream inputStream(sensorNames[0], sensorSyncNames[0]);
    //            InputStream inputStream2(sensorNames[1], sensorSyncNames[1]);

    //            char* cc = ctime(&ct);
    //            const auto now = std::chrono::system_clock::now();
    //            std::string folderName = std::format("{:%d-%m-%Y %H:%M:%OS", now);
    //            std::cout << "create dir '" << cc << "'" << std::endl;
    //            std::filesystem::create_directories(cc);
    //            std::filesystem::current_path(cc);
//    std::array<char, 64> buffer;
    char buffer[64] = {0};
//    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeinfo);
    fileFolder = buffer;
    std::cout << "create directory " << fileFolder << std::endl;
    std::filesystem::create_directories(fileFolder);

    //            std::fstream file(fileFolder + "/" + sensorNames[0], std::ios::binary | std::ios::out);
    //            std::fstream file2(fileFolder + "/" + sensorNames[1], std::ios::binary | std::ios::out);
    //            std::vector<std::fstream> files;
    //            files.reserve(2);
    std::fstream files[2];
    //                        gFiles.clear();
    for (int i = 0; i < 2; ++i) {
        //                files.push_back(std::fstream(fileFolder + "/" + sensorNames[i], std::ios::binary | std::ios::out));
//        files[i] = std::fstream(fileFolder + "/" + sensorNames[i] + ".txt", std::ios::binary | std::ios::out);
        files[i] = std::fstream(fileFolder + "/" + sensorNames[i] + ".txt", std::ios::binary | std::ios::out);
        assert(files[i].is_open());
        outputFileStreams.push_back(std::make_unique<OutputStream>(sensorNames[i], inputStreams[i]->getFormat(), inputStreams[i]->getDims(), files[i]));
        //                            outputFileStreams.push_back(std::make_unique<OutputStream>(sensorNames[i], inputStreams[i]->getFormat(), inputStreams[i]->getDims(), gFiles.back()));
        //            outputFileStreams.push_back(std::make_unique<OutputStream>(sensorNames[0], inputStreams[0]->getFormat(), inputStreams[0]->getDims(), file));
        //            outputFileStreams.push_back(std::make_unique<OutputStream>(sensorNames[1], inputStreams[1]->getFormat(), inputStreams[1]->getDims(), file2));
        //                            outputFileStreams.push_back(std::make_unique<OutputStream>(sensorNames[i], inputStreams[i]->getFormat(), inputStreams[i]->getDims(), gFiles.back()));
    }
    //            OutputStream fileStream(sensorNames[0], inputStreams[0]->getFormat(), inputStreams[0]->getDims(), file);
    //            OutputStream fileStream2(sensorNames[1], inputStreams[1]->getFormat(), inputStreams[1]->getDims(), file2);
    try {

        while (!gStop) {
            //                for (int i = 0; i < 2; ++i) {
            //                    Stream::Acquisition acq;
            //                    *inputStreams[i] >> acq;
            //                    *outputFileStreams[i] << acq;
            //                    //                    *outputFileStreams[i] << *inputStreams[i];
            //                    std::cout << "[" << i << "] read/write acq : " << acq << std::endl;
            //                }
            Stream::Acquisition acq;
            *inputStreams[0] >> acq;
            std::cout << "[" << 0 << "] read/write acq : " << acq << std::endl;
            *outputFileStreams[0] << acq;

            *inputStreams[1] >> acq;
            std::cout << "[" << 1 << "] read/write acq : " << acq << std::endl;
            *outputFileStreams[1] << acq;
        }

        //        files[0].close();
        //        files[1].close();
        //            files.clear();
        //            file.close();
        //            file2.close();

    } catch (std::exception& e) {
        std::cout << "catch exception : " << e.what() << std::endl;
        gStop = true;
    }

    for (int i = 0; i < 2; ++i) {
        files[i].close();
    }

    //        for (auto& file : gFiles) {
    //            if (file.is_open()) {
    //                file.close();
    //                std::cout << "close file " << std::endl;
    //            }
    //        }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //    for (int i = 0; i < 2; ++i) {
    //        if (files[i].is_open())
    //            files[i].close();
    //    }

    //        std::filesystem::remove("latest");
    //    std::filesystem::remove_all("latest/*");
    std::filesystem::create_directories("latest");
    for (const auto& dirEntry : std::filesystem::directory_iterator("latest")) {
        std::filesystem::remove(dirEntry);
    }
    std::filesystem::copy(fileFolder, "latest", std::filesystem::copy_options::recursive);

    if (keyBoardCommands.joinable())
        keyBoardCommands.join();

    //    const int nSensor = (argc > 1) ? (atoi(argv[1])) : (-1);

    //    std::set<std::string> sensorNames;
    //    std::set<std::string> sensorSyncNames;
    //    for (int i = 0; i <nSensor; ++i) {
    //        sensorNames.insert(argv[2 + i * 2]);
    //        sensorSyncNames.insert(argv[2 + i * 2 + 1]);
    //    }

    //    std::filesystem::create_directories("records");
    //    std::filesystem::current_path("records");

    //    while (true) {
    //        try {
    //            ClientSocket sock("127.0.0.1", SERVICE_PORT);
    //            sock.write(ClientSocket::Type::VIEWER);

    //            while (true) {

    //                Socket::Message serverMessage;
    //                sock.read(serverMessage);

    //                switch (serverMessage) {

    //                case Socket::Message::PING: {
    //                    // server check client connection
    //                    // nothing to do
    //                } break;

    //                case Socket::Message::NEW_STREAMER: {
    //                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
    //                    std::string streamerSensorName;
    //                    sock.read(streamerSensorName);
    //                    std::string format;
    //                    sock.read(format);
    //                    std::string dims;
    //                    sock.read(dims);
    //                    std::string size;
    //                    sock.read(size);

    //                    std::cout << "[Thread_Client] [viewer] new streamer " << streamerSensorName << ", format:" << format << ", dims:" << dims << ", acquisitionSize:" << size << std::endl;
    //                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '" << streamerSensorName << "'" << std::endl;

    //                    std::thread thread([streamerSensorName]() {
    //                        InputStream inputStream(streamerSensorName);
    //                        std::fstream file(streamerSensorName, std::ios::binary | std::ios::out);
    //                        assert(file.is_open());

    //                        OutputStream fileStream(streamerSensorName, inputStream.getFormat(), inputStream.getDims(), file);
    //                        try {
    //                            while (true) {
    //                                Stream::Acquisition acq;
    //                                inputStream >> acq;
    //                                fileStream << acq;
    //                                std::cout << "receive and wrote acquisition : " << acq << std::endl;
    //                            }
    //                        } catch (std::exception& e) {
    //                            std::cout << "catch exception : " << e.what() << std::endl;
    //                        }

    //                        file.close();
    //                    });
    //                    thread.detach();

    //                } break;

    //                case Socket::Message::DEL_STREAMER: {
    //                    std::string streamerSensorName;
    //                    sock.read(streamerSensorName);
    //                    std::cout << "[Thread_Client] [viewer] del streamer '" << streamerSensorName << "'" << std::endl;
    //                } break;

    //                default:
    //                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
    //                }
    //            }

    //        } catch (std::exception& e) {
    //            std::cout << "[Thread_Client] [viewer] catch exception : " << e.what() << std::endl;
    //        }
    //    } // while (!this->isInterruptionRequested())
}
