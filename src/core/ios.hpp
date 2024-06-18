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

#include <type_traits>

#include "Base.hpp"
#if defined( HUB_DEBUG_INPUT ) || defined( HUB_DEBUG_OUTPUT )
#    include <mutex>
#endif

namespace hub {

#if defined( HUB_DEBUG_INPUT ) || defined( HUB_DEBUG_OUTPUT )

static std::mutex s_mtxIoPrint;

#    undef DEBUG_MSG
#    define DEBUG_MSG( str )               \
        s_mtxIoPrint.lock();               \
        do {                               \
            std::cout << str << std::endl; \
        } while ( false );                 \
        s_mtxIoPrint.unlock();

#endif

///
/// \brief The ios class
/// is an interface
/// defined minimal features of input/output purpose
///
class SRC_API ios
{
  public:
    ///
    /// \brief close
    ///
    virtual void close()        = 0;

    ///
    /// \brief isOpen
    /// \return
    ///
    virtual bool isOpen() const = 0;
};

#if CPP_VERSION >= 20

template <class T>
concept isPacket = requires( T t ) {
    t.data();
    t.size();
}; // uses by acquisition

#else

///
/// \brief Object with data() feature
///
template <typename T>
using has_data_t = decltype( std::declval<T>().data() );

/// \copydoc has_data_t
template <typename T, typename = std::void_t<>>
struct has_data : std::false_type {};

/// \copydoc has_data_t
template <typename T>
struct has_data<T, std::void_t<has_data_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_data_v = has_data<T>::value;

///////////

///
/// \copydoc has_size_t
/// \copydoc has_data_t
///
template <class T>
constexpr bool isPacket = has_data_v<T>&& has_size_v<T>;

#endif

///
/// \brief Object with packable propertie
///
template <typename T>
using packable_t = decltype( T::packable );

/// \copydoc packable_t
template <typename T, typename = std::void_t<>>
struct packable : std::false_type {};

/// \copydoc packable_t
template <typename T>
struct packable<T, std::void_t<packable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool packable_v =
    packable<T>::value || std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T>;

} // namespace hub
