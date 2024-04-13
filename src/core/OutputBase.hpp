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

#include <cassert>

#include "Base.hpp"
#include "ios.hpp"

namespace hub {

///
/// \brief The OutputBase class
/// is an ios interface specify basic output features
///
class SRC_API OutputBase : public ios
{
  public:
    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    virtual void write( const Data_t* data, Size_t len ) = 0;

    ///
    /// \brief setRetain
    /// \param retain
    ///
    virtual void setRetain( bool retain )                = 0;
};

///
/// \brief Object that can write to an output
///
template <typename T>
using writable_t = decltype( std::declval<T>().write( std::declval<OutputBase&>() ) );

/// \copydoc writable_t
template <typename T, typename = std::void_t<>>
struct writable : std::false_type {};

/// \copydoc writable_t
template <typename T>
struct writable<T, std::void_t<writable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool writable_v = writable<T>::value;

////////////////////

///
/// \brief Object with notWritable property
///
template <typename T>
using notWritable_t = decltype( T::notWritable );

/// \copydoc notWritable_t
template <typename T, typename = std::void_t<>>
struct notWritable : std::false_type {};

/// \copydoc notWritable_t
template <typename T>
struct notWritable<T, std::void_t<notWritable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool notWritable_v = notWritable<T>::value;

} // namespace hub
