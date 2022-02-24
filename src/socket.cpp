#include "socket.h"

//#include <net.h>

Socket::Socket()
{
//    std::cout << getHeader(mFdSock) << "Socket()" << std::endl;
    Net::init();
}

//Socket::Socket(socket_fd fdSock)
//    : mFdSock(fdSock)
//{
//    std::cout << getHeader(mFdSock) << "Socket(socket_fd fdSock)" << std::endl;
//    Net::init();
//    //    Net::sSockets.insert(mFdSock);
//}

//// Socket::Socket(Socket& socket)
////     : Socket(socket.mFdSock)
////{
//// }

//// Socket::Socket(Socket& socket)
////     : Socket(socket.mFdSock)
////{
//// }

//Socket::Socket(Socket&& socket)
//    : Socket(socket.mFdSock)
//{
//    std::cout << getHeader(mFdSock) << "Socket(Socket&& fdSock)" << std::endl;
//    socket.mFdSock = -1;
//}

// Socket& Socket::operator=(Socket socket)
//{
//     mFdSock = socket.mFdSock;
//     return *this;
// }


Socket::~Socket()
{
//    std::cout << getHeader(mFdSock) << "~Socket()" << std::endl;
    if (mFdSock != -1) {
        //        if (mOpenned) {
        // Closing connection
        std::cout << getHeader(mFdSock) << "close socket" << std::endl;
        Net::clearSocket(mFdSock);
    }
    //        }
    //    printf("\n");
    //    system("pause");
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

//    std::cout << getHeader(mFdSock) << "loose connection" << std::endl;
//    exit(9);
    return false;

    //        int error = 0;
    //        socklen_t len = sizeof (error);
    //        int retval = getsockopt (mFdSock, SOL_SOCKET, SO_ERROR, &error, &len);
    //        if (retval != 0) {
    //            /* there was a problem getting the error code */
    //            fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
    //            return;
    //        }
    //        if (error != 0) {
    //            /* socket has a non zero error status */
    //            fprintf(stderr, "socket error: %s\n", strerror(error));
    //        }
}

ClientSocket::ClientSocket(std::string ipv4, int port)
    : mIpv4(ipv4)
    , mPort(port)
{
//    std::cout << getHeader(mFdSock) << "ClientSocket(std::string ipv4, int port)" << std::endl;

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
        std::cout << getHeader(mFdSock) << "Failed to connect to address:" << ipv4 << " port:" << port << std::endl;
        //        perror("Failed to connect.\n");
        //        exit(1);
        //        throw socket_error(((std::string("Failed to connect to server at address ") + ipv4 + std::string(" and port ") + std::to_string(mPort))).c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    //    if (listen(mFdSock, 3) < 0) {
    //        perror("listen");
    //        //        exit(EXIT_FAILURE);
    //        Net::clearSocket(mFdSock);
    //        exit(4);
    //    }

    //    constexpr size_t imgSize = 192 * 512;
    //    unsigned char img[imgSize];
    //    int new_socket;
    //    while (true) {

    //        std::cout << "wait client on port " << port << std::endl;
    //        if ((new_socket = accept(mFdSock, (struct sockaddr*)&serv_addr,
    //                 &addrlen))
    //            < 0) {
    //            perror("accept");
    //            //            exit(EXIT_FAILURE);
    //            Net::clearSocket(mFdSock);
    //            exit(5);
    //        }
    //        std::cout << "new client on socket " << new_socket << std::endl;

    //        int dec = 0;

    //        int byteSent = 1;
    //        do {
    //            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    //            unsigned char a;

    //            int byteRead = recv(new_socket, (char*)&a, 1, 0);
    //            if (byteRead != 1) {
    //                std::cout << "can't read sync byte " << byteRead << std::endl;
    //                //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //                //                byteRead = recv(new_socket, (char*)&a, 1, 0);
    //                //                Net::clearSocket(mFdSock);
    //                //                return 4;
    //                break;
    //            }
    //            std::cout << "read a : " << (int)a << std::endl;

    //            // generate new image
    //            for (int i = 0; i < 512; ++i) {
    //                for (int j = 0; j < 192; ++j) {
    //                    img[i * 192 + j] = i + dec;
    //                }
    //            }
    //            ++dec;

    //            int byteSent = send(new_socket, (char*)img, imgSize, 0);
    //            if (byteSent != imgSize) {
    //                std::cout << "can't send sync byte " << byteSent << std::endl;
    //                //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //                //                byteSent = send(new_socket, (char*)img, imgSize, 0);
    //                //                Net::clearSocket(mFdSock);
    //                //                return 3;
    //                break;
    //            }

    //            std::cout << "sent " << byteSent << " bytes" << std::endl;

    //        } while (byteSent > 0);
    //    }
}

ClientSocket::ClientSocket(socket_fd fdSock)
//    : Socket(fdSock)
{
    mFdSock = fdSock;
    std::cout << getHeader(mFdSock) << "ClientSocket(socket_fd fdSock)" << std::endl;
}

//ClientSocket::~ClientSocket()
//{

//}

ClientSocket::ClientSocket(ClientSocket&& sock)
    : Socket()
    , mIpv4(sock.mIpv4)
    , mPort(sock.mPort)
{
    mFdSock = sock.mFdSock;
    sock.mFdSock = -1;
//    std::cout << getHeader(mFdSock) << "ClientSocket(ClientSocket&& sock)" << std::endl;
}

//ClientSocket::ClientSocket(const ClientSocket& sock)
//    : Socket()
//{
//    mFdSock = sock.mFdSock;
//    std::cout << getHeader(mFdSock) << "ClientSocket(ClientSocket& fdSock)" << std::endl;
//}

ServerSocket::ServerSocket(int port)
    : mPort(port)
{
//    std::cout << getHeader(mFdSock) << "ServerSocket(int port)" << std::endl;
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

    socket_fd new_socket;
    std::cout << getHeader(mFdSock) << "wait client on port " << mPort << std::endl;
    socklen_t addrlen = sizeof(mAddress);
    if ((new_socket = accept(mFdSock, (struct sockaddr*)&mAddress,
             &addrlen))
        < 0) {
        perror("accept");
        //            exit(EXIT_FAILURE);
        Net::clearSocket(new_socket);
        exit(1);
    }
    std::cout << getHeader(mFdSock) << "new client on socket " << new_socket << std::endl;

    return ClientSocket(new_socket);
}

// template <class T>
// void Socket::sendData(const T& t)
//{
//     int byteSent = send(mFdSock, (char*)&t, sizeof(T), 0);
//     if (byteSent != sizeof(T)) {
//         std::cout << "can't send packet " << byteSent << "/" << sizeof(T) << std::endl;
//         //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         //                byteSent = send(new_socket, (char*)img, imgSize, 0);
//         //                Net::clearSocket(mFdSock);
//         //                return 3;
//         exit(5);
//     }
// }

void ClientSocket::write(const unsigned char* data, size_t len) const
{
    size_t uploadSize = 0;
    do {
        int byteSent = send(mFdSock, data, len, 0);
        if (byteSent == -1) {
            std::cout << getHeader(mFdSock) << "can't send packet " << byteSent << "/" << len << std::endl;
            perror("Failed to send.\n");
            //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //                byteSent = send(new_socket, (char*)img, imgSize, 0);
            //                Net::clearSocket(mFdSock);
            //                return 3;
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
}

void ClientSocket::read(unsigned char* data, size_t len) const
{
//    std::cout << "ClientSocket::read(char* data, size_t len)" << std::endl;
//    ClientSocket::write(Client::Message::PING); // check peer connection
    size_t downloadSize = 0;
    do {
        int byteRead = recv(mFdSock, data, len, 0);
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
