#ifndef SOCKET_H
#define SOCKET_H

#include <net.h>

#include <functional>
#include <list>
#include <string>

static std::string getHeader(int iSock)
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
        GET_CONNECTED_DEVICES,
        COUNT,
    };
};

class Socket {
public:
    //    Socket(int port); // server socket
    Socket(socket_fd fdSock);
    Socket(Socket&& socket);
    ~Socket();

    bool isConnected() const;

    //    template <class T>
    //    void sendData(const T & t);

protected:
    socket_fd mFdSock;
    //    std::string mIpv4;
};

class ClientSocket : public Socket {
public:
    ClientSocket(std::string ipv4, int port);
    ClientSocket(socket_fd fdSock);

    void write(const char* data, size_t len) const;
    template <class T>
    void write(const T& data) const;
    template <class T>
    void write(const std::list<T>& data) const;

    void read(char* data, size_t len) const;
    template <class T>
    void read(T& data) const;
    template <class T>
    void read(std::list<T>& data) const;

private:
    std::string mIpv4;
    int mPort;
};

template <class T>
void ClientSocket::write(const T& data) const
{
    int len = sizeof(T);
    int byteSent = send(mFdSock, reinterpret_cast<const char*>(&data), len, 0);
    if (byteSent != len) {
        std::cout << getHeader(mFdSock) << "can't send packet " << byteSent << "/" << len << std::endl;
        perror("send error\n");
        exit(5);
    }
    std::cout << getHeader(mFdSock) << "byteSent = " << byteSent << std::endl;
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
    int len = sizeof(T);
    std::cout << getHeader(mFdSock) << "ClientSocket reading " << len << " bytes" << std::endl;
    int downloadSize = 0;
    do {

        if (!isConnected())
            return;

        int byteRead = recv(mFdSock, reinterpret_cast<char*>(&data) + downloadSize, len - downloadSize, 0);
        if (byteRead == -1) {
            //            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        } else if (byteRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        //        else {
        std::cout << getHeader(mFdSock) << "byteRead = " << byteRead << std::endl;
        //        }
        downloadSize += byteRead;
    } while (len != downloadSize);

    //    std::cout << getHeader(mFdSock) << "read message ";
    //    for (int i = 0; i < len; ++i) {
    //        std::cout << (int)*((unsigned char*)&data + i) << " ";
    //    }
    //    std::cout << std::endl;
}

template <class T>
void ClientSocket::read(std::list<T>& list) const
{
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        //    for (const T& el : list) {
        T el;
        read(el);
        list.push_back(std::move(el));
    }
}

class ServerSocket : public Socket {
public:
    ServerSocket(int port);

    ClientSocket waitNewClient();

private:
    int mPort;
    //    bool mOpenned;
    struct sockaddr_in mAddress;
};

#endif // SOCKET_H
