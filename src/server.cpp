#include "server.h"

//#include <net.h>
//#include <socket.h>
//#include <net.h>
#include <cassert>
#include <memory>
#include <stream.h>

static std::string getServerHeader(int iThread)
{
    //    const auto & threadId = std::this_thread::get_id();
    //    const size_t hash = std::hash<std::thread::id>{}(threadId);
    const std::string str = "\t\033[" + std::to_string(31 + iThread % 7) + "m[server:" + std::to_string(iThread) + "]\033[0m ";
    //    const std::string str = "\t\033[" + std::to_string(31 + iThread % 7) + "m[server]\033[0m ";
    return str;
}

// Server::Server(std::function<void (Socket & sock)> processClient, int port)
Server::Server(int port)
    : mServerSock()
//    , mProcessClient(processClient)
{
}

void Server::run()
{
    //    constexpr size_t imgSize = 192 * 512;
    //    unsigned char img[imgSize];
    //    SOCKET new_socket;
    int iThread = 1;

    while (true) {

        //        const ClientSocket && tmp = mServerSock.waitNewClient();
        //        std::shared_ptr<ClientSocket> sock = std::make_shared<ClientSocket>(mServerSock.waitNewClient());
        ClientSocket sock = mServerSock.waitNewClient();
        //        ClientSocket sock = std::move(tmp);
        //        ClientSocket sock2 = std::move(sock);

        //        std::thread thread([this, sock = std::move(sock)]() {
        std::cout << getServerHeader(0) << "new client" << std::endl;

        std::thread thread([this, iThread, sock = std::move(sock)]() {
            std::cout << getServerHeader(iThread) << "new thread\t\t\t\t server status : " << getStatus() << std::endl;
            Client::Type clientType;
            sock.read(clientType);

            switch (clientType) {
            case Client::Type::STREAMER: {
                //                Stream::InitPacket initPacket;
                Streamer streamer;
                sock.read(streamer.initPacket);
                streamer.socket = &sock;
                streamer.id = m_iStreamer++;

                mStreamers.push_back(&streamer);
                std::cout << getServerHeader(iThread) << "new streamer\t\t\t\t server status : " << getStatus() << std::endl;

                unsigned char* data = nullptr;

                try {
                    //                std::cout << "type:" << (int)streamer.mType << std::endl;
                    std::cout << getServerHeader(iThread) << "stream device:" << (int)streamer.initPacket.mDevice << std::endl;
                    std::cout << getServerHeader(iThread) << "stream format:" << (int)streamer.initPacket.mFormat << std::endl;
                    std::cout << getServerHeader(iThread) << "stream height:" << (int)streamer.initPacket.mHeight << std::endl;
                    std::cout << getServerHeader(iThread) << "stream sensor:" << (int)streamer.initPacket.mSensor << std::endl;
                    std::cout << getServerHeader(iThread) << "stream width:" << (int)streamer.initPacket.mWidth << std::endl;

                    // prevent all viewer there is a new streamer
                    std::cout << getServerHeader(iThread) << "prevent viewers this is a new streamer" << std::endl;
                    for (const auto& viewer : mViewers) {
                        viewer->socket->write(Server::Message::NEW_STREAMER);
                        viewer->socket->write(streamer.id);
                    }

                    const size_t acquistionSize = streamer.initPacket.mWidth
                        * streamer.initPacket.mHeight
                        * Stream::formatNbByte[static_cast<int>(streamer.initPacket.mFormat)];
                    //                    unsigned char data[acquistionSize];
                    assert(acquistionSize == 192 * 512);
                    data = new unsigned char[acquistionSize];
                    std::cout << getServerHeader(iThread) << "acquisitionSize:" << acquistionSize << std::endl;

                    // for each new stream acquistion
                    while (true) {

                        if (!streamer.streamViewers.empty()) {
                            sock.write(Message::SYNC);
                            std::cout << getServerHeader(iThread) << "send sync start new acquisition\t server status : " << getStatus() << std::endl;

                            Client::Message message;
                            sock.read(message);

                            assert(message == Client::Message::DATA);

                            //                            switch (message) {
                            //                            case Client::Message::PING:
                            //                                std::cout << getServerHeader(iThread) << "client is pinging" << std::endl;
                            //                                break;

                            //                            case Client::Message::DATA:
                            //                    char sync = 's';
                            //                    sock.write(sync);

                            Stream::TimestampInterval timestampInterval;
                            sock.read(timestampInterval);
                            //                    sock.read()
                            sock.read(data, acquistionSize);
                            std::cout << getServerHeader(iThread) << "receive data from streamer " << streamer.id << " and send it for " << streamer.streamViewers.size() << " stream viewers" << std::endl;

                            //                            constexpr int width = 192;
                            //                            constexpr int height = 512;
                            //                            int dec = data[0];
                            //                            for (int i = 0; i < width; ++i) {
                            //                                for (int j = 0; j < height; ++j) {
                            //                                    assert(i + j * width < acquistionSize);
                            //                                    assert(data[i + j * width] == (j + dec) % 256);
                            //                                }
                            //                            }
                            int dec = data[0];
                            for (size_t i = 0; i < acquistionSize; ++i) {
                                if (data[i] != (i + dec) % 256) {
                                    int tmp = data[i];
                                    unsigned char* ptr = &data[i];
                                    assert(false);
                                }
//                                assert(data[i] == 127);
                            }

                            // broadcast data
                            // stream new acquisition for all viewers of this stream
                            for (const auto& streamViewer : streamer.streamViewers) {
                                streamViewer->socket->write(Server::Message::DATA);
                                streamViewer->socket->write(timestampInterval);
                                streamViewer->socket->write(data, acquistionSize);
                            }
                            //                    sock.read(data);
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                            //                            std::cout << "wait input for new frame" << std::endl;
                            //                            std::string a;
                            //                            std::cin >> a;
                            //                                break;

                            //                            default:
                            //                                std::cout << getServerHeader(iThread) << "unknown client message" << std::endl;
                            //                                exit(5);
                            //                            }
                        } else {
                            sock.write(Server::Message::PING);
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        }
                    }

                } catch (socket_error& e) {
                    std::cout << getServerHeader(iThread) << "catch socket exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "catch exception : " << e.what() << std::endl;
                    throw;
                }
                delete[] data;
                mStreamers.remove(&streamer);

                for (const auto* streamViewer : streamer.streamViewers) {
                    streamViewer->socket->write(Server::Message::CLOSE);
                }

                for (const auto* viewer : mViewers) {
                    viewer->socket->write(Server::Message::DEL_STREAMER);
                    viewer->socket->write(streamer.id);
                    //                    streamViewer->socket->write(Server::Message::CLOSE);
                }

                //                std::cout << getServerHeader(iThread) << "remove streamer\t\t server status : " << getStatus() << std::endl;

                //                for (const auto & viewer : mViewers) {
                //                }
                std::cout << getServerHeader(iThread) << "end streamer" << std::endl;
            } break;

            case Client::Type::VIEWER: {
                //                sock.write(Server::Message::LIST_OF_STREAMERS);
                //                sock.write(mStreamers);
                //                for (int i = 0; i < mStreamers.size(); ++i) {

                // for each streamer, open stream viewer socket
                for (const auto* streamer : mStreamers) {
                    //                for (const auto & streamer : mStreamers) {
                    sock.write(Server::Message::NEW_STREAMER);
                    sock.write(streamer->id);
                    //                    sock.write(streamer);
                }
                //                mViewers.push_back(Viewer { std::move(sock) });
                Viewer viewer { &sock };
                mViewers.push_back(&viewer);
                std::cout << getServerHeader(iThread) << "new viewer\t\t\t\t server status : " << getStatus() << std::endl;

                try {
                    // check client still alive
                    while (true) {
                        sock.write(Server::Message::PING);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }

                } catch (socket_error& e) {
                    std::cout << getServerHeader(iThread) << "catch socket exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "catch exception : " << e.what() << std::endl;
                    exit(3);
                    throw;
                }

                mViewers.remove(&viewer);
                std::cout << getServerHeader(iThread) << "end viewer" << std::endl;

            } break;

            case Client::Type::STREAM_VIEWER: {
                std::cout << getServerHeader(iThread) << "new stream viewer" << std::endl;
                int iStreamer;
                sock.read(iStreamer);

                Streamer* streamerI = nullptr;
                for (Streamer* streamer : mStreamers) {
                    if (streamer->id == iStreamer) {
                        streamerI = streamer;
                        break;
                    }
                }
                assert(streamerI != nullptr);
                sock.write(streamerI->initPacket);

                StreamViewer streamViewer { &sock };
                streamerI->streamViewers.push_back(&streamViewer);
                //                ClientSocket sock2 = std::move(sock);
                //                mViewers.push_back(Viewer { sock });
                //                streamerI->streamViewers.push_back(Viewer { sock });
                try {
                    while (true) {
                        //                        sock.write(Server::Message::PING);
                        Client::Message clientMessage;
                        sock.read(clientMessage);
                        std::cout << getServerHeader(iThread) << "read message from stream viewer" << std::endl;

                        if (clientMessage == Client::Message::CLOSE) {
                            std::cout << getServerHeader(iThread) << "stream viewer want to close" << std::endl;
                            break;
                        }
                    }
                } catch (socket_error& e) {
                    std::cout << getServerHeader(iThread) << "catch socket exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "catch exception : " << e.what() << std::endl;
                    throw;
                }

                streamerI = nullptr;
                for (Streamer* streamer : mStreamers) {
                    if (streamer->id == iStreamer) {
                        streamerI = streamer;
                        break;
                    }
                }
                if (streamerI != nullptr) {
                    streamerI->streamViewers.remove(&streamViewer);
                }
                std::cout << getServerHeader(iThread) << "stream viewer end" << std::endl;

                if (sock.isConnected())
                    sock.write(Server::Message::OK);
                //                viewer.socket =std::move(sock);
                //                streamerI->viewers.push_back(Viewer(sock));
                //                Viewer viewer;
                //                viewer.socket = std::move(sock);
                //                streamerI->viewers.push_back({sock});
                std::cout << getServerHeader(iThread) << "end stream viewer" << std::endl;
            } break;

            default:
                std::cout << getServerHeader(iThread) << "unknown client type" << std::endl;
            }

            std::cout << getServerHeader(iThread) << "thread end\t\t\t\t server status : " << getStatus() << std::endl;
            //            Stream::InitPacket initPacket;
            //            sock.read(initPacket);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            //            mProcessClient(sock);
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
    for (const auto& streamer : mStreamers) {
        streamViewersStr += "(" + std::to_string(streamer->id) + "," + std::to_string(streamer->streamViewers.size()) + ")";

        //        for (const auto & streamViewer : streamer->streamViewers) {

        //        }
        streamViewersStr += ",";
    }
    streamViewersStr += "]";

    std::string str = std::string("nbStreamer = ") + std::to_string(mStreamers.size()) + ", nbViewer = " + std::to_string(mViewers.size()) + " " + streamViewersStr;
    return str;
}
