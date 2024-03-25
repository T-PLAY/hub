/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/15

#pragma once

#include <array>
#include <numeric>

#include "core/Macros.hpp"
#include "core/Traits.hpp"
#include "core/io/Memory.hpp"

namespace hub {
namespace io {

// Todo move to core input/output ios class, all stream data need versioning to be aware of compatibility
///
/// \brief The Header class
///
class SRC_API Header
{
  public:
    // using MagicNumber = std::array<char, 128>;
    ///
    /// \brief MagicNumber
    ///
    using MagicNumber = std::array<char, 16>;

    ///
    /// \brief getSize
    ///
    auto getSize() const {
        return sizeof( m_headerSize ) + sizeof( m_dataSize ) + 4 + m_userDefined.size();
    }

    ///
    /// \brief Header
    /// \param dataSize
    /// \param userDefined
    ///
    explicit Header( Size_t dataSize = 0, const Datas_t& userDefined = {} );

    ///
    /// \brief Header
    ///
    Header( const Header& ) = default;

    ///
    /// \brief operator =
    /// \return
    ///
    Header& operator=( const Header& ) = default;

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Header& other ) const {
        return m_magicNumber == other.m_magicNumber && m_headerSize == other.m_headerSize &&
               m_dataSize == other.m_dataSize && m_userDefined == other.m_userDefined;
    }

    ///
    /// \brief toString
    ///
    auto toString() const {
        std::string str;
        // cppcheck-suppress useStlAlgorithm
        for ( const auto& number : m_magicNumber ) {
            str += number;
        }
        return str + "header size: " + std::to_string( m_headerSize ) + "\ndata size: " + std::to_string( m_dataSize ) + "\nuser defined: " +
               hub::to_string( m_userDefined );
    }

    ///
    /// \brief write
    /// \param output
    ///
    template <class Output>
    void write( Output& output ) {
        output.write( reinterpret_cast<Data_t*>(m_magicNumber.data()), m_magicNumber.size() );
        output.writeAll( m_headerSize, m_dataSize, m_userDefined );
    }

    ///
    /// \brief read
    /// \param input
    ///
    template <class Input>
    void read( Input& input ) {
        input.read( reinterpret_cast<Data_t*>(m_magicNumber.data()), m_magicNumber.size() );

        checkMagicNumber();

        input.readAll( m_headerSize, m_dataSize, m_userDefined );
    }

    ///
    /// \brief getDataSize
    /// \return
    ///
    Size_t getDataSize() const { return m_dataSize; }

    ///
    /// \brief getUserDefined
    /// \return
    ///
    const Datas_t& getUserDefined() const { return m_userDefined; }

  private:
    void checkMagicNumber() const;

  private:
    MagicNumber m_magicNumber;

    Size_t m_headerSize;
    Size_t m_dataSize;
    Datas_t m_userDefined;

    static_assert( sizeof( m_headerSize ) == 8 );
    static_assert( sizeof( m_dataSize ) == 8 );
};

///
/// \brief make_header
/// \param t
/// \return
///
template <class T>
Header make_header( const T& t ) {

    // std::cout << "[Header] make_header() making header ..." << std::endl;

    Memory memory;
    memory.write( t );

    // std::cout << "[Header] make_header() memory : " << memory.getData() << std::endl;

    Header header( t.dataSize(), memory.getData() );

    // std::cout << "[Header] make_header() done" << std::endl;
    return header;
}

} // namespace io
} // namespace hub
