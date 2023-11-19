#pragma once

#include <fstream>

#include "core/Input.hpp"
#include "io/File.hpp"

namespace hub {
namespace input {

class InputFile : public Input, public io::File
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
//    bool isEnd() const override;
    bool isEnd() const override;

    ///
    /// @copydoc InputOutput::read()
    ///
    void read( Data_t* data, Size_t len ) override;

    void clear() override;

  private:
    std::ifstream m_file;
};


} // namespace input
} // namespace hub
