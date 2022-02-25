#ifndef SOCKET_H
#define SOCKET_H

#include <net.h>

#include <exception>
#include <functional>
#include <list>
#include <string>

#define SERVICE_PORT 4041

//#define DEBUG_MSG

class socket_error : public std::runtime_error {
    //    const char*mMsg;
    //    const char* mFile;
    //    int mLine;
    //    const char* mFunc;
    //    const char* mInfo;

public:
    socket_error(const char* const message) throw()
        : std::runtime_error(message)
    {
    }

    //    socket_error(const char* msg, const char* file_, int line_, const char* func_, const char* info_ = "")
    ////        : std::exception(msg)
    //        : mMsg(msg)
    //        , mFile(file_)
    //        , mLine(line_)
    //        , mFunc(func_)
    //        , mInfo(info_)
    //    {
    //    }
    const char* what() const throw()
    {
        //        return std::exception::what();
        return std::runtime_error::what();
        //        return (std::string("Socket exception : ") + mMsg + mFile + mLine + mFunc + mInfo).c_str();
    }
};

static std::string getHeader(socket_fd iSock)
{
    std::string str = "\033[" + std::to_string(31 + iSock % 7) + "m[socket:" + std::to_string(iSock) + "]\033[0m ";
    return str;
}

class Client {
public:
    enum class Type {
        NONE,
        STREAMER,
        VIEWER,
        STREAM_VIEWER,
        COUNT
    };

    enum class Message {
        NONE,
        PING,
        GET_CONNECTED_DEVICES,
        DATA,
        CLOSE,
        COUNT,
    };
};

class Socket {
public:
    //    Socket(int port); // server socket
    //    Socket(socket_fd fdSock);
    bool isConnected() const;
protected:
    Socket();
    //    Socket(Socket & socket);
    //    Socket(Socket &socket);
    //    Socket(Socket&& socket);
    //    Socket& operator=(Socket socket);

    ~Socket();


    //    template <class T>
    //    void sendData(const T & t);

protected:
    socket_fd mFdSock = -1;
    //    std::string mIpv4;
};

class ClientSocket : public Socket {
public:
    ClientSocket(std::string ipv4, int port = SERVICE_PORT);
    ClientSocket(socket_fd fdSock);
//    ~ClientSocket();

    ClientSocket(const ClientSocket& sock) = delete;
    ClientSocket(const ClientSocket&& sock) = delete;
    ClientSocket(ClientSocket& sock) = delete;
    ClientSocket(ClientSocket&& sock);
    //    ClientSocket(const ClientSocket& sock);
    ClientSocket& operator=(const ClientSocket& sock) = delete;
    ClientSocket& operator=(const ClientSocket&& sock) = delete;
    ClientSocket& operator=(ClientSocket sock) = delete;
    ClientSocket& operator=(ClientSocket& sock) = delete;

    ClientSocket&& operator=(ClientSocket&& sock) = delete;

    void write(const unsigned char* data, size_t len) const;
    template <class T>
    void write(const T& data) const;
    template <class T>
    void write(const std::list<T>& data) const;

    void read(unsigned char* data, size_t len) const;
    template <class T>
    void read(T& data) const;
    template <class T>
    void read(std::list<T>& data) const;

private:
    std::string mIpv4;
    int mPort;
};

class ServerSocket : public Socket {
public:
    ServerSocket(int port = SERVICE_PORT);

    ClientSocket waitNewClient();

private:
    int mPort;
    //    bool mOpenned;
    struct sockaddr_in mAddress;
};

template <class T>
void ClientSocket::write(const T& data) const
{
    int len = sizeof(T);
    int uploadSize = 0;
    do {
        // server is able to detect client disconnecting
        if (!isConnected()) {
            std::cout << getHeader(mFdSock) << "client lost" << std::endl;
            throw socket_error("Client lost");
            //            return;
            std::cout << "AFTER throw" << std::endl;
        }

        int byteSent = send(mFdSock, reinterpret_cast<const char*>(&data), len, 0);
        if (byteSent == -1) {
            std::cout << getHeader(mFdSock) << "can't send packet " << byteSent << "/" << len << std::endl;
            perror("send error\n");
            //        exit(5);
            throw socket_error("Can't write packet, peer connection lost");
            return;
        } else if (byteSent == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        uploadSize += byteSent;
#ifdef DEBUG_MSG
        std::cout << getHeader(mFdSock) << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" << std::endl;
#endif

    } while (len != uploadSize);

#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "write message ";
    for (int i = 0; i < len; ++i) {
        std::cout << (int)*((unsigned char*)&data + i) << " ";
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
void ClientSocket::read(T& data) const
{
    //    std::cout << "ClientSocket::read(T& data)" << std::endl;

    int len = sizeof(T);
    //    std::cout << getHeader(mFdSock) << "ClientSocket reading " << len << " bytes" << std::endl;
    int downloadSize = 0;
    do {

        //        if (!isConnected())
        //            return;

        //        char buffer[32];
        //        if (recv(mFdSock, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
        //            std::cout << getHeader(mFdSock) << "server not connected" << std::endl;
        //        }

        //        ClientSocket::write(Client::Message::PING); // check peer connection
        //        std::cout << "start recv" << std::endl;
        int byteRead = recv(mFdSock, reinterpret_cast<char*>(&data) + downloadSize, len - downloadSize, 0);
        //        std::cout << "end recv" << std::endl;
        if (byteRead == -1) {
            //            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "byte read == -1 error" << std::endl;
            throw socket_error("Can't read packet, peer connection lost");
            exit(5);
            return;
        } else if (byteRead == 0) {
            throw socket_error("0 byte received, peer connection lost");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        //        else {

        //        }
        downloadSize += byteRead;
#ifdef DEBUG_MSG
        std::cout << getHeader(mFdSock) << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" << std::endl;
#endif
    } while (len != downloadSize);

#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "read message ";
    for (int i = 0; i < len; ++i) {
        std::cout << (int)*((unsigned char*)&data + i) << " ";
    }
    std::cout << std::endl;
#endif
}

template <class T>
void ClientSocket::read(std::list<T>& list) const
{
    //    std::cout << "ClientSocket::read(std::list<T> & list)" << std::endl;
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        //    for (const T& el : list) {
        T el;
        read(el);
        list.push_back(std::move(el));
    }
}

#endif // SOCKET_H
