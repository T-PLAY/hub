
#if defined WIN32
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library
using socklen_t = int;

#else
#define closesocket close
#include <arpa/inet.h>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <thread>


SOCKET sock;
void clearSocket()
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

#define PORT 4043

int main(void)
{
#if defined WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("error at WSASturtup\n");
        return 0;
    }
#else
    signal(SIGINT, signalHandler);
#endif

    // Socket creation
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        perror("socket creation failed.\n");
        clearSocket();
        return 0;
    }

    // Server address construction
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    //    address.sin_addr.s_addr = inet_addr("192.168.0.100"); // server IP
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // Server port

    // Create server
    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Failed to bind.\n");
        clearSocket();
        return 0;
    }

    if (listen(sock, 3) < 0) {
        perror("listen");
        //        exit(EXIT_FAILURE);
        clearSocket();
        return 0;
    }

//    constexpr size_t imgSize = 192 * 512;
//    unsigned char img[imgSize];
    SOCKET new_socket;
    while (true) {

        std::cout << "wait client on port " << PORT << std::endl;
        if ((new_socket = accept(sock, (struct sockaddr*)&address,
                 &addrlen))
            < 0) {
            perror("accept");
            //            exit(EXIT_FAILURE);
            clearSocket();
            return 0;
        }
        std::cout << "new client on socket " << new_socket << std::endl;


//        int dec = 0;
//        int byteSent = 1;
//        do {
//            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//            unsigned char a;

//            int byteRead = recv(new_socket, (char*)&a, 1, 0);
//            if (byteRead != 1) {
//                std::cout << "can't read sync byte " << byteRead << std::endl;
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
//                break;
//            }

//            std::cout << "sent " << byteSent << " bytes" << std::endl;

//        } while (byteSent > 0);
    }

    // Closing connection
    clearSocket();
    printf("\n");
    system("pause");
    return (0);
}
