/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/17

#pragma once

#include <fstream>

#include "core/Output.hpp"
#include "io/File.hpp"
#include "io/Header.hpp"

namespace hub {
namespace output {

class SRC_API OutputFile : public Output, public io::File
{
  public:
    using io::File::getHeader;
    using Output::write;

    explicit OutputFile( const io::Header& header, const std::string& filePath );

  public:
    ///
    /// @copydoc InputOutput::isOpen()
    ///
    bool isOpen() const override;
    ///
    /// @copydoc InputOutput::close()
    ///
    void close() override;

    ///
    /// @copydoc InputOutput::write()
    ///
    void write( const Data_t* data, Size_t len ) override;

    void setRetain( bool retained ) override;

  private:
    std::ofstream m_file;
};

} // namespace output
} // namespace hub
