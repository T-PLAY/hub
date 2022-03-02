#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <socket.h>
#include <stream.h>
#include <map>

//struct StreamViewer {
////    OutputStream* mOutputStream = nullptr;
//    OutputStream mOutputStream;
//};

struct Viewer {
    const ClientSocket* const socket = nullptr;
};

struct Streamer {
//    InputStream* mInputStream = nullptr;
    InputStream mInputStream;
//    InputStream mInputStream;
//    std::list<OutputStream> streamViewers;
    std::list<OutputStream> mOutputStreams;
};

class Server {
public:
//    enum class Message {
//        NONE,
//        PING,
//        //        CLOSE,
//        NEW_STREAMER,
//        DEL_STREAMER,
//        //        OPEN_INPUT_STREAM,
//        SYNC,
//        OK,
//        CLOSE,
//        DATA,
//        COUNT,
//    };

    Server(int port = SERVICE_PORT);

    void run();

    std::string getStatus() const;

private:
    ServerSocket mServerSock;
//    std::list<Streamer*> mStreamers;
    std::map<std::string, Streamer*> mStreamers;
    std::list<Viewer*> mViewers;
};

#endif // SERVER_H
