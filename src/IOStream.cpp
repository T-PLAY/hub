#include "IOStream.h"

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
    int strLen;
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
