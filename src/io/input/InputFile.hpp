/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/08/17
 */

#pragma once

#include <fstream>

#include "core/Input.hpp"
#include "io/File.hpp"

namespace hub {
namespace input {

///
/// \brief The InputFile class
/// is an implement of Input using disk file
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
