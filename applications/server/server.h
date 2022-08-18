#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
//#include <socket.h>
//#include <queue>
#include <deque>

#include "Socket.hpp"
#include "Sensor.hpp"

// struct Streamer;

// struct StreamViewer {
// };

// struct StreamSyncViewer : public StreamViewer {
// };

#include <mutex>

struct Streamer {
    hub::InputSensor mInputSensor;
    std::string mSensorName;

    std::list<hub::OutputSensor> mOutputSensors;
    std::map<std::string, std::list<hub::OutputSensor>> mSensor2syncViewers;

    Streamer* mSyncMaster = nullptr;
    std::map<std::string, std::deque<hub::Acquisition>> mSensor2acqs;
};

struct Viewer {
    const hub::ClientSocket* const mSock = nullptr;

    void notifyNewStreamer(const Streamer& streamer) const;
};

class Server {
public:
    Server(int port = hub::SERVICE_PORT);

    void run();

    std::string getStatus() const;

private:
    hub::ServerSocket mServerSock;
    std::map<std::string, Streamer*> mStreamers;
    std::list<Viewer*> mViewers;

    std::mutex mMtx;
};

#endif // SERVER_H
