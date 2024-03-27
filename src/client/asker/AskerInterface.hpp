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
