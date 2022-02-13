#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 4043
#include <iostream>
#include <thread>

int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    //    char buffer[1024] = { 0 };
    //    char* hello = "Hello from server";

    // Creating socket file descriptor
//    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
            sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "wait client on port " << PORT << std::endl;
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    constexpr size_t imgSize = 192 * 512;
    //    constexpr size_t imgSize = 100;
    unsigned char img[imgSize];
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                 (socklen_t*)&addrlen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::cout << "new client on socket " << new_socket << std::endl;

        //    valread = read(new_socket, buffer, 1024);
        //    printf("%s\n", buffer);
        int dec = 0;

        //    for (int i =0; i <imgSize; ++i) {
        //        img[i] = i * 255 / 100;
        //    }
        int byteSent = 1;
        do {
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            unsigned char a;

            int byteRead = recv(new_socket, &a, 1, 0);
            while (byteRead != 1) {
                std::cout << "can't read sync byte " << byteRead  << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                byteRead = recv(new_socket, &a, 1, 0);
                return 4;
            }
//            if (byteRead != 1) {
//                std::cout << "can't read sync byte " << byteRead  << std::endl;
//                return -1;
//            }
            std::cout << "read a : " << (int)a << std::endl;

            for (int i = 0; i < 512; ++i) {
                for (int j = 0; j < 192; ++j) {
                    img[i * 192 + j] = i + dec;
                }
            }
            ++dec;

//            int byteSent = send(new_socket, (char*)&a, 1, 0);
            int byteSent = send(new_socket, (char*)img, imgSize, 0);
//            int byteSent = write(new_socket, (char*)&a, 1);
            while (byteSent != imgSize) {
                std::cout << "can't send sync byte " << byteSent  << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
//                byteSent = send(new_socket, (char*)&a, 1, 0);
                byteSent = send(new_socket, (char*)img, imgSize, 0);
//                byteSent = write(new_socket, (char*)&a, 1);
                return 3;
            }
//            std::cout << "sent a : " << (int)a << std::endl;

////            try {
////            byteSent = send(new_socket, img, imgSize, 0);
//            byteSent = write(new_socket, img, imgSize);
//            if (byteSent != imgSize) {
//                std::cout << "error sent bad size" << std::endl;
//                return 3;
//            }
////            }  catch (std::exception e) {
////                std::cout << e.what() << std::endl;
////                return -2;
////            }
            std::cout << "sent " << byteSent << " bytes" << std::endl;
////            std::this_thread::sleep_for(std::chrono::milliseconds(100));
////            if (dec > 100) {
////                std::this_thread::sleep_for(std::chrono::milliseconds(1'000));
////            }
//            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
//            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//            double fps = 1'000 / std::max((double)duration, 0.0001);
//            std::cout << "fps : " << fps << std::endl;
////            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        } while (byteSent > 0);
    }
    return 0;
}
