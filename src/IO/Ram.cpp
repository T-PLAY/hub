#include "IO/Ram.hpp"

#include <cassert>
#include <iostream>

#include <cstring>
#include <thread>

namespace hub {
namespace io {

CyclicBuff::CyclicBuff( size_t size ) : m_buff( new unsigned char[size] ), m_buffLen( size ) {}

CyclicBuff::~CyclicBuff() {
    //    assert( !m_outputSensorWantsToClose );
    //    assert( !m_inputSensorClose );
    m_outputSensorWantsToClose = true;

    delete[] m_buff;
    std::cout << "[CyclicBuff:" << this << "] ~CyclicBuff()" << std::endl;
}

void CyclicBuff::write( const unsigned char* data, size_t len ) {
    assert( !m_outputSensorWantsToClose );

    size_t uploadSize = 0;
    do {
        // size of empty space in buff to write data
        long spaceLeft = m_readHead - m_writeHead;
        if ( spaceLeft <= 0 ) spaceLeft += m_buffLen;

        long byteWrote = spaceLeft;
        assert( byteWrote >= 0 );
        if ( len > byteWrote ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
            continue;
            //            throw CyclicBuff::exception( "Buffer overflow, no space left" );
            //            assert( false );
        }
        byteWrote =
            std::min( byteWrote, (long)len - (long)uploadSize ); // size of not copied user data
        byteWrote =
            std::min( byteWrote, (long)m_buffLen - (long)m_writeHead ); // distance to buffer end
        assert( byteWrote > 0 );

        memcpy( &m_buff[m_writeHead], data + uploadSize, byteWrote );
        m_writeHead = ( m_writeHead + byteWrote ) % m_buffLen;

        //        if ( byteWrote <= 0 ) {
        //            assert( false );
        //            throw CyclicBuff::exception( "End of file" );
        //        }

        uploadSize += byteWrote;
    } while ( len != uploadSize );
}

void CyclicBuff::read( unsigned char* data, size_t len ) {
    size_t downloadSize = 0;
    do {

        long byteRead;
        do {
            if ( m_outputSensorWantsToClose ) {
                m_inputSensorClose = true;
                std::cout << "[CyclicBuff:" << this << "] read() : inputSensor close" << std::endl;
                throw CyclicBuff::exception( "Connection closed" );
                //                assert( false );
            }
            if ( m_buff == nullptr ) {
                m_inputSensorClose = true;
                throw CyclicBuff::exception( "End of buffer (nullptr)" );
                //                assert( false );
            }
            // nb bytes ready to read
            byteRead = m_writeHead - m_readHead;
            if ( byteRead == 0 ) { std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) ); }
            if ( byteRead < 0 ) byteRead = m_buffLen - m_readHead;
            byteRead =
                std::min( byteRead, (long)len - (long)downloadSize ); // user copied bytes left
            byteRead = std::min( byteRead,
                                 (long)m_buffLen - (long)m_readHead ); // distance to end of buffer

        } while ( byteRead == 0 );

        memcpy( data + downloadSize, &m_buff[m_readHead], byteRead );
        m_readHead = ( m_readHead + byteRead ) % m_buffLen;

        assert( byteRead > 0 );
        //        if ( byteRead <= 0 ) {
        //            assert( false );
        //            throw CyclicBuff::exception( "End of file" );
        //        }

        downloadSize += byteRead;
    } while ( len != downloadSize );
}

void CyclicBuff::close()  {
    m_outputSensorWantsToClose = true;
    m_inputSensorClose         = true;
}

bool CyclicBuff::isOpen() const {
    return !m_inputSensorClose;
}

bool CyclicBuff::isEnd() const {
    return m_writeHead == m_readHead;
}

Ram::Ram( CyclicBuff& buff ) : m_buff( buff ) {}

void Ram::close()  {
    assert( isOpen() );
    m_buff.close();
    assert( !isOpen() );
}

bool Ram::isOpen() const {
    return m_buff.isOpen();
}

bool Ram::isEnd() const {
    assert( isOpen() );
    return m_buff.isEnd();
}

void Ram::write( const unsigned char* data, size_t len )  {
    m_buff.write( data, len );
}

void Ram::read( unsigned char* data, size_t len )  {
    try {
        m_buff.read( data, len );
    }
    catch ( std::exception& e ) {
        std::cout << "[Ram] catch exception : " << e.what() << std::endl;
        throw e;
    }
}

} // namespace io
} // namespace hub
