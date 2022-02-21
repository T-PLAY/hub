#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <socket.h>
//#include <list>
#include <stream.h>

struct Viewer {
    const ClientSocket * socket = nullptr;
};

struct Streamer {
    Stream::InitPacket initPacket;
    const ClientSocket * socket = nullptr;
    int id;
    std::list<Viewer*> viewers;
};


class Server
{
public:
    enum Message {
        NONE,
//        CLOSE,
        NEW_STREAMER,
//        OPEN_INPUT_STREAM,
        COUNT,
    };
//    Server(std::function<void(Socket & sock)> processClient, int port = 4043);
    Server(int port = 4043);

    void run();

private:
    ServerSocket mServerSock;
    std::list<Streamer*> mStreamers;
//    std::vector<ClientSocket> mStreamerSockets;
    std::list<Viewer*> mViewers;
    int m_iStreamer = 0;
//    std::vector<ClientSocket> mViewerSockets;
//    int mFdSock;
//    const std::function<void(Socket & sock)> mProcessClient;
};

#endif // SERVER_H
