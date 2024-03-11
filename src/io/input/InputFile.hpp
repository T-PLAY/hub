/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/17

#pragma once

#include <fstream>

#include "core/Input.hpp"
#include "io/File.hpp"

namespace hub {
namespace input {

///
/// \brief The InputFile class
///
class SRC_API InputFile : public Input, public io::File
{
  public:
    using Input::read;
    using io::File::getHeader;

    explicit InputFile( const std::string& filePath );

  public:
    bool isOpen() const override;
    void close() override;
    bool isEnd() const override;
    void read( Data_t* data, Size_t len ) override;
    void clear() override;

  private:
    std::ifstream m_file;
};

} // namespace input
} // namespace hub
