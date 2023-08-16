#include "io/File.hpp"

#include <cassert>
#include <iostream>
#include <memory>

namespace hub {
namespace io {

File::File( std::fstream&& file ) : m_file( new std::fstream( std::move( file ) ) ) {
//File::File( std::fstream&& file ) {
//File::File( std::fstream&& file ) : m_file(std::move(file)) {
//File::File( std::fstream&& file ) : m_file( std::make_unique<std::fstream>(std::move(file)) ) {

    assert( m_file->is_open() );
}

File::File( File&& fileIO ) : m_file( std::move( fileIO.m_file ) ) {
    fileIO.m_file = nullptr;
}

File::~File() {
    if ( m_file != nullptr ) {
        std::cout << "[File:" << this << "] ~File() started" << std::endl;
        // todo fix this bug
        File::close();
        assert( !File::isOpen() );
        std::cout << "[File:" << this << "] ~File() close file" << std::endl;
        std::cout << "[File:" << this << "] ~File() ended" << std::endl;
    }
}

void File::close() {
    m_file->close();
    assert( !isOpen() );
}

bool File::isOpen() const {
    return m_file->is_open();
}

bool File::isEnd() const {
    assert( isOpen() );
    return m_file->peek() == EOF;
}

void File::write( const unsigned char* data, size_t len ) {
    m_file->write( reinterpret_cast<const char*>( data ), len );
}

void File::read( unsigned char* data, size_t len ) {
    size_t downloadSize = 0;
    do {

        auto before = m_file->tellg();
        m_file->read( reinterpret_cast<char*>( data + downloadSize ), len - downloadSize );
        auto after    = m_file->tellg();
        auto byteRead = after - before;
        // std::cout << "[File] byteRead = " << byteRead << std::endl;
        if ( byteRead <= 0 ) { throw File::exception( "End of file" ); }

        downloadSize += (size_t)byteRead;
    } while ( len != downloadSize );
}

} // namespace io
} // namespace hub
