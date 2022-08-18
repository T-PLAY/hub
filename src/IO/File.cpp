#include "IO/File.hpp"

#include <cassert>
#include <iostream>

namespace hub {
namespace io {

File::File( std::fstream&& file ) :
    mFile( *std::move( new std::fstream( std::move( file ) ) ) ) {
    assert( mFile.is_open() );
}

void File::close() {
    mFile.close();
}

void File::write( const unsigned char* data, size_t len ) const {
    mFile.write( (const char*)data, len );
}

void File::read( unsigned char* data, size_t len ) const {
    size_t downloadSize = 0;
    do {

        auto before = mFile.tellg();
        mFile.read( reinterpret_cast<char*>( data + downloadSize ), len - downloadSize );
        auto after    = mFile.tellg();
        auto byteRead = after - before;
        // std::cout << "[File] byteRead = " << byteRead << std::endl;
        if ( byteRead <= 0 ) { throw File::exception( "End of file" ); }

        downloadSize += (size_t)byteRead;
    } while ( len != downloadSize );
}

} // namespace io
} // namespace hub
