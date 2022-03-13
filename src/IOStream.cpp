#include "IOStream.h"

void IOStream::write(const std::string& str) const
{
    int nbEl = static_cast<int>(str.size());
    write(nbEl);

    const char* data = str.data();
    write((const unsigned char*)data, nbEl);
}

void IOStream::read(std::string& str) const
{
    int nbEl;
    read(nbEl);

    unsigned char* tmp = new unsigned char[nbEl + 1];
    read(tmp, nbEl);
    tmp[nbEl] = 0;
    str = std::string((char*)tmp);
    delete[] tmp;
}
