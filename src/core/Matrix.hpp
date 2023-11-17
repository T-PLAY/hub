
#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "core/Macros.hpp"
#include "core/Traits.hpp"

// #include "core/MatrixT.hpp"

// #include "MatrixBase.hpp"

namespace hub {

using Dims = std::vector<int>;

class Matrix
{
  public:
    class Node
    {
      public:
        using HashType = decltype( typeid( void ).hash_code() );
        //        template <class Type>
        Node() = default;
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
//            str += "(" + std::to_string(m_size) + ")";
            return str;
        }

        const auto size() const { return m_size; }

        bool operator==( const Node& other ) const {
            return m_hashCode == other.m_hashCode && m_dims == other.m_dims &&
                   m_name == other.m_name && m_size == other.m_size;
        }

        friend class Matrix;

        //        template <class Serial>
        //        void serialize( Serial& serial ) {}
        static constexpr auto serialize( auto& archive, auto& self ) {
            return archive( self.m_hashCode, self.m_dims, self.m_name, self.m_size );
        }

      private:
        HashType m_hashCode;
        Dims m_dims;
        std::string m_name;
        Size_t m_size;
    };

    Matrix() = default;

    Matrix( Node&& node ) {
        m_nodes.push_back( node );
        m_size += node.size();
    };

    //    template <class Type, Size_t N = 1, Size_t... Ns>
    //        requires( N > 0 && ( ( Ns > 1 ) && ... ) )
    //    Matrix() {
    //        m_nodes.push_back( Matrix::make_node<Type, N, Ns...>() );
    //    }

    Matrix(Matrix && matrix) = default;
    Matrix(const Matrix & matrix) = delete;

    Matrix & operator=(Matrix && matrix) = default;
    Matrix & operator=(const Matrix & matrix) = delete;

    Matrix clone() const {
        Matrix matrix;
        matrix << *this;
        return matrix;
    }

//    template <class MatrixT>
//    Matrix& operator=( const MatrixT& matrix ) {

//        return *this;
//    }

    //    void push_back( Node&& node ) {
    //        m_size += node.size();
    //        //        m_vector.resize( m_size );
    //        m_nodes.push_back( std::move( node ) );
    //    }

    Matrix & operator<<( const Matrix& other ) {
        for ( const auto& node : other.m_nodes ) {
            m_nodes.push_back( node );
            m_size += node.m_size;
        }
        return *this;
        //        return m_nodes;
    }

    std::string toString() const {
        std::string str;
        //        for ( const auto& node : m_nodes ) {
        for ( int i = 0; i < m_nodes.size(); ++i ) {
            const auto& node = m_nodes.at( i );
            str += node.toString();
            if ( i != m_nodes.size() - 1 ) str += "_";
        }
        str += "(" + std::to_string(m_size) + ")";

        if ( !m_vector.empty() ) {
            str += " = ";
            str += ::toString( m_vector );
        }
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
    bool hasAnyType() const {
        return ( hasType<Types>() && ... );
    }

    template <class... Types>
        requires( sizeof...( Types ) > 1 )
    bool hasSomeType() const {
        return ( hasType<Types>() || ... );
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

    Dims getDims( int i ) const { return m_nodes.at( i ).m_dims; }

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

    //    template <class Serial>
    //    void serialize( Serial& serial ) {
    //        //        serial(m_nodes);
    //        serial( m_size );
    //    }
    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.m_nodes, self.m_size );
    }

    void setData( const Data_t* data, Size_t size ) {
        assert( size == m_size );
        if ( m_vector.empty() ) m_vector.resize( m_size );
        assert( m_vector.size() == m_size );
        //        assert( m_vector.empty() );
        //        m_vector.resize( size );
        //        assert(m_vector.size() == size);
        std::copy( data, data + size, m_vector.data() );
    }

    const Data_t* data() const {
        //        if (m_vector.empty())
        //            m_vector.resize(m_size);
        assert( !m_vector.empty() );
        return m_vector.data();
    }
    Data_t* data() {
        if ( m_vector.empty() ) m_vector.resize( m_size );
        assert( !m_vector.empty() );
        return m_vector.data();
    }
    Size_t size() const { return m_size; }

    bool operator==( const Matrix& other ) const {
        return m_nodes == other.m_nodes && m_size == other.m_size;
    }

//  private:
    template <class Type, Size_t N = 1, Size_t... Ns>
        requires( N > 0 && ( ( Ns > 1 ) && ... ) )
    static Matrix::Node make_node() {
        auto size = sizeof( Type );
        for ( auto dim : { N, Ns... } ) {
            size *= dim;
        }
        return Matrix::Node(
            typeid( Type ).hash_code(), std::move( Dims { N, Ns... } ), TYPE_NAME( Type ), size );
        //    m_hashCode = typeid( Type ).hash_code();
        //    m_name     = TYPE_NAME( Type );
    }

  private:
    std::vector<Node> m_nodes;
    Size_t m_size = 0;
    std::vector<Data_t> m_vector;
};
// static_assert( sizeof( Matrix ) == 56 );

// template <class Type>
// Matrix::Node make_node(Dims && dims) {

//}

template <class Type, Size_t N = 1, Size_t... Ns>
    requires( ! std::is_same_v<Type, Matrix> && N > 0 && ( ( Ns > 1 ) && ... ) )
Matrix make_matrix() {
    //     Matrix matrix;
    return Matrix( Matrix::make_node<Type, N, Ns...>() );
    //     return matrix;
}

template <class... Matrices>
    requires (sizeof...(Matrices) > 1)
Matrix make_matrix(const Matrices&... matrices) {
    Matrix matrix;
//    ((matrix << matrices) << ...);
    (matrix << ... << matrices);
    return matrix;
//    return Matrix();
}

//    static_assert(decltype(m_hashCode) == 8);
static_assert( sizeof( decltype( typeid( void ).hash_code() ) ) == 8 );

} // namespace hub
