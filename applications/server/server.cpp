#include "server.h"

#include <cassert>
#include <memory>
#include <stream.h>
#include <vector>

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
                //                auto& outputStreams = streamer.mOutputStreams;
                auto& streamViewers = streamer.mStreamViewers;

                assert(mStreamers.find(sensorName) == mStreamers.end());
                mStreamers[sensorName] = &streamer;
                std::cout << getServerHeader(iThread) << "[streamer] new streamer\t\t\t server status : " << getStatus() << std::endl;

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

                        //                         std::cout << getServerHeader(iThread) << "[streamer] receive data from streamer '" << inputStream.getSensorName() << "' and send it for " << streamer.mStreamViewers.size() << " stream viewers" << std::endl;
                        //                        std::cout << getServerHeader(iThread) << "[streamer] receive acq " << acq << std::endl;

                        // broadcast data
                        // stream new acquisition for all viewers of this stream
                        auto it = streamViewers.begin();
                        while (it != streamViewers.end()) {
                            //                            const auto& outputStream = *it;
                            auto& streamViewer = *it;

                            try {
                                if (streamViewer.mSyncMaster != nullptr) {
                                    std::cout << getServerHeader(iThread) << "[streamer] save acq : \t\t" << acq << std::endl;
                                    //                                    Stream::Acquisition acqTest;
                                    //                                    Stream::Acquisition acq2 = acqTest;
                                    //                                    streamViewer.mAcqs.push_back(acq);
                                    streamViewer.mAcqs.push_front(acq.clone());

                                } else {
                                    std::cout << getServerHeader(iThread) << "[streamer] receive and send acq : \t" << acq << std::endl;
                                    *streamViewer.mOutputStream << acq;

                                    auto it2 = streamer.mSyncViewers.begin();
                                    while (it2 != streamer.mSyncViewers.end()) {
                                        auto& syncViewer = *it2;
                                        //                                    for (const auto& syncViewer : streamer.mSyncViewers) {
                                        auto& acqs = syncViewer->mAcqs;
                                        //                                        assert(acqs.size() > 0);
                                        while (acqs.empty()) {
                                            std::cout << getServerHeader(iThread) << "[streamer] empty acqs" << std::endl;
                                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                        }

                                        Stream::Acquisition bestMatchAcq = std::move(acqs.back());
                                        acqs.pop_back();
                                        auto minDist = std::abs(acq.mBackendTimestamp - bestMatchAcq.mBackendTimestamp);

                                        bool foundBestMatch = false;
                                        while (!acqs.empty() && !foundBestMatch) {
                                            auto& acq2 = acqs.back();
                                            auto dist = std::abs(acq.mBackendTimestamp - acq2.mBackendTimestamp);
                                            if (dist < minDist) {
                                                minDist = dist;
                                                std::cout << getServerHeader(iThread) << "[streamer] delete acq : \t\t" << bestMatchAcq << std::endl;
                                                bestMatchAcq = std::move(acq2);
                                                acqs.pop_back();
                                            } else {
                                                foundBestMatch = true;
                                            }
                                        }
                                        std::cout << getServerHeader(iThread) << "[streamer] best acq : \t\t" << bestMatchAcq << "\t(" << acqs.size() << ")" << std::endl;

                                        acqs.push_back(bestMatchAcq.clone());

                                        //                                    for (const auto & syncViewerAcq : syncViewer->mAcqs) {
                                        //                                        for (int i = 1; i < acqs.size(); ++i) {
                                        //                                            const auto& curAcq = acqs[i];

                                        //                                            auto dist = std::abs(acq.mBackendTimestamp - curAcq.mBackendTimestamp);
                                        //                                            if (dist < minDist) {
                                        //                                                minDist = dist;
                                        //                                                bestMatchAcq = curAcq;
                                        //                                            }
                                        //                                        }

                                        try {
                                            *syncViewer->mOutputStream << bestMatchAcq;
                                            std::cout << "----------" << std::endl;
                                            ++it2;
                                        } catch (std::exception& e) {
                                            it2 = streamer.mSyncViewers.erase(it2);

//                                            *syncViewer->mSyncMaster->mStreamViewers.remove()

                                            std::cout << getServerHeader(iThread) << "[streamer] out : end sync viewer\t server status : " << getStatus() << std::endl;
                                        }

                                        //                                        syncViewer->mAcqs.clear();
                                    }
                                }

                                ++it;

                            } catch (Socket::exception& e) {
                                std::cout << getServerHeader(iThread) << "[streamer] out : catch socket exception : " << e.what() << std::endl;
                                it = streamViewers.erase(it);
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
                std::cout << getServerHeader(iThread) << "[stream viewer] new stream viewer\t\t server status : " << getStatus() << std::endl;

                std::string sensorName;
                sock.read(sensorName);

                std::string syncSensorName;
                sock.read(syncSensorName);
                assert(syncSensorName == "" || mStreamers.find(syncSensorName) != mStreamers.end());

                if (mStreamers.find(sensorName) != mStreamers.end()) {
                    Streamer* streamer = mStreamers.at(sensorName);

                    //                    streamer->mOutputStreams.emplace_back(std::move(sock), streamer->mInputStream);
                    //                    streamer->mStreamViewers.emplace_back(std::move(sock), streamer->mInputStream);
                    //                    streamer->mStreamViewers.emplace_back(OutputStream(std::move(sock), streamer->mInputStream));
                    //                    streamer->mStreamViewers.emplace_back({std::move(sock), streamer->mInputStream});
                    //                    OutputStream && outputStream = OutputStream(std::move(sock), streamer->mInputStream);
                    //                    StreamViewer && streamViewer{{std::move(sock), streamer->mInputStream}};
                    //                    streamer->mStreamViewers.push_back({{std::move(sock), streamer->mInputStream}});
                    StreamViewer streamViewer;
                    streamViewer.mOutputStream = std::make_unique<OutputStream>(std::move(sock), streamer->mInputStream);
                    streamer->mStreamViewers.emplace_back(std::move(streamViewer));

                    StreamViewer& streamViewer2 = streamer->mStreamViewers.back();

                    if (syncSensorName != "") {
                        Streamer* syncMaster = mStreamers.at(syncSensorName);
                        streamViewer2.mSyncMaster = syncMaster;
                        syncMaster->mSyncViewers.push_back(&streamViewer2);
                    }

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
        //        streamViewersStr += "(" + str + "," + std::to_string(streamer->mOutputStreams.size()) + ")";
        streamViewersStr += "(" + str + "," + std::to_string(streamer->mStreamViewers.size()) + ")";

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
