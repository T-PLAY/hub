#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <socket.h>
#include <stream.h>

// struct StreamViewer {
// };
struct Streamer {
    InputStream mInputStream;
    std::list<OutputStream> mOutputStreams;
};

struct Viewer {
    const ClientSocket* const mSock = nullptr;

    void notifyNewStreamer(const Streamer & streamer) const;
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
