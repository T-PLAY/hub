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
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

#pragma once

#include "core/Any.hpp"

namespace hub {
namespace data {

///
/// \brief The UserData class
/// is a custom data, using for event sensor.
///
class SRC_API UserData
{
  public:
    ///
    /// \brief UserData
    /// \param name
    /// \param any
    ///
    UserData( const std::string& name, const Any& any );

    ///
    /// \brief getName
    /// \return
    ///
    const std::string& getName() const;
    ///
    /// \brief getValue
    /// \return
    ///
    const Any& getValue() const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param userData
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const UserData& userData );

  private:
    std::string m_name;
    Any m_value;
};

} // namespace data
} // namespace hub
