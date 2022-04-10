#include "server.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <socket.h>
#include <stream.h>
#include <utility>
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
        sock.setIsServer(true);
        std::cout << getServerHeader(0) << "new client" << std::endl;

        std::thread thread([this, iThread, sock = std::move(sock)]() mutable {
            std::cout << getServerHeader(iThread) << "new thread\t\t\t\t" << getStatus() << std::endl;

            ClientSocket::Type clientType;
            sock.read(clientType);

            switch (clientType) {
            case ClientSocket::Type::STREAMER: {

                std::string sensorName;
                sock.read(sensorName);
                if (mStreamers.find(sensorName) == mStreamers.end()) {
                    sock.write(Socket::Message::NOT_FOUND);
                } else {
                    sock.write(Socket::Message::FOUND);
                    std::cout << getServerHeader(iThread) << "[stream viewer] stream sensor name : '" << sensorName << "' already exist" << std::endl;
                    break;
                }
                assert(mStreamers.find(sensorName) == mStreamers.end());

                //                Streamer streamer { { std::move(sock), sensorName }, sensorName, {}, {}, nullptr, {} };
                //                Streamer streamer { InputStream(std::move(sock)), sensorName, {}, {}, nullptr, {} };
                Streamer streamer { InputStream(std::move(sock)), sensorName, {}, {}, nullptr, {} };
                //                const std::string sensorName = streamer.mInputStream.getSensorName();
                // sensor is unique
                //                streamer.mSensorName = sensorName;
                const auto& inputStream = streamer.mInputStream;
                auto& outputStreams = streamer.mOutputStreams;
                auto& sensor2syncViewers = streamer.mSensor2syncViewers;
                auto& sensor2acqs = streamer.mSensor2acqs;
                auto& syncMaster = streamer.mSyncMaster;

                // sensor is unique
                //                if (mStreamers.find(sensorName) != mStreamers.end())
                //                    break;

                assert(mStreamers.find(sensorName) == mStreamers.end());
                mStreamers[sensorName] = &streamer;
                std::cout << getServerHeader(iThread) << "[streamer] new streamer\t\t" << getStatus() << std::endl;

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
                    const Stream::MetaData& metadata = inputStream.getMetaData();
                    for (const auto& pair : metadata) {
                        const auto& name = pair.first;
                        const auto& val = pair.second;
                        //                        std::cout << getServerHeader(iThread) << "[streamer] metadata: " << val.type().name() << " " << name << " = " << std::endl;
                        std::cout << getServerHeader(iThread) << "[streamer] metadata: " << val.type().name() << " " << name << " = '" << any::to_string(val) << "'" << std::endl;
                    }
//                    std::cout << "metadata: " << Stream::to_string(metadata) << std::endl;

                    Stream::Acquisition acq;
                    // for each new stream acquistion
                    while (true) {

                        inputStream >> acq;

                        // broadcast data
                        // send new acquisition for all no sync stream viewers of this stream
                        {
                            auto it = outputStreams.begin();
                            while (it != outputStreams.end()) {
                                const auto& outputStream = *it;
                                try {
                                    // std::cout << getServerHeader(iThread) << "[streamer] receive and send acq : \t" << acq << std::endl;
                                    outputStream << acq;

                                    ++it;

                                } catch (std::exception& e) {
                                    // no sync stream viewer lost
                                    std::cout << getServerHeader(iThread) << "[streamer] out : catch outputStream exception : " << e.what() << std::endl;
                                    it = outputStreams.erase(it);

                                    std::cout << getServerHeader(iThread) << "[streamer] out : end stream viewer\t" << getStatus() << std::endl;
                                    std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
                                }
                            }
                        }

                        // save new acquisition for all sync stream viewers of this stream
                        if (syncMaster != nullptr) {
                            Streamer* root = syncMaster;
                            while (root->mSyncMaster != nullptr)
                                root = root->mSyncMaster;

                            // std::cout << getServerHeader(iThread) << "[streamer] save acq : \t\t" << acq << std::endl;
                            root->mSensor2acqs[sensorName].emplace_front(acq.clone());
                        }

                        // for all synchronize viewers of this stream, send best match acquisition
                        // according to the current acquisition
                        {
                            auto it = sensor2syncViewers.begin();
                            while (it != sensor2syncViewers.end()) {
                                auto& pair = *it;
                                const auto& syncSensorName = pair.first;
                                auto& outputStreams = pair.second;

                                assert(!outputStreams.empty());
                                auto& acqs = sensor2acqs[syncSensorName];
                                while (acqs.empty()) {
                                    std::cout << getServerHeader(iThread) << "[streamer] empty acqs, sleep" << std::endl;
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                }

                                Stream::Acquisition bestMatchAcq = std::move(acqs.back());
                                // search best match acq
                                {
                                    acqs.pop_back();
                                    auto minDist = std::abs(acq.mBackendTimestamp - bestMatchAcq.mBackendTimestamp);
                                    //                                    const auto period = bestMatchAcq.mBackendTimeOfArrival - bestMatchAcq.mBackendTimestamp;

                                    bool foundBestMatch = false;
                                    //                                    while (!foundBestMatch && !acqs.empty()) {
                                    while (!foundBestMatch) {
                                        //                                    while (!foundBestMatch && minDist > period / 2) {
                                        while (acqs.empty()) {
                                            //                                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                                        }
                                        auto& acq2 = acqs.back();
                                        auto dist = std::abs(acq.mBackendTimestamp - acq2.mBackendTimestamp);
                                        if (dist < minDist) {
                                            minDist = dist;
                                            // std::cout << getServerHeader(iThread) << "[streamer] delete acq : \t\t" << bestMatchAcq << std::endl;
                                            bestMatchAcq = std::move(acq2);
                                            acqs.pop_back();
                                        } else {
                                            foundBestMatch = true;
                                        }
                                    }

                                    //                                    assert(minDist < 1000 + std::min(acq.mBackendTimeOfArrival - acq.mBackendTimestamp, bestMatchAcq.mBackendTimeOfArrival - bestMatchAcq.mBackendTimestamp) / 2);
                                    // std::cout << getServerHeader(iThread) << "[streamer] best acq : \t\t" << bestMatchAcq << "\t(" << acqs.size() << ")" << std::endl;
                                    std::cout << "timestamp start diff : " << minDist / 1000.0 << " milliseconds" << std::endl;

                                    acqs.emplace_back(bestMatchAcq.clone());
                                }

                                // broadcast best match acquisition fo each stream viewers
                                auto it2 = outputStreams.begin();
                                while (it2 != outputStreams.end()) {
                                    auto& outputStream = *it2;

                                    try {
                                        outputStream << bestMatchAcq;
                                        // std::cout << "----------" << std::endl;
                                        ++it2;

                                    } catch (std::exception& e) {
                                        std::cout << getServerHeader(iThread) << "[viewer] catch sync viewer exception : " << e.what() << std::endl;
                                        it2 = outputStreams.erase(it2);
                                    }
                                }

                                if (outputStreams.empty()) {
                                    mStreamers.at(syncSensorName)->mSyncMaster = nullptr;
                                    assert(it2 == outputStreams.end());
                                    it = sensor2syncViewers.erase(it);
                                    std::cout << getServerHeader(iThread) << "[streamer] out : end sync viewer\t" << getStatus() << std::endl;
                                } else {

                                    ++it;
                                }
                            } // while (it != sensor2syncViewers.end())
                        }
                    } // while (true)

                    //                } catch (Socket::exception& e) {
                    //                    std::cout << getServerHeader(iThread) << "[streamer] in : catch inputStream exception : " << e.what() << std::endl;
                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "[streamer] in : catch inputStream exception : " << e.what() << std::endl;
                    //                    throw;
                }
                mStreamers.erase(sensorName);

                for (const auto& pair : sensor2syncViewers) {
                    const auto& syncSensorName = pair.first;
                    mStreamers.at(syncSensorName)->mSyncMaster = nullptr;
                }

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
                std::cout << getServerHeader(iThread) << "[viewer] new viewer\t\t\t" << getStatus() << std::endl;

                try {
                    // check client still alive
                    while (true) {
                        sock.write(Socket::Message::PING);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }

                } catch (std::exception& e) {
                    std::cout << getServerHeader(iThread) << "[viewer] catch viewer exception : " << e.what() << std::endl;
                }

                mViewers.remove(&viewer);
                std::cout << getServerHeader(iThread) << "[viewer] end" << std::endl;

                sock.clear();

            } break;

            case ClientSocket::Type::STREAM_VIEWER: {

                std::string sensorName;
                sock.read(sensorName);
                if (mStreamers.find(sensorName) == mStreamers.end()) {
                    sock.write(Socket::Message::NOT_FOUND);
                    std::cout << getServerHeader(iThread) << "[stream viewer] unknown sensor name : '" << sensorName << "'" << std::endl;
                    break;
                } else {
                    sock.write(Socket::Message::OK);
                }
                assert(mStreamers.find(sensorName) != mStreamers.end());

                std::string syncSensorName;
                sock.read(syncSensorName);
                if (syncSensorName != "" && mStreamers.find(syncSensorName) == mStreamers.end()) {
                    sock.write(Socket::Message::NOT_FOUND);
                    std::cout << getServerHeader(iThread) << "[stream viewer] unknown sync sensor name : '" << syncSensorName << "'" << std::endl;
                    break;
                } else {
                    sock.write(Socket::Message::OK);
                }
                assert(syncSensorName == "" || mStreamers.find(syncSensorName) != mStreamers.end());

                std::cout << getServerHeader(iThread) << "[stream viewer] new stream viewer\t" << getStatus() << std::endl;

                Streamer* streamer = mStreamers.at(sensorName);

                const auto& inputStream = streamer->mInputStream;
                if (syncSensorName == "") {
                    //                    streamer->mOutputStreams.emplace_back(std::move(sock), streamer->mInputStream);
                    //                    streamer->mOutputStreams.emplace_back(sensorName, inputStream.getFormat(), inputStream.getDims(), std::move(sock), inputStream.getMetaData());
                    //                    streamer->mOutputStreams.push_back(OutputStream(sensorName, inputStream.getFormat(), inputStream.getDims(), std::move(sock), inputStream.getMetaData()));

                    // here
                    //                    streamer->mOutputStreams.emplace_back(sensorName, inputStream.getFormat(), inputStream.getDims(), ClientSocket(std::move(sock)), inputStream.getMetaData());
                    streamer->mOutputStreams.emplace_back(sensorName, inputStream.getFormat(), inputStream.getDims(), ClientSocket(std::move(sock)), inputStream.getMetaData());

                    //                    streamer->mOutputStreams.emplace_back(OutputStream(ClientSocket(std::move(sock))));
                    //                    streamer->mOutputStreams.emplace_back(std::move(sock), Stream::Format::BGR8, {}, {});
                    //                    streamer->mOutputStreams.push_back(OutputStream(std::move(sock), Stream::Format::BGR8, {}, {}));
                    //                    streamer->mOutputStreams.emplace_back(std::move(sock), Stream::Format::BGR8, std::vector<int>(), Stream::MetaData());
                    //                    streamer->mOutputStreams.push_back(ClientSocket(std::move(sock), Stream::Format::BGR8, std::vector<int>(), Stream::MetaData()));

                    //                    streamer->mOutputStreams.emplace_back(ClientSocket(std::move(sock)));
                    //                    streamer->mOutputStreams.emplace_back(std::forward<ClientSocket&&>(sock));
                    //                    streamer->mOutputStreams.push_back(std::move(sock));
                    //                    streamer->mOutputStreams.insert(ClientSocket(std::move(sock)));

                } else {
                    Streamer* syncMaster = mStreamers.at(syncSensorName);

                    //                    syncMaster->mSensor2syncViewers[sensorName].emplace_back(std::move(sock), streamer->mInputStream);
                    //                    syncMaster->mSensor2syncViewers[sensorName].push_back(OutputStream(sensorName, inputStream.getFormat(), inputStream.getDims(), std::move(sock), inputStream.getMetaData()));

                    // here
                    //                    syncMaster->mSensor2syncViewers[sensorName].emplace_back(sensorName, inputStream.getFormat(), inputStream.getDims(), ClientSocket(std::move(sock)), inputStream.getMetaData());
                    syncMaster->mSensor2syncViewers[sensorName].emplace_back(sensorName, inputStream.getFormat(), inputStream.getDims(), ClientSocket(std::move(sock)), inputStream.getMetaData());

                    //                    syncMaster->mSensor2syncViewers[sensorName].emplace_back(std::move(sock));
                    assert(streamer->mSyncMaster == nullptr || streamer->mSyncMaster == syncMaster);
                    streamer->mSyncMaster = syncMaster;
                }

                std::cout << getServerHeader(iThread) << "[stream viewer] end" << std::endl;
            } break;

            default:
                std::cout << getServerHeader(iThread) << "unknown client type" << std::endl;
            }

            std::cout << getServerHeader(iThread) << "thread end\t\t\t\t" << getStatus() << std::endl;
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
        streamViewersStr += "(" + str + "," + std::to_string(streamer->mOutputStreams.size()) + "," + std::to_string(streamer->mSensor2syncViewers.size()) + ")";

        streamViewersStr += ",";
    }
    streamViewersStr += "]";

    std::string str = std::string("status : nbStreamer:") + std::to_string(mStreamers.size()) + ", nbViewer:" + std::to_string(mViewers.size()) + " " + streamViewersStr;
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

    mSock->write(streamer.mInputStream.getMetaData());
}
