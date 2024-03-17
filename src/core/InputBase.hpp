/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/09
 */

#pragma once

#include <cassert>

#include "Macros.hpp"
#include "ios.hpp"

namespace hub {

///
/// \brief InputBase
/// is an ios interface specify basic input features
///
class SRC_API InputBase : public ios
{
  public:
    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    virtual void read( Data_t* data, Size_t len ) = 0;

    ///
    /// \brief isEnd
    /// \return
    ///
    virtual bool isEnd() const                    = 0;

    ///
    /// \brief clear
    ///
    virtual void clear()                          = 0;
};

///
/// \brief not_endable_t
///
template <typename T>
using not_endable_t = decltype( T::not_endable );

template <typename T, typename = std::void_t<>>
struct not_endable : std::false_type {};

template <typename T>
struct not_endable<T, std::void_t<not_endable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool not_endable_v = not_endable<T>::value;

#if CPP_VERSION >= 20
template <class T>
concept Endable = !not_endable_v<T>;
#else

///
/// \brief Endable
///
template <class T>
constexpr bool Endable = !not_endable_v<T>;

#endif

///////////////////////////////////////

///
/// \brief readable_t
///
template <typename T>
using readable_t = decltype( std::declval<T>().read( std::declval<InputBase&>() ) );

template <typename T, typename = std::void_t<>>
struct readable : std::false_type {};

template <typename T>
struct readable<T, std::void_t<readable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool readable_v = readable<T>::value;

///////////////////////////////////////

///
/// \brief notReadable_t
///
template <typename T>
using notReadable_t = decltype( T::notReadable );

template <typename T, typename = std::void_t<>>
struct notReadable : std::false_type {};

template <typename T>
struct notReadable<T, std::void_t<notReadable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool notReadable_v = notReadable<T>::value;

} // namespace hub
