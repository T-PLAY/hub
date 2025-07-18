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
/// is a custom data, using for sensor events.
///
class SRC_API UserData
{
  public:
    ///
    /// \brief Corresponds to a name with any value
    /// \param name [in] represents the name of data
    /// \param any [in] is the value of data
    ///
    UserData( const std::string& name, const Any& any );

    ///
    /// \brief Getter function
    /// \return the name of user data
    ///
    const std::string& getName() const;

    ///
    /// \brief Getter function
    /// \return The value of user data
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
