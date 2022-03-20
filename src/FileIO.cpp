#include "FileIO.h"

#include <cassert>

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
    size_t downloadSize = 0;
    do {
        auto byteRead = mFile.readsome((char*)data + downloadSize, len - downloadSize);
        if (byteRead == 0) {
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
