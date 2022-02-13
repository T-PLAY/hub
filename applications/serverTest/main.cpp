// As seen on http://www.di.uniba.it/~reti/LabProRete/Interazione(TCP)Client-Server_Portabile.pdf

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

#define PORT 4043

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <thread>

void ClearWinSock()
{
#if defined WIN32
    WSACleanup();
#endif
}

void ErrorHandler(char* errorMessage)
{
    printf(errorMessage);
}

int server_fd;
void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

    // cleanup and close up stuff here
    // terminate program
    closesocket(server_fd);

    exit(signum);
}

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
    server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0) {
        ErrorHandler("socket creation failed.\n");
        closesocket(server_fd);
        ClearWinSock();
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
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        ErrorHandler("Failed to bind.\n");
        closesocket(server_fd);
        ClearWinSock();
        return 0;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        //        exit(EXIT_FAILURE);
        closesocket(server_fd);
        ClearWinSock();
        return 0;
    }

    constexpr size_t imgSize = 192 * 512;
    unsigned char img[imgSize];
    int new_socket;
    while (true) {

        std::cout << "wait client on port " << PORT << std::endl;
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                 &addrlen))
            < 0) {
            perror("accept");
            //            exit(EXIT_FAILURE);
            closesocket(server_fd);
            ClearWinSock();
            return 0;
        }
        std::cout << "new client on socket " << new_socket << std::endl;

        int dec = 0;

        int byteSent = 1;
        do {
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            unsigned char a;

            int byteRead = recv(new_socket, (char*)&a, 1, 0);
            if (byteRead != 1) {
                std::cout << "can't read sync byte " << byteRead << std::endl;
                //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //                byteRead = recv(new_socket, (char*)&a, 1, 0);
                //                closesocket(server_fd);
                //                ClearWinSock();
                //                return 4;
                break;
            }
            std::cout << "read a : " << (int)a << std::endl;

            // generate new image
            for (int i = 0; i < 512; ++i) {
                for (int j = 0; j < 192; ++j) {
                    img[i * 192 + j] = i + dec;
                }
            }
            ++dec;

            int byteSent = send(new_socket, (char*)img, imgSize, 0);
            if (byteSent != imgSize) {
                std::cout << "can't send sync byte " << byteSent << std::endl;
                //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //                byteSent = send(new_socket, (char*)img, imgSize, 0);
                //                closesocket(server_fd);
                //                ClearWinSock();
                //                return 3;
                break;
            }

            std::cout << "sent " << byteSent << " bytes" << std::endl;

        } while (byteSent > 0);
    }

    // Closing connection
    closesocket(server_fd);
    ClearWinSock();
    printf("\n");
    system("pause");
    return (0);
}
