/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/30
 */

#pragma once

#include <list>
#include <string>

#include "core/Macros.hpp"
#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace client {

///
/// \brief The AskerInterface interface
/// allow user to get some information from connected sensor to the server.
///
class SRC_API AskerInterface
{
  public:
    ///
    /// \brief listStreams
    /// \return
    /// list of connected sensors to the server
    ///
    virtual std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() = 0;

    ///
    /// \brief getAcquisition
    /// allow to get last acquired data from connected sensor to the server
    /// \param streamName
    /// \return
    ///
    virtual sensor::Acquisition getAcquisition( const std::string& streamName ) = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
};

} // namespace client
} // namespace hub
