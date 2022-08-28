#include "IO/Ram.hpp"

#include <cassert>
#include <iostream>

#include <cstring>
#include <thread>

namespace hub {
namespace io {

CyclicBuff::CyclicBuff( size_t size ) : m_buff( new unsigned char[size] ), m_buffLen( size ) {}

CyclicBuff::~CyclicBuff() {
    assert( !m_outputSensorWantsToClose );
    assert( !m_inputSensorClose );
    m_outputSensorWantsToClose = true;
//    while ( !m_inputSensorClose ) {
//        std::cout << "[CyclicBuff:" << this << "] ~CyclicBuff() : wait for inputSensorClose"
//                  << std::endl;
//        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
//    }

    delete[] m_buff;
    std::cout << "[CyclicBuff:" << this << "] ~CyclicBuff()" << std::endl;
}

void CyclicBuff::write( const unsigned char* data, size_t len ) {
    assert( !m_outputSensorWantsToClose );

    size_t uploadSize = 0;
    do {
        // size of empty space in buff to write data
        auto spaceLeft = m_readHead - m_writeHead;
        if ( spaceLeft <= 0 ) spaceLeft += m_buffLen;
        //        std::cout << "[CyclicBuff] buff size = " << spaceLeft << std::endl;

        size_t byteWrote = spaceLeft;
        if ( len > byteWrote ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
            continue;
            throw CyclicBuff::exception( "Buffer overflow, no space left" );
            assert( false );
        }
        byteWrote = std::min( byteWrote, len - uploadSize );        // size of not copied user data
        byteWrote = std::min( byteWrote, m_buffLen - m_writeHead ); // distance to buffer end

        memcpy( &m_buff[m_writeHead], data + uploadSize, byteWrote );
        m_writeHead = ( m_writeHead + byteWrote ) % m_buffLen;

        if ( byteWrote <= 0 ) {
            assert( false );
            throw CyclicBuff::exception( "End of file" );
        }

        uploadSize += byteWrote;
    } while ( len != uploadSize );

    //    std::cout << "[CyclicBuff] write " << len << " bytes" << std::endl;
}

void CyclicBuff::read( unsigned char* data, size_t len ) {
    size_t downloadSize = 0;
    do {

        size_t byteRead;
        do {
            if ( m_outputSensorWantsToClose ) {
                m_inputSensorClose = true;
                std::cout << "[CyclicBuff:" << this << "] read() : inputSensor close" << std::endl;
                throw CyclicBuff::exception( "Connection closed" );
                assert( false );
            }
            if ( m_buff == nullptr ) {
                m_inputSensorClose = true;
                //                std::cout << "[CyclicBuff] read() : inputSensor close" <<
                //                std::endl;
                throw CyclicBuff::exception( "End of buffer (nullptr)" );
                assert( false );
            }
            // nb bytes ready to read
            byteRead = m_writeHead - m_readHead;
            if ( byteRead == 0 ) {
                //                std::cout << "[Ram] nothing to read" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
            }
            if ( byteRead < 0 ) byteRead = m_buffLen - m_readHead;
            byteRead = std::min( byteRead, len - downloadSize );     // user copied bytes left
            byteRead = std::min( byteRead, m_buffLen - m_readHead ); // distance to end of buffer

        } while ( byteRead == 0 );

        memcpy( data + downloadSize, &m_buff[m_readHead], byteRead );
        m_readHead = ( m_readHead + byteRead ) % m_buffLen;

        if ( byteRead <= 0 ) {
            assert( false );
            throw CyclicBuff::exception( "End of file" );
        }

        downloadSize += byteRead;
    } while ( len != downloadSize );
}

void CyclicBuff::close() {}

Ram::Ram( CyclicBuff& buff ) : m_buff( buff ) {}

void Ram::close() {
    m_buff.close();
}

void Ram::write( const unsigned char* data, size_t len ) const {
    m_buff.write( data, len );
}

void Ram::read( unsigned char* data, size_t len ) const {
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
