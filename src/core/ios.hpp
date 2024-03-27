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

#include <type_traits>

#include "Macros.hpp"
#if defined( HUB_DEBUG_INPUT ) || defined( HUB_DEBUG_OUTPUT )
#    include <mutex>
#endif
#include "Traits.hpp"

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
/// \brief has_data_t
///
template <typename T>
using has_data_t = decltype( std::declval<T>().data() );
template <typename T, typename = std::void_t<>>
struct has_data : std::false_type {};
template <typename T>
struct has_data<T, std::void_t<has_data_t<T>>> : std::true_type {};
template <typename T>
static constexpr bool has_data_v = has_data<T>::value;

///////////

///
/// \brief isPacket
///
template <class T>
constexpr bool isPacket = has_data_v<T>&& has_size_v<T>;

#endif

///
/// \brief packable_t
///
template <typename T>
using packable_t = decltype( T::packable );
template <typename T, typename = std::void_t<>>
struct packable : std::false_type {};
template <typename T>
struct packable<T, std::void_t<packable_t<T>>> : std::true_type {};
template <typename T>
static constexpr bool packable_v =
    packable<T>::value || std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T>;

} // namespace hub
