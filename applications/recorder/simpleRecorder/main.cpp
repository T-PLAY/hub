
#include <filesystem>
#include <fstream>
#include <stream.h>

int main()
{

    //    std::filesystem::current_path(std::filesystem::temp_directory_path());
    std::filesystem::create_directories("records");
    std::filesystem::current_path("records");

    while (true) {
        try {
            ClientSocket sock("127.0.0.1", SERVICE_PORT);
            sock.write(ClientSocket::Type::VIEWER);

            while (true) {

                Socket::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {

                case Socket::Message::PING: {
                    // server check client connection
                    // nothing to do
                } break;

                case Socket::Message::NEW_STREAMER: {
                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
                    std::string streamerSensorName;
                    sock.read(streamerSensorName);
                    std::string format;
                    sock.read(format);
                    std::string dims;
                    sock.read(dims);
                    std::string size;
                    sock.read(size);

                    std::cout << "[Thread_Client] [viewer] new streamer " << streamerSensorName << ", format:" << format << ", dims:" << dims << ", acquisitionSize:" << size << std::endl;
                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '" << streamerSensorName << "'" << std::endl;

                    std::thread thread([streamerSensorName]() {
                        InputStream inputStream(streamerSensorName);
                        //                        std::ofstream file(streamerSensorName, std::ios::binary);
                        std::fstream file(streamerSensorName, std::ios::binary | std::ios::out);
                        assert(file.is_open());

                        OutputStream fileStream(streamerSensorName, inputStream.getFormat(), inputStream.getDims(), file);
                        try {
                            while (true) {
                                Stream::Acquisition acq;
                                inputStream >> acq;
                                fileStream << acq;
                                std::cout << "receive and wrote acquisition : " << acq << std::endl;
                            }
                        } catch (std::exception& e) {
                            std::cout << "catch exception : " << e.what() << std::endl;
                        }

                        file.close();
                    });
                    thread.detach();

                } break;

                case Socket::Message::DEL_STREAMER: {
                    std::string streamerSensorName;
                    sock.read(streamerSensorName);
                    std::cout << "[Thread_Client] [viewer] del streamer '" << streamerSensorName << "'" << std::endl;
                } break;

                default:
                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
                }
            }

        } catch (std::exception& e) {
            std::cout << "[Thread_Client] [viewer] catch exception : " << e.what() << std::endl;
        }
    } // while (!this->isInterruptionRequested())
}
