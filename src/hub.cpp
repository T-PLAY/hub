#include "hub.h"


#include <cstring>
#include <iostream>
#include <stdio.h>
#include <thread>

void clearSocket(SOCKET sock)
{
    closesocket(sock);
#ifdef WIN32
    WSACleanup();
#endif
}

#ifndef WIN32
void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

    // cleanup and close up stuff here
    // terminate program
    clearSocket();

    exit(signum);
}
#endif

Hub::Hub(std::string ipv4, int port)
    : mIpv4(ipv4)
    , mPort(port)
{
#if defined WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("error at WSAStartup\n");
        exit(1);
    }
#else
    signal(SIGINT, signalHandler);
#endif

    // Socket creation
    mSock = socket(PF_INET, SOCK_STREAM, 0);
    if (mSock < 0) {
        perror("socket creation failed.\n");
        clearSocket(mSock);
        exit(2);
    }

    // Server address construction
    struct sockaddr_in serv_addr;
    //    socklen_t addrlen = sizeof(serv_addr);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.100"); // server IP
    serv_addr.sin_addr.s_addr = inet_addr(mIpv4.c_str());
    serv_addr.sin_port = htons(mPort); // Server port

    //    if(inet_pton(AF_INET, mIpv4, &serv_addr.sin_addr)<=0)
    //    {
    //        printf("\nInvalid address/ Address not supported \n");
    //        return -1;
    //    }

    // Connect to server
    if (connect(mSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Failed to connect to address:" << mIpv4 << " port:" << mPort << std::endl;
        clearSocket(mSock);
        exit(3);
    }

    //    if (listen(mSock, 3) < 0) {
    //        perror("listen");
    //        //        exit(EXIT_FAILURE);
    //        clearSocket(mSock);
    //        exit(4);
    //    }

    //    constexpr size_t imgSize = 192 * 512;
    //    unsigned char img[imgSize];
    //    int new_socket;
    //    while (true) {

    //        std::cout << "wait client on port " << mPort << std::endl;
    //        if ((new_socket = accept(mSock, (struct sockaddr*)&serv_addr,
    //                 &addrlen))
    //            < 0) {
    //            perror("accept");
    //            //            exit(EXIT_FAILURE);
    //            clearSocket(mSock);
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
    //                //                clearSocket(mSock);
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
    //                //                clearSocket(mSock);
    //                //                return 3;
    //                break;
    //            }

    //            std::cout << "sent " << byteSent << " bytes" << std::endl;

    //        } while (byteSent > 0);
    //    }
}

Hub::~Hub()
{
    // Closing connection
    clearSocket(mSock);
    printf("\n");
    system("pause");
}

template <class T>
void Hub::sendPacket(const T & t)
{
    int byteSent = send(mSock, (char*)&t, sizeof(T), 0);
    if (byteSent != sizeof(T)) {
        std::cout << "can't send packet " << byteSent << "/" << sizeof(T) << std::endl;
        //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //                byteSent = send(new_socket, (char*)img, imgSize, 0);
        //                clearSocket(mSock);
        //                return 3;
        exit(5);
    }
}
