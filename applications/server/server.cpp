#include "server.h"

#include <cassert>
#include <memory>
#include <stream.h>

static std::string dims2string(const std::vector<int>& dims)
{
    std::string str = "[";
    for (const auto& dim : dims) {
        str += std::to_string(dim) + ", ";
    }
    str[str.size() - 2] = ']';
    str.resize(str.size() - 1);
    return str;
}

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

            ClientSocket::Type clientType;
            sock.read(clientType);

            switch (clientType) {
            case ClientSocket::Type::STREAMER: {

                Streamer streamer { std::move(sock), {} };
                const std::string sensorName = streamer.mInputStream.getSensorName();
                const auto& inputStream = streamer.mInputStream;
                auto& outputStreams = streamer.mOutputStreams;

                assert(mStreamers.find(sensorName) == mStreamers.end());
                mStreamers[sensorName] = &streamer;
                std::cout << getServerHeader(iThread) << "[streamer] new streamer\t\t server status : " << getStatus() << std::endl;

                std::cout << getServerHeader(iThread) << "[streamer] sensor name = '" << sensorName << "'" << std::endl;

                try {

                    // prevent all viewer there is a new streamer
                    std::cout << getServerHeader(iThread) << "[streamer] prevent viewers there is a new streamer" << std::endl;
                    for (const auto& viewer : mViewers) {
                        viewer->notifyNewStreamer(streamer);
                    }

                    const size_t acquisitionSize = inputStream.getAcquisitionSize();
                    std::cout << getServerHeader(iThread) << "[streamer] acquisitionSize:" << acquisitionSize << std::endl;
                    std::cout << getServerHeader(iThread) << "[streamer] dims:" << dims2string(inputStream.getDims()) << std::endl;
                    // std::cout << getServerHeader(iThread) << "[streamer] height:" << inputStream.getDims().at(1) << std::endl;
                    std::cout << getServerHeader(iThread) << "[streamer] format:" << Stream::format2string[(int)inputStream.getFormat()] << " (byte:" << Stream::format2nByte[(int)inputStream.getFormat()] << ")" << std::endl;

                    Stream::Acquisition acq;
                    // for each new stream acquistion
                    while (true) {

                        //                        if (!outputStreams.empty()) {
                        //                            const auto start = std::chrono::high_resolution_clock::now();
//                        const auto start = std::chrono::high_resolution_clock::now();

                        inputStream >> acq;

                        // std::cout << getServerHeader(iThread) << "[streamer] receive data from streamer '" << inputStream.getSensorName() << "' and send it for " << streamer.mOutputStreams.size() << " stream viewers" << std::endl;

                        // broadcast data
                        // stream new acquisition for all viewers of this stream
                        auto it = outputStreams.cbegin();
                        while (it != outputStreams.cend()) {
                            const auto& outputStream = *it;

                            try {
                                outputStream << acq;
                                ++it;

                            } catch (Socket::exception& e) {
                                std::cout << getServerHeader(iThread) << "[streamer] out : catch socket exception : " << e.what() << std::endl;
                                it = outputStreams.erase(it);
                                std::cout << getServerHeader(iThread) << "[streamer] out : end stream viewer\t server status : " << getStatus() << std::endl;
                                std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
                            } catch (std::exception& e) {
                                std::cout << getServerHeader(iThread) << "[streamer] out : catch exception : " << e.what() << std::endl;
                                throw;
                            }
                        }

                        //                        const auto end = std::chrono::high_resolution_clock::now();
                        //                        const auto fps = (1'000'000) / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                        //                        std::cout << "fps : " << fps << std::endl;

                        //                const auto maxFps = 60;
                        //                const auto now = std::chrono::high_resolution_clock::now();
                        //                std::cout << "sleep for " <<  (start + std::chrono::microseconds(1'000'000 / maxFps) - now).count() << std::endl;
                        //                std::this_thread::sleep_until(start + std::chrono::microseconds(1'000'000 / maxFps));

                        // std::cout << getServerHeader(iThread) << "[streamer] data from streamer sent for " << outputStreams.size() << " stream viewers" << std::endl;

                        //                            const auto maxFps = 60;
                        //                            const auto end = start + std::chrono::microseconds(1'000'000 / maxFps);
                        //                            std::this_thread::sleep_until(end);

                        //                        } else {
                        //                            inputStream.ping();
                        //                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        //                        }
                    } // while (true)

                } catch (Socket::exception& e) {
                    std::cout << getServerHeader(iThread) << "[streamer] in : catch socket exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "[streamer] in : catch exception : " << e.what() << std::endl;
                    throw;
                }
                mStreamers.erase(sensorName);

                //                for (auto& outputStream : streamer.mOutputStreams) {
                //                    outputStream.close();
                //                }

                for (const auto* viewer : mViewers) {
                    viewer->mSock->write(Socket::Message::DEL_STREAMER);
                    viewer->mSock->write(inputStream.getSensorName());
                }

                std::cout << getServerHeader(iThread) << "[streamer] end" << std::endl;
            } break;

            case ClientSocket::Type::VIEWER: {

                Viewer viewer { &sock };

                // for each streamer, open stream viewer socket
                for (const auto& pair : mStreamers) {
                    const auto& streamer = *pair.second;
                    viewer.notifyNewStreamer(streamer);
                }

                mViewers.push_back(&viewer);
                std::cout << getServerHeader(iThread) << "[viewer] new viewer\t\t\t server status : " << getStatus() << std::endl;

                try {
                    // check client still alive
                    while (true) {
                        sock.write(Socket::Message::PING);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }

                } catch (Socket::exception& e) {
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

            case ClientSocket::Type::STREAM_VIEWER: {
                std::cout << getServerHeader(iThread) << "[stream viewer] new stream viewer\t server status : " << getStatus() << std::endl;

                std::string sensorName;
                sock.read(sensorName);

                if (mStreamers.find(sensorName) != mStreamers.end()) {
                    Streamer* streamer = mStreamers.at(sensorName);

                    streamer->mOutputStreams.emplace_back(std::move(sock), streamer->mInputStream);
                } else {
                    std::cout << getServerHeader(iThread) << "[stream viewer] unknown sensor name : '" << sensorName << "'" << std::endl;
                }

                std::cout << getServerHeader(iThread) << "[stream viewer] end" << std::endl;
            } break;

            default:
                std::cout << getServerHeader(iThread) << "unknown client type" << std::endl;
            }

            std::cout << getServerHeader(iThread) << "thread end\t\t\t\t server status : " << getStatus() << std::endl;
            std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
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
        std::string str = sensorName.substr(0, 3);
        streamViewersStr += "(" + str + "," + std::to_string(streamer->mOutputStreams.size()) + ")";

        streamViewersStr += ",";
    }
    streamViewersStr += "]";

    std::string str = std::string("nbStreamer = ") + std::to_string(mStreamers.size()) + ", nbViewer = " + std::to_string(mViewers.size()) + " " + streamViewersStr;
    return str;
}

void Viewer::notifyNewStreamer(const Streamer& streamer) const
{
    mSock->write(Socket::Message::NEW_STREAMER);
    mSock->write(streamer.mInputStream.getSensorName());

    mSock->write(std::string(Stream::format2string[(int)streamer.mInputStream.getFormat()]));

    std::string dimStr;
    for (const auto dim : streamer.mInputStream.getDims()) {
        dimStr += std::to_string(dim) + " x ";
    }
    dimStr.resize(dimStr.size() - 3);
    mSock->write(dimStr);
    mSock->write(std::to_string(streamer.mInputStream.getAcquisitionSize()));
}
