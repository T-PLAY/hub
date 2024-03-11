/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/03

#pragma once

#include "ClientSocketI.hpp"
#include "core/Macros.hpp"

namespace hub {
namespace net {

///
/// \brief The ServerSocketI class
///
class SRC_API ServerSocketI
{
  public:
    ///
    /// \brief getPort
    /// \return
    ///
    virtual int getPort() const = 0;

    ///
    /// \brief waitNewClient
    /// \return
    ///
    ClientSocketI waitNewClient();
};

} // namespace net
} // namespace hub
