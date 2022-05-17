#include "RamIO.h"

#include <cassert>
#include <iostream>

//#include <memory>
#include <cstring>
#include <thread>

// RamIO::RamIO(std::fstream &&file)
//     : mFile(std::move(file))
//{
//     assert(file.is_open());
// }

// RamIO::RamIO(std::fstream &file)
//{

//}
// RamIO::RamIO()
////    : m_buff(buff)
////    , m_len(len)
//{
//}

// RamIO::RamIO(std::fstream&& file)
//     : mFile(*std::move(new std::fstream(std::move(file))))
//{
//     assert(mFile.is_open());
// }

void CyclicBuff::write(const unsigned char* data, size_t len)
{
    size_t uploadSize = 0;
    do {

        auto byteWrote = m_readHead - m_writeHead;
        if (byteWrote <= 0)
            byteWrote += m_bufLen;
        if (len > byteWrote) {
            throw CyclicBuff::exception("Buffer overflow");
        }
        byteWrote = std::min(byteWrote, len - uploadSize);

        memcpy(&m_buff[m_writeHead], data + uploadSize, byteWrote);
        m_writeHead = (m_writeHead + byteWrote) % m_bufLen;

        if (byteWrote <= 0) {
            assert(false);
            throw CyclicBuff::exception("End of file");
        }

        uploadSize += byteWrote;
    } while (len != uploadSize);
}

void CyclicBuff::read(unsigned char* data, size_t len)
{
    size_t downloadSize = 0;
    do {

        size_t byteRead;
        do {
            byteRead = m_writeHead - m_readHead;
            if (byteRead < 0)
                byteRead = m_bufLen - m_readHead;
            byteRead = std::min(byteRead, len - downloadSize);
            if (byteRead == 0) {
//                std::cout << "[RamIO] nothing to read" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        } while (byteRead == 0);

        memcpy(data + downloadSize, &m_buff[m_readHead], byteRead);
        m_readHead = (m_readHead + byteRead) % m_bufLen;

        if (byteRead <= 0) {
            assert(false);
            throw CyclicBuff::exception("End of file");
        }

        downloadSize += byteRead;
    } while (len != downloadSize);
}

RamIO::RamIO(CyclicBuff& buff)
    : m_buff(buff)
{
}

void RamIO::close()
{
    //    mFile.close();
    //    m_buff = nullptr;
    //    m_len = 0;
}

void RamIO::write(const unsigned char* data, size_t len) const
{
    m_buff.write(data, len);
}

///////////////////////////////////////////////////////////////////////////////

void RamIO::read(unsigned char* data, size_t len) const
{
    m_buff.read(data, len);
}
