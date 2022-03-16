#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <socket.h>
#include <stream.h>
//#include <queue>
#include <deque>

//struct Streamer;

//struct StreamViewer {
//    // public:
//    //     StreamViewer(const StreamViewer &) = default;
//    std::unique_ptr<OutputStream> mOutputStream;
//    Streamer* mSyncMaster = nullptr;
//    Streamer* mStreamer = nullptr;
//    //    std::vector<Stream::Acquisition> mAcqs;
//    std::deque<Stream::Acquisition> mAcqs;
//};

// struct StreamSyncViewer : public StreamViewer {
//     //    InputStream mInputStream;
//     std::vector<Stream::Acquisition> mAcqs;
// };

struct Streamer {
    InputStream mInputStream;
    std::string mSensorName;

    std::list<OutputStream> mOutputStreams;
    //    std::list<StreamViewer> mStreamViewers;
//    std::list<StreamViewer> mSyncViewers;
    std::map<std::string, std::list<OutputStream>> mSensor2syncViewers;

    Streamer* mSyncMaster = nullptr;
    std::map<std::string, std::deque<Stream::Acquisition>> mSensor2acqs;

    //    std::list<InputStream> mInputSlaveStreams;
    //    std::list<OutputStream> mOutputSlaveStreams;
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
    //    std::list<StreamViewer> mStreamViewers;
};

#endif // SERVER_H
