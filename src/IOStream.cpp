#include "IOStream.h"

#include <iostream>

void IOStream::write(const std::string& str) const
{
    int strLen = static_cast<int>(str.size());
    write(strLen);

    if (strLen > 0) {
        const char* data = str.data();
        write((const unsigned char*)data, strLen);
    }
}

void IOStream::read(std::string& str) const
{
    int strLen = 0;
    read(strLen);

    if (strLen == 0) {
        str = "";

    } else {
        unsigned char* tmp = new unsigned char[strLen + 1];
        read(tmp, strLen);
        tmp[strLen] = 0;
        str = std::string((char*)tmp);
        delete[] tmp;
    }
}

void IOStream::setupOutput(const std::string& sensorName) const
{
    (void)sensorName;
#ifdef DEBUG_IOSTREAM
    std::cout << "IOStream::setOutputName(const std::string& sensorName)" << std::endl;
#endif
    ////        ClientSocket::Type clientType = ClientSocket::Type::STREAMER;
    ////        write(clientType);

    //        write(sensorName);
    //        Socket::Message mess;
    //        read(mess);
    //        if (mess == Socket::Message::FOUND) {
    //            throw Socket::exception((std::string("sensor '") + sensorName + "' is already attached to server").c_str());
    //        }
    //        assert(mess == Socket::Message::NOT_FOUND);

    assert(mMode == Mode::INPUT_OUTPUT);
//    mMode = Mode::OUTPUT;
}

// IOStream::IOStream(const Mode& mode)
//     : mMode(mode)
//{
// }

// void IOStream::setupMode(const Mode& mode)
//{
#ifdef DEBUG_IOSTREAM
//     std::cout << "IOStream::setupMode(const Mode& mode)" << std::endl;
#endif
//     mMode = mode;
// }
