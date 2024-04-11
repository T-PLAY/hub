/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <list>
#include <string>

#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace client {

///
/// \brief The AskerInterface interface
/// allow user to get some information from connected sensor to the server
///
class SRC_API AskerInterface
{
  public:
    ///
    /// \brief Get the active streams from the server
    /// \return List of connected streams from the server
    ///
    virtual std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() = 0;

    ///
    /// \brief Get last acquired data from connected stream from the server
    /// \param streamName [in] is the stream you want to get the acquisition
    /// \warning streamName you want to retrieve data has to be connected to the server
    /// \return Latest acquisition shared to the server by the sensor stream
    ///
    virtual sensor::Acquisition getAcquisition( const std::string& streamName ) = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
};

} // namespace client
} // namespace hub
