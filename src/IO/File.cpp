#include "IO/File.hpp"

#include <cassert>
#include <iostream>

namespace hub {
namespace io {

File::File( std::fstream&& file ) :
    m_file( new std::fstream( std::move( file ) ) ) {

    assert( m_file->is_open() );
}

File::~File() {
    std::cout << "[File:" << this << "] ~File()" << std::endl;
    if ( m_file != nullptr ) {
        std::cout << "[File:" << this << "] ~File() close file" << std::endl;
        assert( !m_file->is_open() );
    }
}

void File::close() {
    m_file->close();
}

void File::write( const unsigned char* data, size_t len ) const {
    m_file->write( (const char*)data, len );
}

void File::read( unsigned char* data, size_t len ) const {
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
