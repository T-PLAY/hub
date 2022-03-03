#include "socket.h"

#include <cassert>

Socket::Socket()
{
#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "Socket()" << std::endl;
#endif
    Net::init();
}

Socket::~Socket()
{
#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "~Socket()" << std::endl;
#endif
    if (mFdSock != -1) {
        std::cout << getHeader(mFdSock) << "close socket" << std::endl;
        Net::clearSocket(mFdSock);
        mFdSock = -1;
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

ClientSocket::ClientSocket(std::string ipv4, int port)
    : mIpv4(ipv4)
    , mPort(port)
{
#ifdef DEBUG_MSG
    std::cout << getHeader(mFdSock) << "ClientSocket(std::string ipv4, int port)" << std::endl;
#endif

    // Socket creation
    mFdSock = socket(PF_INET, SOCK_STREAM, 0);
    if (mFdSock < 0) {
        perror("[socket] socket creation failed.\n");
        return;
    }
    Net::sSockets.insert(mFdSock);

    // Server address construction
    struct sockaddr_in serv_addr;
    //    socklen_t addrlen = sizeof(serv_addr);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.100"); // server IP
    serv_addr.sin_addr.s_addr = inet_addr(ipv4.c_str());
    serv_addr.sin_port = htons(port); // Server port

    //    if(inet_pton(AF_INET, ipv4, &serv_addr.sin_addr)<=0)
    //    {
    //        printf("\nInvalid address/ Address not supported \n");
    //        return -1;
    //    }

    // Connect to server
    while (connect(mFdSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        throw Socket::exception(((std::string("Failed to connect to server at address ") + ipv4 + std::string(" and port ") + std::to_string(mPort))).c_str());
    }

    std::cout << getHeader(mFdSock) << "new client on socket " << mFdSock << std::endl;

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
    sock.mFdSock = -1;

#ifdef DEBUG_MSG
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
    Net::sSockets.insert(mFdSock);

    // Server address construction
    //    struct sockaddr_in address;
    //    socklen_t addrlen = sizeof(address);
    memset(&mAddress, 0, sizeof(mAddress));
    mAddress.sin_family = AF_INET;
    //    mAddress.sin_addr.s_addr = inet_addr("192.168.0.100"); // server IP
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
        //    if ((new_socket = accept(mFdSock, (struct sockaddr*)&mAddress,
        //             &addrlen))
        //        < 0) {
        //        == INVALID_SOCKET) {
        perror("not accept new socket");
        //            exit(EXIT_FAILURE);
        Net::clearSocket(new_socket);
        exit(1);
    }
    std::cout << getHeader(mFdSock) << "new client on socket " << new_socket << std::endl;
    Net::sSockets.insert(new_socket);

    return ClientSocket(new_socket);
}

void ClientSocket::write(const unsigned char* data, size_t len) const
{
    size_t uploadSize = 0;
    do {
        // winsock const char * data
        //        int byteSent = send(mFdSock, static_cast<const char*>(data), len, 0);
        // winsock int len
        int byteSent = send(mFdSock, reinterpret_cast<const char*>(data) + uploadSize, static_cast<int>(len - uploadSize), 0);
        if (byteSent == -1) {
            std::cout << getHeader(mFdSock) << "can't send packet " << byteSent << "/" << len << std::endl;
            perror("Failed to send.\n");
            throw Socket::exception("Can't write packet, peer connection lost");

        } else if (byteSent == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        uploadSize += byteSent;
#ifdef DEBUG_MSG
        std::cout << getHeader(mFdSock) << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" << std::endl;
#endif
    } while (len != uploadSize);
}

void ClientSocket::write(const std::string& str) const
{
    int nbEl = str.size();
    write(nbEl);

    const char* data = str.data();
    write((unsigned char*)data, nbEl);
}

///////////////////////////////////////////////////////////////////////////////


void ClientSocket::read(unsigned char* data, size_t len) const
{
    size_t downloadSize = 0;
    do {
        int byteRead = recv(mFdSock, reinterpret_cast<char*>(data) + downloadSize, static_cast<int>(len - downloadSize), 0);
        if (byteRead == -1) {
            std::cout << getHeader(mFdSock) << "can't read packet " << byteRead << "/" << len << std::endl;
            perror("Failed to read.\n");
            exit(5);
        } else if (byteRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        downloadSize += byteRead;
#ifdef DEBUG_MSG
        std::cout << getHeader(mFdSock) << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" << std::endl;
#endif
    } while (len != downloadSize);
}

void ClientSocket::read(std::string& str) const
{
    int nbEl;
    read(nbEl);

    unsigned char tmp[nbEl + 1];
    read(tmp, nbEl);
    tmp[nbEl] = 0;
    str = std::string((char*)tmp);
}

void ClientSocket::waitClose() const
{
    Socket::Message message;
    bool wantToClose = false;
    while (!wantToClose) {
        read(message);

        switch (message) {
        case Message::CLOSE:
            wantToClose = true;
            break;

        default:
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    }

    write(Message::OK);
}

void ClientSocket::clear()
{
    if (mFdSock != -1) {
        std::cout << getHeader(mFdSock) << "close socket" << std::endl;
        Net::clearSocket(mFdSock);
        mFdSock = -1;
    }
}
