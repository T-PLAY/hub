
#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "core/Macros.hpp"
#include "core/Node.hpp"
#include "core/Traits.hpp"

namespace hub {

class Matrix
{
  public:
    Matrix() = default;

    Matrix( Matrix&& matrix )      = default;
    Matrix( const Matrix& matrix ) = delete;

    Matrix& operator=( Matrix&& matrix )      = default;
    Matrix& operator=( const Matrix& matrix ) = delete;

    Matrix clone() const;

    Matrix& operator|=( const Matrix& other );
    Matrix operator|( const Matrix& other ) const;

    std::string toString() const;

    template <class Type>
    bool hasType() const;

    template <class... Types>
        requires( sizeof...( Types ) > 1 )
    bool hasAnyType() const;

    template <class... Types>
        requires( sizeof...( Types ) > 1 )
    bool hasSomeType() const;

    template <class Type>
    int nType();

    Dims getDims( int i ) const;

    template <class Type, int i = 0>
    Dims getDims() const;

    template <class Type>
    Size_t getOffset( int i ) const;

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> )
    Type get();

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> )
    Type get() const;

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
        requires( !std::is_pointer_v<Type> )
    Type get();

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
        requires( !std::is_pointer_v<Type> )
    Type get() const;

    Size_t nType() const;

    void setData( const Data_t* data, Size_t size );

    const Data_t* data() const;
    Data_t* data();
    Size_t size() const;

    // std::span getSpan() const;
    const auto& getData() const { return m_vector; }

    bool operator==( const Matrix& other ) const;

    void push_back( const Node& node );

    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.m_nodes, self.m_size, self.m_vector );
    }

    bool hasValue() const;

    void clear();

  protected:
  protected:
    std::vector<Node> m_nodes;
    Size_t m_size = 0;
    std::vector<Data_t> m_vector;
};

/////////////////////////////////////// MAKER /////////////////////////////////////////////////////

template <class Type, Size_t N = 1, Size_t... Ns>
    requires( !std::is_same_v<Type, Matrix> && N > 0 && ( ( Ns > 1 ) && ... ) )
Matrix make_matrix() {
    Matrix matrix;
    matrix.push_back( make_node<Type, N, Ns...>() );
    return matrix;
}

template <class... Matrices>
    requires( sizeof...( Matrices ) > 1 )
Matrix make_matrix( const Matrices&... matrices ) {
    Matrix matrix;
    matrix = ( matrices | ... );
    return matrix;
}

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

inline Matrix Matrix::clone() const {
    Matrix matrix;
    matrix |= *this;
    return matrix;
}

inline Matrix& Matrix::operator|=( const Matrix& other ) {
    size_t sizeBeforeAdd = m_size;
    for ( const auto& node : other.m_nodes ) {
        push_back( node );
    }
    if ( !other.m_vector.empty() ) {
        m_vector.resize( sizeBeforeAdd + other.m_vector.size() );
        std::copy( other.m_vector.begin(), other.m_vector.end(), m_vector.begin() + sizeBeforeAdd );
    }
    return *this;
}

inline Matrix Matrix::operator|( const Matrix& other ) const {
    Matrix matrix = this->clone();
    matrix |= other;
    return matrix;
}

inline std::string Matrix::toString() const {
    std::string str;
    for ( int i = 0; i < m_nodes.size(); ++i ) {
        const auto& node = m_nodes.at( i );
        str += node.toString();
        if ( i != m_nodes.size() - 1 ) str += "_";
    }

    if ( m_vector.empty() ) { str += "(" + std::to_string( m_size ) + ")"; }
    else {
        str += " = ";
        str += ::toString( m_vector );
    }
    return str;
}

inline Dims Matrix::getDims( int i ) const {
    return m_nodes.at( i ).m_dims;
}

inline Size_t Matrix::nType() const {
    return m_nodes.size();
}

inline void Matrix::setData( const Data_t* data, Size_t size ) {
    assert( size == m_size );
    if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( m_vector.size() == m_size );
    std::copy( data, data + size, m_vector.data() );
}

inline const Data_t* Matrix::data() const {
    assert( !m_vector.empty() );
    return m_vector.data();
}

inline Data_t* Matrix::data() {
    if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( !m_vector.empty() );
    return m_vector.data();
}

inline Size_t Matrix::size() const {
    return m_size;
}

inline bool Matrix::operator==( const Matrix& other ) const {
    return m_nodes == other.m_nodes && m_size == other.m_size && m_vector == other.m_vector;
}

inline void Matrix::push_back( const Node& node ) {
    m_nodes.push_back( node );
    m_size += node.size();
}

inline bool Matrix::hasValue() const {
    return !m_vector.empty();
}

template <class Type>
bool Matrix::hasType() const {
    for ( const auto& node : m_nodes ) {
        if ( node.m_hashCode == typeid( Type ).hash_code() ) { return true; }
    }
    return false;
}

template <class... Types>
    requires( sizeof...( Types ) > 1 )
bool Matrix::hasAnyType() const {
    return ( hasType<Types>() && ... );
}

template <class... Types>
    requires( sizeof...( Types ) > 1 )
bool Matrix::hasSomeType() const {
    return ( hasType<Types>() || ... );
}

template <class Type>
int Matrix::nType() {
    assert( hasType<Type>() );
    int ret             = 0;
    const auto typeHash = typeid( Type ).hash_code();
    for ( const auto& node : m_nodes ) {
        if ( node.m_hashCode == typeHash ) { ++ret; }
    }
    return ret;
}

template <class Type, int i>
Dims Matrix::getDims() const {
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
Size_t Matrix::getOffset( int i ) const {
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

template <class Type, int i, class RawType>
    requires( std::is_pointer_v<Type> )
Type Matrix::get() {
    if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( m_vector.data() + offset );
}

template <class Type, int i, class RawType>
    requires( std::is_pointer_v<Type> )
Type Matrix::get() const {
    // if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( m_vector.data() + offset );
}

template <class Type, int i, class RawType>
    requires( !std::is_pointer_v<Type> )
Type Matrix::get() {
    if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( *( m_vector.data() + offset ) );
}

template <class Type, int i, class RawType>
    requires( !std::is_pointer_v<Type> )
Type Matrix::get() const {
    // if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( m_vector.size() == m_size );
    // const auto offset = 0;
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( *( m_vector.data() + offset ) );
}

inline void Matrix::clear() {
    m_vector.clear();
}

} // namespace hub
