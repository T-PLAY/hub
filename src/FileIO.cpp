#include "FileIO.h"

#include <cassert>

FileIO::FileIO(std::fstream& file)
    : mFile(file)
{
}

void FileIO::write(const unsigned char* data, size_t len) const
{
    mFile.write((const char*)data, len);
    //    int byteSent = send(mFdSock, data + uploadSize, static_cast<int>(len - uploadSize), 0);
}

///////////////////////////////////////////////////////////////////////////////

void FileIO::read(unsigned char* data, size_t len) const
{
    auto streamSize = mFile.readsome((char*)data, len);
    if (streamSize == 0) {
        throw exception("End of file");
    }
    assert(streamSize == len);
    //    size_t downloadSize = 0;
    //    do {
    //        int byteRead = recv(mFdSock, data + downloadSize, static_cast<int>(len - downloadSize), 0);
    //        if (byteRead == -1) {
    //            std::cout << "byte read == -1 error" << std::endl;
    //            throw Socket::exception("Can't read packet, peer connection lost");

    //        } else if (byteRead == 0) {
    //            throw Socket::exception("0 byte received, peer connection lost");
    //        }

    //        downloadSize += byteRead;
    //#ifdef DEBUG_SOCKET
    //        std::cout << getHeader(mFdSock) << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" << std::endl;
    //#endif
    //    } while (len != downloadSize);

    //#ifdef DEBUG_SOCKET
    //    std::cout << getHeader(mFdSock) << "read message ";

    //    for (size_t i = 0; i < len; ++i) {
    //        std::cout << (int)*(data + i) << " ";
    //    }

    //#endif
}
