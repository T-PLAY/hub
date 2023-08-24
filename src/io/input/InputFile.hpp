#pragma once

#include <fstream>

#include "Input.hpp"
#include "io/File.hpp"

namespace hub {
namespace input {

class InputFile : public io::Input, public io::File
{
  public:
//    InputFile(std::ifstream &&file);
    explicit InputFile(const std::string & filePath);
    explicit InputFile(const char * filePath);

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
    /// @copydoc InputOutput::isEnd()
    ///
    bool isEnd() const override;

    ///
    /// @copydoc InputOutput::read()
    ///
    void read( unsigned char* data, size_t len ) override;

  private:
    std::ifstream m_file;
};


} // namespace input
} // namespace hub
