#pragma once

#include <fstream>

#include "core/Output.hpp"
#include "io/Header.hpp"
#include "io/File.hpp"

namespace hub {
namespace output {

class OutputFile : public Output, public io::File
{
  public:
    using Output::write;

    explicit OutputFile(const io::Header & header, const std::string & filePath);
    // explicit OutputFile(const io::Header & header, const char * filePath);

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

    void setRetain(bool retained) override;

  private:
    std::ofstream m_file;

};


} // namespace output
} // namespace hub
