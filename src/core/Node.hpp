/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/01

#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Macros.hpp"
#include "Traits.hpp"

namespace hub {

///
/// \brief Dims
///
using Dims = std::vector<int>;

///
/// \brief The Node class
///
class Node
{
  public:
    Node() = default;
    // Node( HashType hash, Dims&& dims, std::string typeName, Size_t size );

    ///
    /// \brief Node
    /// \param dims
    /// \param typeName
    /// \param size
    /// \param id
    ///
    Node( Dims&& dims, std::string typeName, Size_t size, TypeId_t id );

    ///
    /// \brief toString
    /// \param pretty
    /// \return
    ///
    std::string toString( bool pretty ) const;

    // Size_t size() const;

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Node& other ) const;

#if CPLUSPLUSVERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.m_dims, self.m_typeName, self.m_size );
    }
#endif
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_dims, self.m_typeName, self.m_size, self.m_id );
    }

    friend class Matrix;

    ///
    /// \brief getDims
    /// \return
    ///
    const Dims& getDims() const;

    ///
    /// \brief getTypeName
    /// \return
    ///
    const std::string& getTypeName() const;

    ///
    /// \brief getSize
    /// \return
    ///
    Size_t getSize() const;

    ///
    /// \brief getTypeId
    /// \return
    ///
    TypeId_t getTypeId() const;

  private:
    // HashType m_hashCode;
    Dims m_dims;
    std::string m_typeName;
    Size_t m_size;
    TypeId_t m_id;
    // Data_t * m_data = nullptr;
};

/////////////////////////////////////// MAKER /////////////////////////////////////////////////////

template <class Type, Size_t N = 1, Size_t... Ns>
#if CPLUSPLUSVERSION >= 20
requires( N > 0 && ( ( Ns > 1 ) && ... ) )
#endif
    static Node make_node(
        // Data_t * data
    ) {
    auto size = hub::sizeOf<Type>() * N;
    if constexpr ( sizeof...( Ns ) > 0 ) {
        for ( auto dim : { Ns... } ) {
            size *= dim;
        }
    }
    return Node( std::move( Dims { N, Ns... } ), TYPE_NAME( Type() ), size, TYPE_ID( Type ) );
}

template <class Type, class... Dims>
static Node make_node(
    // Data_t * data,
    const Dims&... dims ) {
    auto size = hub::sizeOf<Type>();
    for ( auto dim : { dims... } ) {
        size *= dim;
    }
    return Node( hub::Dims { dims... }, TYPE_NAME( Type() ), size, TYPE_ID( Type ) );
}

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

inline Node::Node( Dims&& dims, std::string typeName, Size_t size, TypeId_t id ) :
    m_dims { std::move( dims ) }, m_typeName { typeName }, m_size { size }, m_id { id } {}

inline std::string Node::toString( bool pretty ) const {
    std::string str;
    str += m_typeName;
    str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );
    if ( !( m_dims.size() == 1 && m_dims.at( 0 ) == 1 ) ) {
        if ( pretty )
            str += " : ";
        else
            str += ":";
        Size_t i = 0;
        for ( auto dim : m_dims ) {
            str += std::to_string( dim );
            if ( i != m_dims.size() - 1 ) {
                if ( pretty )
                    str += " x ";
                else
                    str += "x";
            }
            ++i;
        }
    }
    return str;
}

// inline Size_t Node::size() const {
//     return m_size;
// }

inline bool Node::operator==( const Node& other ) const {
    return m_dims == other.m_dims && m_typeName == other.m_typeName && m_size == other.m_size;
}

inline const Dims& Node::getDims() const {
    return m_dims;
}

inline const std::string& Node::getTypeName() const {
    return m_typeName;
}

inline TypeId_t Node::getTypeId() const {
    return m_id;
}

inline Size_t Node::getSize() const {
    return m_size;
}

} // namespace hub
