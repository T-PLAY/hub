#include "FileIO.h"

#include <cassert>
#include <iostream>

FileIO::FileIO(std::fstream& file)
    : mFile(file)
{
}

void FileIO::write(const unsigned char* data, size_t len) const
{
    mFile.write((const char*)data, len);
}

///////////////////////////////////////////////////////////////////////////////

void FileIO::read(unsigned char* data, size_t len) const
{
//    std::cout << "[FileIO] read(char* data, size_t len)" << std::endl;
    size_t downloadSize = 0;
    do {
//        auto byteRead = mFile.readsome((char*)data + downloadSize, len - downloadSize);
//        mFile.read((char*)data + downloadSize, len - downloadSize);

        auto before = mFile.tellg();
        mFile.read(reinterpret_cast<char*>(data + downloadSize), len - downloadSize);
        auto after = mFile.tellg();
        auto byteRead = after - before;
//        assert(after - before == len);
        // std::cout << "[FileIO] byteRead = " << byteRead << std::endl;
        if (byteRead <= 0) {
            throw exception("End of file");
        }


        downloadSize += byteRead;
        //        assert(streamSize == len);
    } while (len != downloadSize);

    //#ifdef DEBUG_SOCKET
    //#endif

    //#ifdef DEBUG_SOCKET

    //#endif
}
