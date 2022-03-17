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
    };

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

    ClientSocket(std::string ipv4, int port);
    ClientSocket(socket_fd fdSock);

    ClientSocket(const ClientSocket& sock) = delete;
    ClientSocket(const ClientSocket&& sock) = delete;
    ClientSocket(ClientSocket& sock) = delete;
    ClientSocket(ClientSocket&& sock);

    ClientSocket& operator=(const ClientSocket& sock) = delete;
    ClientSocket&& operator=(const ClientSocket&& sock) = delete;
    ClientSocket operator=(ClientSocket sock) = delete;
    ClientSocket& operator=(ClientSocket& sock) = delete;
    ClientSocket&& operator=(ClientSocket&& sock) = delete;

    void write(const unsigned char* data, size_t len) const override;
    template <class T>
    void write(const T& t) const;

    void read(unsigned char* data, size_t len) const override;
    template <class T>
    void read(T& t) const;

    void waitClose() const;
    void clear();

private:
    std::string mIpv4;
    int mPort;
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

#ifdef DEBUG_SOCKET
    if (std::is_enum<T>::value) {
        if (std::is_same<T, Message>::value) {
            std::cout << ", " << message2string[(int)(const Message&)t];
        }
        if (std::is_same<T, ClientSocket::Type>::value) {
            std::cout << ", new client : " << ClientSocket::type2string[(int)(const ClientSocket::Type&)t];
        }
    }
    std::cout << std::endl;
#endif
}

template <class T>
void ClientSocket::read(T& t) const
{
    // std::cout << "\033[31mClientSocket::read(T& t) -> IOStream::read(t)\033[0m" << std::endl;
    IOStream::read(t);

#ifdef DEBUG_SOCKET
    if (std::is_enum<T>::value) {
        if (std::is_same<T, Message>::value) {
            std::cout << ", " << message2string[(int)(Message&)t];
        }
        if (std::is_same<T, ClientSocket::Type>::value) {
            std::cout << ", new client : " << ClientSocket::type2string[(int)(ClientSocket::Type&)t];
        }
    }
    std::cout << std::endl;
#endif
}

#endif // SOCKET_H
