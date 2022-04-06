#include "socket.h"

#include <cassert>

Socket::Socket()
{
#ifdef DEBUG_SOCKET
    std::cout << getHeader(mFdSock) << "Socket()" << std::endl;
#endif
    Net::init();
}

Socket::~Socket()
{
#ifdef DEBUG_SOCKET
    std::cout << getHeader(mFdSock) << "~Socket()" << std::endl;
#endif
    if (mFdSock != INVALID_SOCKET) {
        std::cout << getHeader(mFdSock) << "close socket" << std::endl;
        Net::clearSocket(mFdSock);
    }
}

bool Socket::isConnected() const
{
#ifdef WIN32
    // not sure how to check this in windows
    return true;
#else

    // TODO - make this better, because its probably wrong (but seems to work)

    int optval;
    socklen_t optlen = sizeof(optval);

    int res = getsockopt(mFdSock, SOL_SOCKET, SO_ERROR, &optval, &optlen);

    if (optval == 0 && res == 0)
        return true;
#endif

    return false;
}

ClientSocket::ClientSocket(ClientSocket::Type clientType, std::string sensorName, std::string syncSensorName, std::string ipv4, int port)
    : mIpv4(ipv4)
    , mPort(port)
{
#ifdef DEBUG_SOCKET
    std::cout << getHeader(mFdSock) << "ClientSocket(std::string ipv4, int port)" << std::endl;
#endif

    // Socket creation
    mFdSock = socket(PF_INET, SOCK_STREAM, 0);
    if (mFdSock < 0) {
        perror("[socket] socket creation failed.\n");
        return;
    }
    Net::sSockets.push_back(mFdSock);

    // Server address construction
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ipv4.c_str());
    serv_addr.sin_port = htons(port); // Server port

    // Connect to server
    while (connect(mFdSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        throw Socket::exception(((std::string("Failed to connect to server at address ") + ipv4 + " and port " + std::to_string(mPort))).c_str());
    }

    std::cout << getHeader(mFdSock) << "new client on socket " << mFdSock << std::endl;

    // ask server
    write(clientType);

    switch (clientType) {
    case Type::STREAM_VIEWER:
    {
        //        ClientSocket::Type clientType = ClientSocket::Type::STREAM_VIEWER;
        write(sensorName);
        Socket::Message mess;
        read(mess);
        if (mess == Socket::Message::NOT_FOUND) {
            throw Socket::exception((std::string("sensor '") + sensorName + "' is not attached to server").c_str());
        }
        assert(mess == Socket::Message::OK);

        write(syncSensorName);
        read(mess);
        if (mess == Socket::Message::NOT_FOUND) {
            throw Socket::exception((std::string("sync sensor '") + syncSensorName + "' is not attached to server").c_str());
        }
        assert(mess == Socket::Message::OK);
    } break;

    case Type::STREAMER: {
//        ClientSocket::Type clientType = ClientSocket::Type::STREAMER;
//        write(clientType);

        write(sensorName);
        Socket::Message mess;
        read(mess);
        if (mess == Socket::Message::FOUND) {
            throw Socket::exception((std::string("sensor '") + sensorName + "' is already attached to server").c_str());
        }
        assert(mess == Socket::Message::NOT_FOUND);

        write(sensorName);
    } break;

    case Type::VIEWER:
        break;

    default:
        assert(false);
    }
}

ClientSocket::ClientSocket(socket_fd fdSock)
{
    mFdSock = fdSock;
    std::cout << getHeader(mFdSock) << "ClientSocket(socket_fd fdSock)" << std::endl;
}

ClientSocket::ClientSocket(ClientSocket&& sock)
    : Socket()
    , mIpv4(sock.mIpv4)
    , mPort(sock.mPort)
{
    mFdSock = sock.mFdSock;
    sock.mFdSock = INVALID_SOCKET;

#ifdef DEBUG_SOCKET
    std::cout << getHeader(mFdSock) << "ClientSocket(ClientSocket && sock)" << std::endl;
#endif
}

ServerSocket::ServerSocket(int port)
    : mPort(port)
{
    // Socket creation
    mFdSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mFdSock < 0) {
        perror("socket creation failed.\n");
        return;
    }
    Net::sSockets.push_back(mFdSock);

    // Server address construction
    memset(&mAddress, 0, sizeof(mAddress));
    mAddress.sin_family = AF_INET;
    mAddress.sin_addr.s_addr = INADDR_ANY;
    mAddress.sin_port = htons(port); // Server port

    // Create server
    if (bind(mFdSock, (struct sockaddr*)&mAddress, sizeof(mAddress)) < 0) {
        perror("Failed to bind.\n");
        return;
    }

    if (listen(mFdSock, 3) < 0) {
        perror("listen");
        return;
    }
}
ClientSocket ServerSocket::waitNewClient()
{

    std::cout << getHeader(mFdSock) << "wait client on port " << mPort << std::endl;
    socklen_t addrlen = sizeof(mAddress);
    socket_fd new_socket = accept(mFdSock, (struct sockaddr*)&mAddress, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        perror("not accept new socket");
        Net::clearSocket(new_socket);
        exit(1);
    }
    std::cout << getHeader(mFdSock) << "new client on socket " << new_socket << std::endl;
    Net::sSockets.push_back(new_socket);

    return ClientSocket(new_socket);
}

void ClientSocket::write(const unsigned char* data, size_t len) const
{
    assert(len > 0);
    size_t uploadSize = 0;
    do {
        if (!isConnected()) {
            std::cout << getHeader(mFdSock) << "write(const unsigned char* data, size_t len) : isConnected() client lost" << std::endl;
            throw Socket::exception("Client lost");
        }
        // winsock const char * data
        // winsock int len
        int byteSent = send(mFdSock, (const char*)data + uploadSize, static_cast<int>(len - uploadSize), 0);
        if (byteSent == -1) {
            std::cout << getHeader(mFdSock) << "can't send packet " << byteSent << "/" << len << std::endl;
            //            perror("Failed to send.\n");
            throw Socket::exception("Can't write packet, peer connection lost");

        } else if (byteSent == 0) {
            std::cout << "byteSent == 0, sleep" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        uploadSize += byteSent;
#ifdef DEBUG_SOCKET
        std::cout << getHeader(mFdSock) << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" << std::endl;
#endif
    } while (len != uploadSize);

#ifdef DEBUG_SOCKET
    std::cout << getHeader(mFdSock) << "write message ";
    for (size_t i = 0; i < len; ++i) {
        std::cout << (int)*(data + i) << " ";
    }
#endif
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocket::read(unsigned char* data, size_t len) const
{
    size_t downloadSize = 0;
    do {
        int byteRead = recv(mFdSock, (char*)data + downloadSize, static_cast<int>(len - downloadSize), 0);
        if (byteRead == -1) {
            std::cout << "byte read == -1 error" << std::endl;
            throw Socket::exception("Can't read packet, peer connection lost");

        } else if (byteRead == 0) {
            throw Socket::exception("0 byte received, peer connection lost");
        }

        downloadSize += byteRead;
#ifdef DEBUG_SOCKET
        std::cout << getHeader(mFdSock) << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" << std::endl;
#endif
    } while (len != downloadSize);

#ifdef DEBUG_SOCKET
    std::cout << getHeader(mFdSock) << "read message ";

    for (size_t i = 0; i < len; ++i) {
        std::cout << (int)*(data + i) << " ";
    }

#endif
}

void ClientSocket::waitClose() const
{
    Socket::Message message;
    bool wantToClose = false;
    while (!wantToClose) {
        IOStream::read(message);

        switch (message) {
        case Message::CLOSE:
            wantToClose = true;
            break;

        default:
            std::cout << "waitClose, sleep" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    }

    IOStream::write(Message::OK);
}

void ClientSocket::clear()
{
    if (mFdSock != INVALID_SOCKET) {
        std::cout << getHeader(mFdSock) << "close socket" << std::endl;
        Net::clearSocket(mFdSock);
        mFdSock = INVALID_SOCKET;
    }
}
