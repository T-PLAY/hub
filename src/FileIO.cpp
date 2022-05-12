#include "FileIO.h"

#include <cassert>
#include <iostream>

//FileIO::FileIO(std::fstream &&file)
//    : mFile(std::move(file))
//{
//    assert(file.is_open());
//}

// FileIO::FileIO(std::fstream &file)
//{

//}
FileIO::FileIO(std::fstream &&file)
    : mFile(*std::move(new std::fstream(std::move(file))))
{
    assert(mFile.is_open());
}

void FileIO::close()
{
    mFile.close();
}

void FileIO::write(const unsigned char* data, size_t len) const
{
    mFile.write((const char*)data, len);
}

///////////////////////////////////////////////////////////////////////////////

void FileIO::read(unsigned char* data, size_t len) const
{
    size_t downloadSize = 0;
    do {

        auto before = mFile.tellg();
        mFile.read(reinterpret_cast<char*>(data + downloadSize), len - downloadSize);
        auto after = mFile.tellg();
        auto byteRead = after - before;
        // std::cout << "[FileIO] byteRead = " << byteRead << std::endl;
        if (byteRead <= 0) {
            throw FileIO::exception("End of file");
        }

        downloadSize += byteRead;
    } while (len != downloadSize);

    //#ifdef DEBUG_SOCKET
    //#endif

    //#ifdef DEBUG_SOCKET

    //#endif
}
