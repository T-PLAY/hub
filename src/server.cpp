#include "server.h"

#include <cassert>
#include <memory>
#include <stream.h>

static std::string getServerHeader(int iThread)
{
    const std::string str = "\t\033[" + std::to_string(31 + iThread % 7) + "m[server:" + std::to_string(iThread) + "]\033[0m ";
    return str;
}

Server::Server(int port)
    : mServerSock(port)
{
}

void Server::run()
{
    int iThread = 1;

    while (true) {

        ClientSocket sock = mServerSock.waitNewClient();
        std::cout << getServerHeader(0) << "new client" << std::endl;

        std::thread thread([this, iThread, sock = std::move(sock)]() mutable {
            std::cout << getServerHeader(iThread) << "new thread\t\t\t\t server status : " << getStatus() << std::endl;

            Client::Type clientType;
            sock.read(clientType);

            switch (clientType) {
            case Client::Type::STREAMER: {

//                InputStream inputStream(std::move(sock));

                Streamer streamer {std::move(sock), {}};
                const std::string sensorName = streamer.mInputStream.getSensorName();
//                streamer.mInputStream = &inputStream;
                const auto & inputStream = streamer.mInputStream;

                //                mStreamers.push_back(&streamer);
                assert(mStreamers.find(sensorName) == mStreamers.end());
                mStreamers[sensorName] = &streamer;
                std::cout << getServerHeader(iThread) << "[streamer] new streamer\t\t server status : " << getStatus() << std::endl;

                std::cout << getServerHeader(iThread) << "[streamer] sensor name = '" << sensorName << "'" << std::endl;

                Stream::Acquisition acq;

                try {

                    // prevent all viewer there is a new streamer
                    std::cout << getServerHeader(iThread) << "[streamer] prevent viewers there is a new streamer" << std::endl;
                    for (const auto& viewer : mViewers) {
                        viewer->socket->write(Socket::Message::NEW_STREAMER);
                        viewer->socket->write(inputStream.getSensorName());
                    }

                    const size_t acquistionSize = inputStream.getAcquisitionSize();
                    assert(acquistionSize == 192 * 512);
                    acq.data = new unsigned char[acquistionSize];
                    std::cout << getServerHeader(iThread) << "[streamer] acquisitionSize:" << acquistionSize << std::endl;

                    // for each new stream acquistion
                    while (true) {

                        if (!streamer.mOutputStreams.empty()) {
                            const auto start = std::chrono::high_resolution_clock::now();

                            inputStream >> acq;

                            std::cout << getServerHeader(iThread) << "[streamer] receive data from streamer '" << inputStream.getSensorName() << "' and send it for " << streamer.mOutputStreams.size() << " stream viewers" << std::endl;

                            int dec = acq.data[0];
                            int width = inputStream.getDims().at(0);
                            for (size_t i = 0; i < acquistionSize; ++i) {
                                if (acq.data[i] != (i / width + dec) % 256) {
                                    int tmp = acq.data[i];
                                    unsigned char* ptr = &acq.data[i];
                                    assert(false);
                                }
                            }

                            // broadcast data
                            // stream new acquisition for all viewers of this stream
                            for (auto& outputStream : streamer.mOutputStreams) {
                                //                                assert(streamViewer->mOutputStream != nullptr);
                                outputStream << acq;
                            }
                            std::cout << getServerHeader(iThread) << "[streamer] data from streamer sent for all stream viewers" << std::endl;

                            const auto expectedFps = 20;
                            const auto end = start + std::chrono::microseconds(1'000'000 / expectedFps);
                            std::this_thread::sleep_until(end);

                        } else {
                            //                            sock.write(Socket::Message::PING);
                            inputStream.ping();
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        }
                    } // while (true)

                } catch (socket_error& e) {
                    std::cout << getServerHeader(iThread) << "[streamer] catch socket exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "[streamer] catch exception : " << e.what() << std::endl;
                    throw;
                }
                delete[] acq.data;
                //                mStreamers.remove(&streamer);
                mStreamers.erase(sensorName);

                for (auto& outputStream : streamer.mOutputStreams) {
                    //                    outputStream->socket->write(Stream::Message::CLOSE);
                    outputStream.close();
                }

                for (const auto* viewer : mViewers) {
                    viewer->socket->write(Socket::Message::DEL_STREAMER);
                    viewer->socket->write(inputStream.getSensorName());
                }

                std::cout << getServerHeader(iThread) << "[streamer] end" << std::endl;
            } break;

            case Client::Type::VIEWER: {

                //                ClientSocket sock(std::move(sock));
                // for each streamer, open stream viewer socket
                //                for (const auto* streamer : mStreamers) {
                for (const auto& pair : mStreamers) {
                    const auto& sensorName = pair.first;
                    //                    const auto & streamer = pair.second;

                    sock.write(Socket::Message::NEW_STREAMER);
                    sock.write(sensorName);
                }

                Viewer viewer { &sock };
                mViewers.push_back(&viewer);
                std::cout << getServerHeader(iThread) << "[viewer] new viewer\t\t\t server status : " << getStatus() << std::endl;

                try {
                    // check client still alive
                    while (true) {
                        sock.write(Socket::Message::PING);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }
                    //                    sock.waitClose();

                } catch (socket_error& e) {
                    std::cout << getServerHeader(iThread) << "[viewer] catch socket exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "[viewer] catch exception : " << e.what() << std::endl;
                    exit(3);
                    throw;
                }

                mViewers.remove(&viewer);
                std::cout << getServerHeader(iThread) << "[viewer] end" << std::endl;

                sock.clear();

            } break;

            case Client::Type::STREAM_VIEWER: {
                std::cout << getServerHeader(iThread) << "[stream viewer] new stream viewer\t server status : " << getStatus() << std::endl;

                std::string sensorName;
                sock.read(sensorName);

                assert(mStreamers.find(sensorName) != mStreamers.end());
                Streamer* streamer = mStreamers.at(sensorName);

                //                OutputStream outputStream(std::move(sock), *streamer->mInputStream);
                //                StreamViewer streamViewer = { &outputStream };
                //                streamer->mOutputStreams.push_back(&streamViewer);

                streamer->mOutputStreams.emplace_back(std::move(sock), streamer->mInputStream);
                //                streamer->mOutputStreams.empla(OutputStream { std::move(sock), *streamer->mInputStream });

                //                try {
                //                    // wait close connection from client
                //                    outputStream.waitClose();

                //                } catch (socket_error& e) {
                //                    std::cout << getServerHeader(iThread) << "[stream viewer] catch socket exception : " << e.what() << std::endl;
                //                } catch (std::exception& e) {
                //                    std::cout << getServerHeader(iThread) << "[stream viewer] catch exception : " << e.what() << std::endl;
                //                    throw;
                //                }

                //                streamer->mOutputStreams.remove(&streamViewer);

                //                std::cout << getServerHeader(iThread) << "case stream viewer : stream viewer end" << std::endl;

                //                if (sock.isConnected())
                //                    sock.write(Socket::Message::OK);

                std::cout << getServerHeader(iThread) << "[stream viewer] end" << std::endl;
            } break;

            default:
                std::cout << getServerHeader(iThread) << "unknown client type" << std::endl;
            }

            std::cout << getServerHeader(iThread) << "thread end\t\t\t\t server status : " << getStatus() << std::endl;
            std::cout << "---------------------------------------------------------------------------------------------" << std::endl;

            //            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            return;
            exit(6);
        });
        thread.detach();

        ++iThread;
    } // while (true)
}

std::string Server::getStatus() const
{
    std::string streamViewersStr = "[";
    for (const auto& pair : mStreamers) {
        const auto& sensorName = pair.first;
        const auto& streamer = pair.second;
        //        const int idSensor = mStreamers.find()
        std::string str = sensorName.substr(0, 3);
        //        streamViewersStr += "(" + sensorName + "," + std::to_string(streamer->streamViewers.size()) + ")";
        streamViewersStr += "(" + str + "," + std::to_string(streamer->mOutputStreams.size()) + ")";

        streamViewersStr += ",";
    }
    streamViewersStr += "]";

    std::string str = std::string("nbStreamer = ") + std::to_string(mStreamers.size()) + ", nbViewer = " + std::to_string(mViewers.size()) + " " + streamViewersStr;
    return str;
}
