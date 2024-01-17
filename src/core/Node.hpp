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

using Dims = std::vector<int>;

class Node
{
  public:
    Node() = default;
    // Node( HashType hash, Dims&& dims, std::string typeName, Size_t size );
    Node( Dims&& dims, std::string typeName, Size_t size, Id_t id );

    std::string toString( bool pretty ) const;

    Size_t size() const;

    bool operator==( const Node& other ) const;

#if CPLUSPLUSVERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) {
        // return archive( self.m_hashCode, self.m_dims, self.m_typeName, self.m_size );
        return archive( self.m_dims, self.m_typeName, self.m_size );
    }
#endif
    // friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_dims, self.m_typeName, self.m_size, self.m_id );
    }

    friend class Matrix;

    const Dims& getDims() const;
    const std::string& getTypeName() const;
    size_t getId() const;

  private:
    // cannot be const due of serialize
    // HashType m_hashCode;
    Dims m_dims;
    std::string m_typeName;
    Size_t m_size;
    Id_t m_id;
};

/////////////////////////////////////// MAKER /////////////////////////////////////////////////////

template <class Type, Size_t N = 1, Size_t... Ns>
#if CPLUSPLUSVERSION >= 20
    requires( N > 0 && ( ( Ns > 1 ) && ... ) )
#endif
static Node make_node() {
    auto size = sizeof( Type ) * N;
    if constexpr ( sizeof...( Ns ) > 0 ) {
        for ( auto dim : { Ns... } ) {
            size *= dim;
        }
    }
    return Node( std::move( Dims { N, Ns... } ), TYPE_NAME( Type() ), size, TYPE_ID(Type) );
}

template <class Type, class... Dims>
// requires( N > 0 && ( ( Ns > 1 ) && ... ) )
static Node make_node( const Dims&... dims ) {
    auto size = sizeof( Type );
    // if constexpr ( sizeof...( Ns ) > 0 ) {
    // for ( auto dim : { Ns... } ) {
    for ( auto dim : { dims... } ) {
        size *= dim;
    }
    // }
    return Node( hub::Dims { dims... }, TYPE_NAME( Type() ), size, TYPE_ID(Type) );
}

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

inline Node::Node( Dims&& dims, std::string typeName, Size_t size, Id_t id ) :
    // m_hashCode { hash },
    m_dims { std::move( dims ) },
    m_typeName { typeName },
    m_size { size },
    m_id { id }
{

}

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

inline Size_t Node::size() const {
    return m_size;
}

inline bool Node::operator==( const Node& other ) const {
    return
        // m_hashCode == other.m_hashCode &&
        m_dims == other.m_dims && m_typeName == other.m_typeName && m_size == other.m_size;
}

inline const Dims& Node::getDims() const {
    return m_dims;
}

inline const std::string& Node::getTypeName() const {
    return m_typeName;
}

inline size_t Node::getId() const
{
    return m_id;
}

} // namespace hub
