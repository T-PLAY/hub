#include <iostream>
#include "UDPSender.h"

UDPSender::UDPSender(const std::string& ipAddress, int port) {
#ifdef _WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    InetPton(AF_INET, (PCWSTR)ipAddress.c_str(), &_addr.sin_addr.s_addr);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _sock = socket(AF_INET,SOCK_DGRAM,0);
#elif __linux__
    _sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &_addr, 0, sizeof(_addr));

    inet_aton(ipAddress.c_str() , &_addr.sin_addr);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
#endif
}

void UDPSender::send(std::string message) {
    std::cout << "sent : " << message << std::endl;
#ifdef _WIN32
    sendto(_sock, message.c_str(), message.length(), 0, (SOCKADDR*)&_addr, sizeof(_addr));
#elif __linux__
    sendto(_sock, message.c_str(), message.length(), 0, reinterpret_cast<const sockaddr *>(&_addr), sizeof(_addr));
#endif
}

void UDPSender::cleanup() {
#ifdef _WIN32
    closesocket(_sock);
    WSACleanup();
#elif __linux__
    close(_sock);
#endif

}

void UDPSender::send(TrackerData data) {
    std::string message = Utils::dataToMessage(data);
    send(message);
}


