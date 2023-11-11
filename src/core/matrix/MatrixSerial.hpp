
#pragma once

#include <algorithm>
#include <type_traits>

#include "MatrixBase.hpp"

namespace hub {

using Dims = std::vector<int>;

class MatrixSerial
{
  public:
    class Node
    {
      public:
        using HashType = decltype( typeid( void ).hash_code() );
        //        template <class Type>
        Node( HashType hash, Dims&& dims, std::string name, Size_t size ) :
            m_hashCode { hash }, m_dims { std::move( dims ) }, m_name { name }, m_size { size } {

            //            m_hashCode = typeid( Type ).hash_code();
            //            m_name     = TYPE_NAME( Type );
            //            m_size     = sizeof( Type );
            //            for ( auto dim : m_dims ) {
            //                m_size *= dim;
            //            }
        }

        std::string toString() const {
            //            return m_name + " " + ::toString( m_dims ) + " " + std::to_string( m_size
            //            );
            std::string str;
            str += m_name;
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

        const auto size() const { return m_size; }

        friend class MatrixSerial;

      private:
        HashType m_hashCode;
        Dims m_dims;
        std::string m_name;
        Size_t m_size;
    };

    void push_back( Node&& node ) {
        m_size += node.size();
        //        m_vector.resize(m_size);
        m_nodes.push_back( std::move( node ) );
    }

    std::string toString() const {
        std::string str;
        //        for ( const auto& node : m_nodes ) {
        for ( int i = 0; i < m_nodes.size(); ++i ) {
            const auto& node = m_nodes.at( i );
            str += node.toString();
            if ( i != m_nodes.size() - 1 ) str += "_";
        }
        str += " = ";
        str += ::toString( m_vector );
        return str;
    }

    template <class Type>
    bool hasType() const {
        for ( const auto& node : m_nodes ) {
            if ( node.m_hashCode == typeid( Type ).hash_code() ) { return true; }
        }
        return false;
    }

    template <class... Types>
        requires( sizeof...( Types ) > 1 )
    bool hasType() const {
        return ( hasType<Types>() && ... );
    }

    template <class Type>
    constexpr int nType() {
        assert( hasType<Type>() );
        int ret             = 0;
        const auto typeHash = typeid( Type ).hash_code();
        for ( const auto& node : m_nodes ) {
            if ( node.m_hashCode == typeHash ) { ++ret; }
        }
        return ret;
    }

    Dims getDims(int i) const {
        return m_nodes.at(i).m_dims;
    }

    template <class Type, int i = 0>
    Dims getDims() const {
        int nFound          = 0;
        const auto typeHash = typeid( Type ).hash_code();
        for ( const auto& node : m_nodes ) {
            if ( node.m_hashCode == typeHash ) {
                if ( i == nFound ) { return node.m_dims; }
                ++nFound;
            }
        }
        assert( false );
        return {};
    }

    template <class Type>
    Size_t getOffset( int i ) const {
        Size_t offset = 0;
        int cptFound  = 0;

        const auto typeHash = typeid( Type ).hash_code();

        for ( const auto& node : m_nodes ) {
            if ( node.m_hashCode == typeHash ) {
                if ( cptFound == i ) { return offset; }
                ++cptFound;
            }
            offset += node.m_size;
        }

        return offset;
    }

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    //        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
        requires( std::is_pointer_v<Type> )
    Type get() {
        assert( m_vector.size() == m_size );
        const auto offset = getOffset<RawType>( i );
        assert( 0 <= offset && offset < m_size );
        return reinterpret_cast<Type>( m_vector.data() + offset );
    }
    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    //        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
        requires( std::is_pointer_v<Type> )
    Type get() const {
        assert( m_vector.size() == m_size );
        const auto offset = getOffset<RawType>( i );
        assert( 0 <= offset && offset < m_size );
        return reinterpret_cast<Type>( m_vector.data() + offset );
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    //        requires( !std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
        requires( !std::is_pointer_v<Type> )
    Type get() {
        assert( m_vector.size() == m_size );
        const auto offset = getOffset<RawType>( i );
        assert( 0 <= offset && offset < m_size );
        return reinterpret_cast<Type>( *( m_vector.data() + offset ) );
    }

    void setData( const Data_t* data, Size_t size ) {
        assert( size == m_size );
        assert( m_vector.empty() );
        m_vector.resize( size );
        //        assert(m_vector.size() == size);
        std::copy( data, data + size, m_vector.data() );
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    //        requires( !std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
        requires( !std::is_pointer_v<Type> )
    Type get() const {
        assert( m_vector.size() == m_size );
        //        const auto offset = getOffset<i, 0, RawType, Types...>();
        const auto offset = 0;
        assert( 0 <= offset && offset < m_size );
        return reinterpret_cast<Type>( *( m_vector.data() + offset ) );
    }

    auto nType() const { return m_nodes.size(); }

  private:
    std::vector<Node> m_nodes;
    Size_t m_size = 0;
    std::vector<Data_t> m_vector;
};
static_assert( sizeof( MatrixSerial ) == 56 );

template <class Type>
MatrixSerial::Node make_node( Dims&& dims ) {
    auto size = sizeof( Type );
    for ( auto dim : dims ) {
        size *= dim;
    }
    return MatrixSerial::Node(
        typeid( Type ).hash_code(), std::move( dims ), TYPE_NAME( Type ), size );
    //    m_hashCode = typeid( Type ).hash_code();
    //    m_name     = TYPE_NAME( Type );
}
// template <class Type>
// MatrixSerial::Node make_node(Dims && dims) {

//}

//    static_assert(decltype(m_hashCode) == 8);
static_assert( sizeof( decltype( typeid( void ).hash_code() ) ) == 8 );

} // namespace hub
