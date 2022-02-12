#include <iostream>
#include "UDPSender.h"

UDPSender::UDPSender(const std::string& ipAddress, int port) : _port{port} {
//    _address = QHostAddress(ipAddress.c_str());
}

void UDPSender::send(std::string message) {
    std::cout << "sent : " << message << std::endl;
//    _sock.writeDatagram(message.c_str(),message.length(),_address,_port);

}

void UDPSender::cleanup() {
//    _sock.close();
}

void UDPSender::send(TrackerData data) {
    std::string message = Utils::dataToMessage(data);
    send(message);
}


