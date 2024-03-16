#include "io/File.hpp"

#include <iostream>
#include <memory>

namespace hub {
namespace io {

File::File( const std::string& filePath ) : m_filePath( filePath ) {}

File::File( const char* filePath ) : m_filePath( filePath ) {}

const std::string& File::getFilePath() const {
    return m_filePath;
}

const io::Header& File::getHeader() const {
    return m_header;
}

} // namespace io
} // namespace hub
