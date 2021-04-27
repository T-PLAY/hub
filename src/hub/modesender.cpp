#include "modesender.h"

#include <utility>
#include <iostream>

ModeSender::ModeSender(uint mode, std::string ip) : _mode{mode}, _ip{std::move(ip)}{
    _sock = socket(AF_INET, SOCK_STREAM, 0);

    _addr.sin_family = AF_INET;
    inet_pton(AF_INET, _ip.c_str(), &_addr.sin_addr);
    _addr.sin_port = htons(4042);
}

bool ModeSender::sendMode() {
    if (!connect(_sock, reinterpret_cast<const struct sockaddr *>(&_addr), _addrLen)) {
        std::cout << _mode << std::endl;
        send(_sock, &_mode, sizeof(_mode), 0);
        return true;
    }
    return false;
}
