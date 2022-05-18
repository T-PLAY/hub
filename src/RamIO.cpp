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

CyclicBuff::CyclicBuff(size_t size)
    : m_buff(new unsigned char[size])
    , m_buffLen(size)
{
    //    assert(m_buff == nullptr);
    //    m_buff = new unsigned char[size];
}

// CyclicBuff &CyclicBuff::operator=(const CyclicBuff &buff)
//{
//     m_buffLen = buff.m_buffLen;
//     return *this;
// }

CyclicBuff::~CyclicBuff()
{
    assert(!m_outputStreamWantsToClose);
    assert(!m_inputStreamClose);
    m_outputStreamWantsToClose = true;
    while (!m_inputStreamClose) {
        std::cout << "[CyclicBuff:" << this << "] ~CyclicBuff() : wait for inputStreamClose" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    //    if (m_inputStreamClose) {
    delete[] m_buff;
    //    m_buff = nullptr;
    //    m_buff = nullptr;
    std::cout << "[CyclicBuff:" << this << "] ~CyclicBuff()" << std::endl;
    //    }
}

void CyclicBuff::write(const unsigned char* data, size_t len)
{
    assert(!m_outputStreamWantsToClose);

    size_t uploadSize = 0;
    do {
        // size of empty space in buff to write data
        auto byteWrote = m_readHead - m_writeHead;
        if (byteWrote <= 0)
            byteWrote += m_buffLen;
        if (len > byteWrote) {
            assert(false);
            throw CyclicBuff::exception("Buffer overflow");
        }
        byteWrote = std::min(byteWrote, len - uploadSize); // size of not copied user data
        byteWrote = std::min(byteWrote, m_buffLen - m_writeHead); // distance to buffer end

        memcpy(&m_buff[m_writeHead], data + uploadSize, byteWrote);
        m_writeHead = (m_writeHead + byteWrote) % m_buffLen;

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
            if (m_outputStreamWantsToClose) {
                m_inputStreamClose = true;
                std::cout << "[CyclicBuff:" << this << "] read() : inputStream close" << std::endl;
                throw CyclicBuff::exception("Connection closed");
                assert(false);
            }
            if (m_buff == nullptr) {
                m_inputStreamClose = true;
                //                std::cout << "[CyclicBuff] read() : inputStream close" << std::endl;
                throw CyclicBuff::exception("End of buffer (nullptr)");
                assert(false);
            }
            // nb bytes ready to read
            byteRead = m_writeHead - m_readHead;
            if (byteRead == 0) {
                //                std::cout << "[RamIO] nothing to read" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
            if (byteRead < 0)
                byteRead = m_buffLen - m_readHead;
            byteRead = std::min(byteRead, len - downloadSize); // user copied bytes left
            byteRead = std::min(byteRead, m_buffLen - m_readHead); // distance to end of buffer

        } while (byteRead == 0);

        memcpy(data + downloadSize, &m_buff[m_readHead], byteRead);
        m_readHead = (m_readHead + byteRead) % m_buffLen;

        if (byteRead <= 0) {
            assert(false);
            throw CyclicBuff::exception("End of file");
        }

        downloadSize += byteRead;
    } while (len != downloadSize);
}

void CyclicBuff::close()
{
}

RamIO::RamIO(CyclicBuff& buff)
    : m_buff(buff)
{
}

void RamIO::close()
{
    m_buff.close();

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
    try {
        m_buff.read(data, len);
    } catch (std::exception& e) {
        std::cout << "[RamIO] catch exception : " << e.what() << std::endl;
        throw e;
    }
}
