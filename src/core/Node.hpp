
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
    using HashType = decltype( typeid( void ).hash_code() );

    Node() = default;
    Node( HashType hash, Dims&& dims, std::string name, Size_t size );

    std::string toString() const;

    Size_t size() const;

    bool operator==( const Node& other ) const;

#if CPLUSPLUSVERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.m_hashCode, self.m_dims, self.m_name, self.m_size );
    }
#endif

    friend class Matrix;

  private:
    HashType m_hashCode;
    Dims m_dims;
    std::string m_name;
    Size_t m_size;
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
    return Node(
        typeid( Type ).hash_code(), std::move( Dims { N, Ns... } ), TYPE_NAME( Type ), size );
}

template <class Type, class... Dims>
    // requires( N > 0 && ( ( Ns > 1 ) && ... ) )
static Node make_node(const Dims&... dims) {
    auto size = sizeof( Type );
    // if constexpr ( sizeof...( Ns ) > 0 ) {
        // for ( auto dim : { Ns... } ) {
        for ( auto dim : { dims... } ) {
            size *= dim;
        }
    // }
    return Node(
            // typeid( Type ).hash_code(), hub::Dims { std::forward<const int&>(dims)... }, TYPE_NAME( Type ), size );
            // typeid( Type ).hash_code(), hub::Dims { std::forward<int>(dims)... }, TYPE_NAME( Type ), size );
            typeid( Type ).hash_code(), hub::Dims { dims... }, TYPE_NAME( Type ), size );
}

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

inline Node::Node( HashType hash, Dims&& dims, std::string name, Size_t size ) :
    m_hashCode { hash }, m_dims { std::move( dims ) }, m_name { name }, m_size { size } {}

inline std::string Node::toString() const {
    std::string str;
    str += m_name;
    str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );
    if ( !( m_dims.size() == 1 && m_dims.at( 0 ) == 1 ) ) {
        str += ":";
        Size_t i = 0;
        for ( auto dim : m_dims ) {
            str += std::to_string( dim );
            if ( i != m_dims.size() - 1 ) str += "x";
            ++i;
        }
    }
    return str;
}

inline Size_t Node::size() const {
    return m_size;
}

inline bool Node::operator==( const Node& other ) const {
    return m_hashCode == other.m_hashCode && m_dims == other.m_dims && m_name == other.m_name &&
           m_size == other.m_size;
}

} // namespace hub
