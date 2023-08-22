#include "InputFile.hpp"

//#include <filesystem>

namespace hub {
namespace input {

//InputFile::InputFile(std::ifstream && file)
//    : File(std::move(file))
//{

//}

InputFile::InputFile(const std::string &filePath)
    : File(filePath)
    , m_file(filePath, std::ios::in | std::ios::binary)
{
    assert(m_file.is_open());
}

InputFile::InputFile(const char *filePath)
    : File(filePath)
    , m_file(filePath, std::ios::in | std::ios::binary)
//            hub::io::File( std::fstream( filename, std::ios::in | std::ios::binary ) ) );
{
    assert(m_file.is_open());

}

bool InputFile::isOpen() const {
    return m_file.is_open();
}

void InputFile::close()
{
    assert(isOpen());
    m_file.close();
}

bool InputFile::isEnd() const {
    assert( isOpen() );
    return const_cast<std::ifstream*>(&m_file)->peek() == EOF;
//    return m_file.t
//    return m_file.eof();
//    return m_file.peek() == EOF;
}

void InputFile::read( unsigned char* data, size_t len ) {
    size_t downloadSize = 0;
    do {

        auto before = m_file.tellg();
        m_file.read( reinterpret_cast<char*>( data + downloadSize ), len - downloadSize );
        auto after    = m_file.tellg();
        auto byteRead = after - before;
        // std::cout << "[File] byteRead = " << byteRead << std::endl;
        if ( byteRead <= 0 ) { throw File::exception( "End of file" ); }

        downloadSize += (size_t)byteRead;
    } while ( len != downloadSize );
}

} // namespace input
} // namespace hub
