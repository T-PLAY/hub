#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <socket.h>
#include <stream.h>

// struct StreamViewer {
// };

struct Viewer {
    const ClientSocket* const socket = nullptr;
};

struct Streamer {
    InputStream mInputStream;
    std::list<OutputStream> mOutputStreams;
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
