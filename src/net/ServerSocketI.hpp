/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

// #include "ClientSocketI.hpp"
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
    // ClientSocketI waitNewClient();
};

} // namespace net
} // namespace hub
