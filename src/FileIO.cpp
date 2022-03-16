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
    auto streamSize = mFile.readsome((char*)data, len);
    if (streamSize == 0) {
        throw exception("End of file");
    }
    assert(streamSize == len);

    //#ifdef DEBUG_SOCKET
    //#endif

    //#ifdef DEBUG_SOCKET

    //#endif
}
