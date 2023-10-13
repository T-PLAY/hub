#include "OutputFile.hpp"

namespace hub {
namespace output {

OutputFile::OutputFile(const std::string &filePath)
    : File(filePath)
    , m_file(filePath, std::ios::out | std::ios::binary | std::ios::trunc)
//                std::fstream( filename, std::ios::out | std::ios::binary | std::ios::trunc ) ) );
{
    assert(m_file.is_open());

}

OutputFile::OutputFile(const char *filePath)
    : File(filePath)
    , m_file(filePath, std::ios::out | std::ios::binary | std::ios::trunc)
{
    assert(m_file.is_open());

}

bool OutputFile::isOpen() const {
    return m_file.is_open();
}

void OutputFile::close() {
    m_file.close();
    assert( !isOpen() );
}


//void OutputFile::write( const Data_t* data, Size_t len ) {
void OutputFile::write( const Data_t* data, Size_t len ) {
    m_file.write( reinterpret_cast<const char*>( data ), len );
}


} // namespace output
} // namespace hub
