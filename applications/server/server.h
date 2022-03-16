#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <socket.h>
#include <stream.h>
//#include <queue>
#include <deque>

// struct Streamer;

// struct StreamViewer {
// };

// struct StreamSyncViewer : public StreamViewer {
// };

struct Streamer {
    InputStream mInputStream;
    std::string mSensorName;

    std::list<OutputStream> mOutputStreams;
    std::map<std::string, std::list<OutputStream>> mSensor2syncViewers;

    Streamer* mSyncMaster = nullptr;
    std::map<std::string, std::deque<Stream::Acquisition>> mSensor2acqs;
};

struct Viewer {
    const ClientSocket* const mSock = nullptr;

    void notifyNewStreamer(const Streamer& streamer) const;
};

class Server {
public:
    Server(int port = SERVICE_PORT);

    void run();

    std::string getStatus() const;

private:
    ServerSocket mServerSock;
    std::map<std::string, Streamer*> mStreamers;
    std::list<Viewer*> mViewers;
};

#endif // SERVER_H
