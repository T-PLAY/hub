#pragma once

#include <fstream>

#include "core/Output.hpp"
#include "io/File.hpp"

namespace hub {
namespace output {

class OutputFile : public Output, public io::File
{
  public:
    explicit OutputFile(const std::string & filePath);
    explicit OutputFile(const char * filePath);

//  protected:
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

  private:
    std::ofstream m_file;

};


} // namespace output
} // namespace hub
