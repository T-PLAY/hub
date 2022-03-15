#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <socket.h>
#include <stream.h>
//#include <queue>
#include <deque>

struct Streamer;

struct StreamViewer {
    //public:
    //    StreamViewer(const StreamViewer &) = default;
    std::unique_ptr<OutputStream> mOutputStream;
    Streamer* mSyncMaster = nullptr;
//    std::vector<Stream::Acquisition> mAcqs;
    std::deque<Stream::Acquisition> mAcqs;
};

//struct StreamSyncViewer : public StreamViewer {
//    //    InputStream mInputStream;
//    std::vector<Stream::Acquisition> mAcqs;
//};

struct Streamer {
    InputStream mInputStream;
    //    std::list<OutputStream> mOutputStreams;
    std::list<StreamViewer> mStreamViewers;

    std::list<StreamViewer*> mSyncViewers;
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
};

#endif // SERVER_H
