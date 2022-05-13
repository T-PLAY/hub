#ifndef SOCKET_H
#define SOCKET_H

#define NOMINMAX

#include <net.h>

#include <IOStream.h>
#include <exception>
#include <functional>
#include <list>
#include <string>

//#define DEBUG_SOCKET

constexpr int SERVICE_PORT = 4043;
constexpr const char* SERVICE_IP = "127.0.0.1";

class Socket {
public:
    class exception : public std::runtime_error {
    public:
        explicit exception(const char* const message) throw()
            : std::runtime_error(message)
        {
        }
        const char* what() const throw()
        {
            return std::runtime_error::what();
        }
    };

    enum class Message {
        NONE,
        PING,
        SYNC,
        DATA,
        OK,
        CLOSE,
        DEL_STREAMER,
        NEW_STREAMER,
        NOT_FOUND,
        FOUND,
        COUNT
    };
    static constexpr char const* message2string[static_cast<int>(Message::COUNT)] = {
        "NONE",
        "PING",
        "SYNC",
        "DATA",
        "OK",
        "CLOSE",
        "DEL_STREAMER",
        "NEW_STREAMER",
        "NOT_FOUND",
        "FOUND",
    };
    friend std::ostream& operator<<(std::ostream& os, const Message& msg)
    {
        os << message2string[(int)msg];
        return os;
    }

    bool isConnected() const;

protected:
    Socket();
    ~Socket();

protected:
    socket_fd mFdSock = INVALID_SOCKET;
};

class ClientSocket : public Socket, public IOStream {
public:
    enum class Type {
        NONE,
        STREAMER,
        VIEWER,
        STREAM_VIEWER,
        COUNT
    };
    static constexpr char const* type2string[static_cast<int>(Type::COUNT)] = {
        "NONE",
        "STREAMER",
        "VIEWER",
        "STREAM_VIEWER",
    };
    friend std::ostream& operator<<(std::ostream& os, const Type& type)
    {
        os << type2string[(int)type];
        return os;
    }

    void connectToServer();

    ClientSocket(const std::string& ipv4 = SERVICE_IP, int port = SERVICE_PORT); // client : streamer
    ClientSocket(const std::string& sensorName, const std::string& syncSensorName = "", const std::string ipv4 = SERVICE_IP, int port = SERVICE_PORT); // client : stream viewer
    ClientSocket(socket_fd fdSock); // server side client (bind and listen)

    ClientSocket(const ClientSocket& sock) = delete;
    ClientSocket(const ClientSocket&& sock) = delete;
    ClientSocket(ClientSocket& sock) = delete;
    ClientSocket(ClientSocket&& sock);

    ClientSocket& operator=(const ClientSocket& sock) = delete;
    ClientSocket&& operator=(const ClientSocket&& sock) = delete;
    ClientSocket operator=(ClientSocket sock) = delete;
    ClientSocket& operator=(ClientSocket& sock) = delete;
    ClientSocket&& operator=(ClientSocket&& sock) = delete;

    ~ClientSocket();

    void close() override;

    void write(const unsigned char* data, size_t len) const override;
    template <class T>
    void write(const T& t) const;

    void read(unsigned char* data, size_t len) const override;
    template <class T>
    void read(T& t) const;

    void waitClose() const;
    void clear();

    void setupOutput(const std::string& sensorName) const override;

    void setIsServer(bool isServer);

private:
    std::string mIpv4;
    int mPort;
    bool mIsServer = false;
};

class ServerSocket : public Socket {
public:
    ServerSocket(int port);

    ClientSocket waitNewClient();

private:
    int mPort;
    struct sockaddr_in mAddress;
};

///////////////////////////////////////////////////////////////////////////////

static std::string getHeader(socket_fd iSock)
{
    std::string str = "\033[" + std::to_string(31 + iSock % 7) + "m[socket:" + std::to_string(iSock) + "]\033[0m ";
    return str;
}

template <class T>
void ClientSocket::write(const T& t) const
{
    // std::cout << "\033[31mClientSocket::write(const T& t) -> IOStream::write(t)\033[0m" << std::endl;
    IOStream::write(t);

    //#ifdef DEBUG_SOCKET
    //#endif
}

template <class T>
void ClientSocket::read(T& t) const
{
    // std::cout << "\033[31mClientSocket::read(T& t) -> IOStream::read(t)\033[0m" << std::endl;
    IOStream::read(t);

    //#ifdef DEBUG_SOCKET
    //#endif
}

#endif // SOCKET_H
