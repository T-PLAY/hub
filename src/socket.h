#ifndef SOCKET_H
#define SOCKET_H

#include <net.h>

#include <exception>
#include <functional>
#include <list>
#include <string>

#define DEBUG_MSG

static std::string getHeader(socket_fd iSock)
{
    std::string str = "\033[" + std::to_string(31 + iSock % 7) + "m[socket:" + std::to_string(iSock) + "]\033[0m ";
    return str;
}

///////////////////////////////////////////////////////////////////////////////

class Client {
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
};

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
    bool isConnected() const;
    enum class Message {
        NONE,
        PING,
        SYNC,
        DATA,
        OK,
        CLOSE,
        DEL_STREAMER,
        NEW_STREAMER,
        COUNT,
    };

    static constexpr char const* message2string[static_cast<int>(Message::COUNT)] = {
        "NONE",
        "PING",
        "SYNC",
        "DATA",
        "OK",
        "CLOSE",
        "DEL_STREAMER",
        "NEW_STREAMER"
    };

protected:
    Socket();

    ~Socket();

protected:
    socket_fd mFdSock = -1;
};

class ClientSocket : public Socket {
public:
    ClientSocket(std::string ipv4, int port);
    ClientSocket(socket_fd fdSock);

    ClientSocket(const ClientSocket& sock) = delete;
    ClientSocket(const ClientSocket&& sock) = delete;
    ClientSocket(ClientSocket& sock) = delete;
    ClientSocket(ClientSocket&& sock);

    ClientSocket& operator=(const ClientSocket& sock) = delete;
    ClientSocket& operator=(const ClientSocket&& sock) = delete;
    ClientSocket& operator=(ClientSocket sock) = delete;
    ClientSocket& operator=(ClientSocket& sock) = delete;

    ClientSocket&& operator=(ClientSocket&& sock) = delete;

    void write(const unsigned char* data, size_t len) const;
    template <class T>
    void write(const T& t) const;
    template <class T>
    void write(const std::list<T>& list) const;
    template <class T>
    void write(const std::vector<T>& vector) const;
    void write(const std::string& str) const;

    void read(unsigned char* data, size_t len) const;
    template <class T>
    void read(T& t) const;
    template <class T>
    void read(std::list<T>& list) const;
    template <class T>
    void read(std::vector<T>& vector) const;
    void read(std::string& str) const;

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

template <class T>
void ClientSocket::write(const T& t) const
{
    int len = sizeof(T);
    int uploadSize = 0;
    do {
        // server is able to detect client disconnecting
        if (!isConnected()) {
            std::cout << getHeader(mFdSock) << "write(const T& t) : client lost" << std::endl;
            throw Socket::exception("Client lost");
        }

        int byteSent = send(mFdSock, reinterpret_cast<const char*>(&t) + uploadSize, len - uploadSize, 0);
        if (byteSent == -1) {
            std::cout << getHeader(mFdSock) << "can't send packet " << byteSent << "/" << len << std::endl;
            perror("send error\n");
            throw Socket::exception("Can't write packet, peer connection lost");
        } else if (byteSent == 0) {
            std::cout << "byteSent == 0, sleep" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//            throw Socket::exception("0 byte send, peer connection lost");
        }
        uploadSize += byteSent;
#ifdef DEBUG_MSG
        std::cout << getHeader(mFdSock) << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" << std::endl;
#endif

    } while (len != uploadSize);

#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "write message ";
    for (int i = 0; i < len; ++i) {
        std::cout << (int)*((unsigned char*)&t + i) << " ";
    }
    if (std::is_same<T, Message>::value) {
        std::cout << ", " << message2string[(int)t];
    }
    if (std::is_same<T, Client::Type>::value) {
        std::cout << ", new client : " << Client::type2string[(int)t];
    }
    std::cout << std::endl;
#endif
}

template <class T>
void ClientSocket::write(const std::list<T>& list) const
{
    int nbEl = list.size();
    write(nbEl);

    for (const T& el : list) {
        write(el);
    }
}

template <class T>
void ClientSocket::write(const std::vector<T>& vector) const
{
    int nbEl = static_cast<int>(vector.size());
    write(nbEl);

    for (const T& el : vector) {
        write(el);
    }
}

///////////////////////////////////////////////////////////////////////////////

template <class T>
void ClientSocket::read(T& t) const
{

    int len = sizeof(T);
    int downloadSize = 0;
    do {

        int byteRead = recv(mFdSock, reinterpret_cast<char*>(&t) + downloadSize, len - downloadSize, 0);
        if (byteRead == -1) {
            std::cout << "byte read == -1 error" << std::endl;
            throw Socket::exception("Can't read packet, peer connection lost");

        } else if (byteRead == 0) {
            throw Socket::exception("0 byte received, peer connection lost");
        }

        downloadSize += byteRead;
#ifdef DEBUG_MSG
        std::cout << getHeader(mFdSock) << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" << std::endl;
#endif
    } while (len != downloadSize);

#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "read message ";
    for (int i = 0; i < len; ++i) {
        std::cout << (int)*((unsigned char*)&t + i) << " ";
    }
    if (std::is_same<T, Message>::value) {
        std::cout << ", " << message2string[(int)t];
    }
    if (std::is_same<T, Client::Type>::value) {
        std::cout << ", new client : " << Client::type2string[(int)t];
    }

    std::cout << std::endl;
#endif
}

template <class T>
void ClientSocket::read(std::list<T>& list) const
{
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        T el;
        read(el);
        list.push_back(std::move(el));
    }
}

template <class T>
void ClientSocket::read(std::vector<T>& vector) const
{
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        T el;
        read(el);
        vector.push_back(std::move(el));
    }
}

#endif // SOCKET_H
