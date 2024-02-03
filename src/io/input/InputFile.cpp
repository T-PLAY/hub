#include "InputFile.hpp"

namespace hub {
namespace input {

InputFile::InputFile( const std::string& filePath ) :
    File( filePath ), m_file( filePath, std::ios::in | std::ios::binary ) {
    assert( m_file.is_open() );
    read( m_header );
}

bool InputFile::isOpen() const {
    return m_file.is_open();
}

void InputFile::close() {
    assert( isOpen() );
    m_file.close();
}

bool InputFile::isEnd() const {
    assert( isOpen() );
    return const_cast<std::ifstream*>( &m_file )->peek() == EOF;
}

void InputFile::read( Data_t* data, Size_t len ) {
    Size_t downloadSize = 0;
    do {

        auto before = m_file.tellg();
        m_file.read( reinterpret_cast<char*>( data + downloadSize ), len - downloadSize );
        auto after    = m_file.tellg();
        auto byteRead = after - before;
        if ( byteRead <= 0 ) { throw File::exception( "End of file" ); }

        downloadSize += (Size_t)byteRead;
    } while ( len != downloadSize );
}

void InputFile::clear() {}

} // namespace input
} // namespace hub
