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

    ///
    /// \brief InputFile
    /// \param filePath
    ///
    explicit InputFile( const std::string& filePath );

  public:
    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;
    ///
    /// \brief close
    ///
    void close() override;
    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;
    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( Data_t* data, Size_t len ) override;
    ///
    /// \brief clear
    ///
    void clear() override;

  private:
    std::ifstream m_file;
};

} // namespace input
} // namespace hub
