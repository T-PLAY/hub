#include "OutputFile.hpp"

namespace hub {
using namespace io;
namespace output {

OutputFile::OutputFile(const std::string &filePath)
    : File(filePath)
    , m_file(filePath, std::ios::out | std::ios::binary | std::ios::trunc)
//                std::fstream( filename, std::ios::out | std::ios::binary | std::ios::trunc ) ) );
{

}

OutputFile::OutputFile(const char *filePath)
    : File(filePath)
    , m_file(filePath, std::ios::out | std::ios::binary | std::ios::trunc)
{

}

bool OutputFile::isOpen() const {
    return m_file.is_open();
}

void OutputFile::close() {
    m_file.close();
    assert( !isOpen() );
}


void OutputFile::write( const unsigned char* data, size_t len ) {
    m_file.write( reinterpret_cast<const char*>( data ), len );
}


} // namespace output
} // namespace hub
