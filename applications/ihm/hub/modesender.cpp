#include "modesender.h"

#include <utility>
#include <iostream>

ModeSender::ModeSender(uint mode, std::string  ip) : _mode{mode}, _address{std::move(ip)}{

}

bool ModeSender::sendMode() {
    _sock.connectToHost(_address.c_str(),4042);
    if(_sock.waitForConnected(10000)) {
        _sock.write((const char *)&_mode,sizeof(_mode));
        _sock.waitForBytesWritten(1000);

        return true;
    }

    return false;
}



