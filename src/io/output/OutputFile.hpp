#pragma once

#include <fstream>

#include "Output.hpp"
#include "io/File.hpp"

namespace hub {
namespace output {

class OutputFile : public io::Output, public io::File
{
  public:
    explicit OutputFile(const std::string & filePath);
    explicit OutputFile(const char * filePath);

  protected:
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
    void write( const unsigned char* data, size_t len ) override;

  private:
    std::ofstream m_file;

};


} // namespace output
} // namespace hub
