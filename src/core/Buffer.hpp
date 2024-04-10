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

#include <array>
#include <type_traits>
#include <vector>

#include "Base.hpp"

namespace hub {

///
/// \brief The BufferOption enum
///
enum BufferOption {
    StaticMemory  = 0x1,
    DynamicMemory = 0x2,
};

///
/// \brief The BufferBase class
/// is an interface representing buffer
///
template <class Type, Size_t Size, BufferOption Option>
class BufferBase
{
  public:
    ///
    /// \brief data
    /// \return
    ///
    virtual Data_t* data()             = 0;

    ///
    /// \brief data
    /// \return
    ///
    virtual const Data_t* data() const = 0;

    ///
    /// \brief size
    /// \return
    ///
    virtual Size_t size() const        = 0;

    ///
    /// \brief toString
    /// \return
    ///
    virtual CONSTEXPR20 std::string toString() const = 0;

#if CPP_VERSION >= 20
    using Span             = std::span<Type, Size>;
    virtual Span getSpan() = 0;
    void getData( Span& span ) const {
        const auto& internalSpan = getSpan();
        std::copy( internalSpan.begin(), internalSpan.end(), span.begin() );
    }
    void setData( const Span& span ) {
        const auto& internalSpan = getSpan();
        std::copy( span.begin(), span.end(), internalSpan.begin() );
    }
#endif
};

////////////////////////////////////////////////////////////////////

///
/// \brief The Buffer class
/// is an implement of BufferBase
/// the data is stored in the heap/stack depending of the size of data
///
template <class Type,
          Size_t Size,
          BufferOption Option = ( Size <= MAX_STACK_SIZE ) ? ( StaticMemory ) : ( DynamicMemory )>
class Buffer : public BufferBase<Type, Size, Option>
{
  public:
    /// \brief _Option
    BufferOption _Option = Option;
};

////////////////////////////////////////////////////////////////////

///
/// \brief The Buffer class
/// represents data stored in stack memory
///
template <class Type, Size_t Size>
class Buffer<Type, Size, StaticMemory> : public BufferBase<Type, Size, StaticMemory>
{
  public:
    ///
    /// \brief Buffer
    /// \param args
    ///
    template <class... Args>
    constexpr explicit Buffer( Args&&... args ) : m_array { std::forward<Type&&>( args )... } {}

    ///
    /// \brief get
    /// \return
    ///
    template <Size_t i>
    constexpr Type get() const {
        return m_array.at( i );
    }

    ///
    /// \brief toString
    /// \return
    ///
    CONSTEXPR20 std::string toString() const override {
        std::string str;
        str += "(static)";
        str += hub::to_string( m_array );
        return str;
    }

    ///
    /// \brief data
    /// \return
    ///
    Data_t* data() override { return (Data_t*)m_array.data(); }

    ///
    /// \brief data
    /// \return
    ///
    const Data_t* data() const override { return (const Data_t*)m_array.data(); }

    ///
    /// \brief size
    /// \return
    ///
    Size_t size() const override { return m_array.size(); }

#if CPP_VERSION >= 20
    using Span = typename BufferBase<Type, Size, StaticMemory>::Span;
    Span getSpan() override { return Span { m_array.begin(), m_array.end() }; }
#endif

  private:
    std::array<Type, Size> m_array;
};

////////////////////////////////////////////////////////////////////

///
/// \brief The Buffer class
/// represents data stored in heap memory
///
template <class Type, Size_t Size>
class Buffer<Type, Size, DynamicMemory> : public BufferBase<Type, Size, DynamicMemory>
{
  public:
    constexpr Buffer() : m_vector( Size ) {}

    ///
    /// \brief Buffer
    /// \param args
    ///
    template <class... Args>
    constexpr explicit Buffer( Args&&... args ) : m_vector { std::forward<Type&&>( args )... } {}

    ///
    /// \brief toString
    /// \return
    ///
    CONSTEXPR20 std::string toString() const override {
        std::string str;
        str += "(dynamic)";
        str += hub::to_string( m_vector );
        return str;
    }

    ///
    /// \brief data
    /// \return
    ///
    Data_t* data() override { return (Data_t*)m_vector.data(); }

    ///
    /// \brief data
    /// \return
    ///
    const Data_t* data() const override { return (const Data_t*)m_vector.data(); }

    ///
    /// \brief size
    /// \return
    ///
    Size_t size() const override { return m_vector.size(); }

#if CPP_VERSION >= 20
    using Span = typename BufferBase<Type, Size, DynamicMemory>::Span;
    Span getSpan() override { return std::span<Type, Size> { m_vector.begin(), m_vector.end() }; }
#endif

  private:
    std::vector<Type> m_vector;
};

} // namespace hub
