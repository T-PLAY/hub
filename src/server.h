#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <socket.h>
//#include <list>
#include <stream.h>

struct StreamViewer {
    const ClientSocket * const socket = nullptr;
//    ClientSocket socket;
};

struct Viewer {
    const ClientSocket * const socket = nullptr;
//    ClientSocket socket;
};

struct Streamer {
    Stream::InitPacket initPacket;
    const ClientSocket * socket = nullptr;
    int id;
    std::list<StreamViewer*> streamViewers;
};


class Server
{
public:
    enum class Message {
        NONE,
        PING,
//        CLOSE,
        NEW_STREAMER,
        DEL_STREAMER,
//        OPEN_INPUT_STREAM,
        SYNC,
        OK,
        CLOSE,
        DATA,
        COUNT,
    };
//    Server(std::function<void(Socket & sock)> processClient, int port = 4043);
    Server(int port = SERVICE_PORT);

    void run();

    std::string getStatus() const;

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
