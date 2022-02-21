#include "server.h"

//#include <net.h>
//#include <socket.h>
//#include <net.h>
#include <cassert>
#include <stream.h>

// Server::Server(std::function<void (Socket & sock)> processClient, int port)
Server::Server(int port)
    : mServerSock(port)
//    , mProcessClient(processClient)
{
}

void Server::run()
{
    //    constexpr size_t imgSize = 192 * 512;
    //    unsigned char img[imgSize];
    //    SOCKET new_socket;
    while (true) {

        ClientSocket sock = mServerSock.waitNewClient();

        std::thread thread([this, sock = std::move(sock)]() {
            Client::Type clientType;
            sock.read(clientType);

            switch (clientType) {
            case Client::Type::STREAMER: {
                std::cout << "[server] new streamer" << std::endl;
                //                Stream::InitPacket initPacket;
                Streamer streamer;
                sock.read(streamer.initPacket);
                streamer.socket = &sock;
                streamer.id = m_iStreamer++;

                mStreamers.push_back(&streamer);
                //                std::cout << "type:" << (int)streamer.mType << std::endl;
                std::cout << "device:" << (int)streamer.initPacket.mDevice << std::endl;
                std::cout << "format:" << (int)streamer.initPacket.mFormat << std::endl;
                std::cout << "height:" << (int)streamer.initPacket.mHeight << std::endl;
                std::cout << "sensor:" << (int)streamer.initPacket.mSensor << std::endl;
                std::cout << "width:" << (int)streamer.initPacket.mWidth << std::endl;

                // prevent all viewer there is a new streamer
                for (const auto* viewer : mViewers) {
                    viewer->socket->write(streamer.id);
                }

                const int acquistionSize = streamer.initPacket.mWidth
                    * streamer.initPacket.mHeight
                    * Stream::formatNbByte[static_cast<int>(streamer.initPacket.mFormat)];
                char data[acquistionSize];
                std::cout << "acquisitionSize:" << acquistionSize << std::endl;

                // for each new stream acquistion
                while (true) {
                    char sync = 's';
                    sock.write(sync);

                    Stream::TimestampInterval timestampInterval;
                    sock.read(timestampInterval);
                    //                    sock.read()
                    sock.read(data, acquistionSize);

                    // stream new acquisition for all viewers of this stream
                    for (const auto* viewer : streamer.viewers) {
                        viewer->socket->write(timestampInterval);
                        viewer->socket->write(data, acquistionSize);
                    }
                    //                    sock.read(data);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                //                for (const auto & viewer : mViewers) {
                //                }
            } break;

            case Client::Type::VIEWER:
                std::cout << "[server] new viewer" << std::endl;
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
                break;

            case Client::Type::STREAM_VIEWER: {
                std::cout << "[server] new stream viewer" << std::endl;
                int iStreamer;
                sock.read(iStreamer);

                Streamer* streamerI = nullptr;
                for (auto* streamer : mStreamers) {
                    if (streamer->id == iStreamer) {
                        streamerI = streamer;
                        break;
                    }
                }
                assert(streamerI != nullptr);
                sock.write(streamerI->initPacket);


//                Viewer viewer;
//                viewer.socket = std::move(sock);
//                streamerI->viewers.push_back({sock});


            } break;

            default:
                std::cout << "[server] unknown client type" << std::endl;
            }

            //            Stream::InitPacket initPacket;
            //            sock.read(initPacket);

            //            mProcessClient(sock);
        });
        thread.detach();
    }
}
