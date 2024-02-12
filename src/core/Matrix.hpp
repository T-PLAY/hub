/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09

#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Macros.hpp"
#include "MatrixBase.hpp"
#include "Node.hpp"
#include "Traits.hpp"

namespace hub {

class SRC_API Matrix
{
  public:
    static struct {
    } matrix;

    Matrix() = default;

    Matrix( Matrix&& matrix )      = default;
    Matrix( const Matrix& matrix ) = delete;

    Matrix& operator=( Matrix&& matrix ) = default;
    Matrix& operator=( const Matrix& matrix ) = delete;

    Matrix clone() const;

    Matrix& operator|=( const Matrix& other );
    Matrix operator|( const Matrix& other ) const;

    std::string toString( bool pretty = false ) const;

    template <class Type>
    bool hasType() const;

    template <class... Types>
    REQUIRES(, sizeof...( Types ) > 1, bool )
    hasAnyType() const;

    template <class... Types>
    REQUIRES(, sizeof...( Types ) > 1, bool )
    hasSomeType() const;

    template <class Type>
    int nType();

    Dims getDims( int i = 0 ) const;

    template <class Type, int i = 0>
    Dims getDims() const;

    template <class Type, int i = 0>
    Size_t getSize() const;

    template <class Type, int i = 0>
    Size_t getCapacity() const;

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    REQUIRES(, std::is_pointer_v<Type>, Type )
    get();

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    REQUIRES(, std::is_pointer_v<Type>, Type )
    get() const;

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    REQUIRES(, !std::is_pointer_v<Type>, Type )
    get();

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    REQUIRES(, !std::is_pointer_v<Type>, Type )
    get() const;

    template <int i = 0, class T, class... Ts>
    void fill_reduce( const T& t, const Ts&... ts ) {
        Data_t* data = getData( i );
        T& tIn       = reinterpret_cast<T&>( *(T*)data );
        tIn          = t;

        if constexpr ( sizeof...( ts ) > 0 ) { fill_reduce<i + 1>( ts... ); }
    }

    template <class... Ts>
    void fill( const Ts&... ts ) {
        fill_reduce<0>( ts... );
    }

    Size_t nType() const;

    void setData( const Data_t* data, Size_t size );

    const Data_t* data() const;
    Data_t* data();
    Size_t size() const;

    template <class Type>
    Size_t getOffset( int i = 0 ) const;

    Size_t getOffset( int i = 0 ) const;

    Data_t* getData( int i = 0 ) {
        assert( !m_vector.empty() );
        assert( m_vector.size() == m_size );

        const auto offset = getOffset( i );
        assert( 0 <= offset && offset < m_size );
        return m_vector.data() + offset;
    }

    const Data_t* getData( int i = 0 ) const {
        assert( !m_vector.empty() );
        assert( m_vector.size() == m_size );

        const auto offset = getOffset( i );
        assert( 0 <= offset && offset < m_size );
        return m_vector.data() + offset;
    }

    bool operator==( const Matrix& other ) const;
#if CPP_VERSION < 20
    bool operator!=( const Matrix& other ) const;
#endif

    void push_back( const Node& node );

#if CPP_VERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.m_nodes, self.m_size, self.m_vector );
    }
#endif
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_nodes, self.m_size, self.m_vector );
    }

    bool hasValue() const;

    void clear();
    void init();

    const std::vector<Node>& getNodes() const;

  protected:
  protected:
    std::vector<Node> m_nodes;
    Size_t m_size = 0;
    std::vector<Data_t> m_vector;
};

/////////////////////////////////////// MAKER /////////////////////////////////////////////////////

template <class Type, Size_t N = 1, Size_t... Ns>
REQUIRES(, !isMatrix<Type> && N > 0 && ( ( Ns > 1 ) && ... ), Matrix )
make_matrix() {
    Matrix matrix;
    matrix.push_back( make_node<Type, N, Ns...>() );
    return matrix;
}

template <class Type, class... Types>
void fill_matrix( Matrix& matrix ) {
    if constexpr ( isMatrix<Type> ) { matrix |= Type().getMatrix(); }
    else {
        matrix |= make_matrix<Type>();
    }
    if constexpr ( sizeof...( Types ) > 0 ) { fill_matrix<Types...>( matrix ); }
}

template <class... Types>
REQUIRES(, sizeof...( Types ) > 1, Matrix )
make_matrix() {
    Matrix matrix;
    fill_matrix<Types...>( matrix );
    return matrix;
}

template <class Type, class... Dims>
REQUIRES(, !isMatrix<Type> && sizeof...( Dims ) > 0, Matrix )
make_matrix( const Dims&... dims ) {
    Matrix matrix;
    matrix.push_back( make_node<Type>( dims... ) );
    return matrix;
}

template <class... Matrices>
REQUIRES(, areMatrices<Matrices...> && sizeof...( Matrices ) > 1, Matrix )
make_matrix( const Matrices&... matrices ) {
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
    if ( other.hasValue() ) {
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

inline std::string Matrix::toString( bool pretty ) const {
    std::string str;
    for ( int i = 0; i < m_nodes.size(); ++i ) {
        const auto& node = m_nodes.at( i );
        str += node.toString( pretty );
        if ( i != m_nodes.size() - 1 ) {
            if ( pretty )
                str += " _ ";
            else
                str += "_";
        }
    }

    if ( !pretty ) {
        if ( m_vector.empty() ) { str += "(" + std::to_string( m_size ) + ")"; }
        else {
            str += " = ";
            str += hub::to_string( m_vector );
        }
    }
    return str;
}

inline Size_t Matrix::nType() const {
    return m_nodes.size();
}

inline void Matrix::setData( const Data_t* data, Size_t size ) {
    assert( size == m_size );
    if ( m_vector.empty() ) m_vector.resize( m_size );
    assert( !m_vector.empty() );
    // Todo fix
    assert( m_vector.size() == m_size );
    std::copy( data, data + size, m_vector.data() );
}

inline const Data_t* Matrix::data() const {
    assert( !m_vector.empty() );
    return m_vector.data();
}

inline Data_t* Matrix::data() {
    assert( !m_vector.empty() );
    return m_vector.data();
}

inline Size_t Matrix::size() const {
    return m_size;
}

inline bool Matrix::operator==( const Matrix& other ) const {
    if ( hasValue() && other.hasValue() ) {
        return m_nodes == other.m_nodes && m_size == other.m_size && m_vector == other.m_vector;
    }
    else {
        return m_nodes == other.m_nodes && m_size == other.m_size;
    }
}

#if CPP_VERSION < 20
inline bool Matrix::operator!=( const Matrix& other ) const {
    return !( *this == other );
}
#endif

inline void Matrix::push_back( const Node& node ) {
    m_nodes.push_back( node );
    m_size += node.size();
}

inline bool Matrix::hasValue() const {
    return !m_vector.empty();
}

template <class Type>
bool Matrix::hasType() const {
    const auto typeId = TYPE_ID( Type );
    for ( const auto& node : m_nodes ) {
        if ( node.m_id == typeId ) { return true; }
    }
    return false;
}

template <class... Types>
REQUIRES(, sizeof...( Types ) > 1, bool )
Matrix::hasAnyType() const {
    return ( hasType<Types>() && ... );
}

template <class... Types>
REQUIRES(, sizeof...( Types ) > 1, bool )
Matrix::hasSomeType() const {
    return ( hasType<Types>() || ... );
}

template <class Type>
int Matrix::nType() {
    assert( hasType<Type>() );
    int ret           = 0;
    const auto typeId = TYPE_ID( Type );
    for ( const auto& node : m_nodes ) {
        if ( node.m_id == typeId ) { ++ret; }
    }
    return ret;
}

inline Dims Matrix::getDims( int i ) const {
    return m_nodes.at( i ).m_dims;
}

template <class Type, int i>
Dims Matrix::getDims() const {
    int nFound        = 0;
    const auto typeId = TYPE_ID( Type );
    for ( const auto& node : m_nodes ) {
        if ( node.m_id == typeId ) {
            if ( i == nFound ) { return node.m_dims; }
            ++nFound;
        }
    }
    assert( false );
    return {};
}

template <class Type, int i>
Size_t Matrix::getSize() const {
    int nFound        = 0;
    const auto typeId = TYPE_ID( Type );
    for ( const auto& node : m_nodes ) {
        if ( node.m_id == typeId ) {
            if ( i == nFound ) { return node.m_size; }
            ++nFound;
        }
    }
    assert( false );
    return {};
}

template <class Type, int i>
Size_t Matrix::getCapacity() const {
    int nFound        = 0;
    const auto typeId = TYPE_ID( Type );
    for ( const auto& node : m_nodes ) {
        if ( node.m_id == typeId ) {
            if ( i == nFound ) {
                Size_t capacity = 1;
                for ( auto dim : node.m_dims ) {
                    capacity *= dim;
                }
                return capacity;
            }
            ++nFound;
        }
    }
    assert( false );
    return {};
}

inline Size_t Matrix::getOffset( int i ) const {
    assert( i < m_nodes.size() );

    Size_t offset = 0;
    auto it       = m_nodes.begin();
    int iNode     = 0;
    while ( iNode < i ) {
        assert( it != m_nodes.end() );
        const auto& node = *it;
        offset += node.m_size;
        ++it;
        ++iNode;
    }
    return offset;
}

template <class Type>
Size_t Matrix::getOffset( int i ) const {
    const auto typeId = TYPE_ID( Type );

    assert( hasType<Type>() );
    Size_t offset = 0;
    int cptFound  = 0;

    for ( const auto& node : m_nodes ) {
        if ( node.m_id == typeId ) {
            if ( cptFound == i ) { return offset; }
            ++cptFound;
        }
        offset += node.m_size;
    }

    return offset;
}

template <class Type, int i, class RawType>
REQUIRES(, std::is_pointer_v<Type>, Type )
Matrix::get() {
    assert( !m_vector.empty() );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( m_vector.data() + offset );
}

template <class Type, int i, class RawType>
REQUIRES(, std::is_pointer_v<Type>, Type )
Matrix::get() const {
    assert( !m_vector.empty() );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( m_vector.data() + offset );
}

template <class Type, int i, class RawType>
REQUIRES(, !std::is_pointer_v<Type>, Type )
Matrix::get() {
    assert( !m_vector.empty() );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( *( m_vector.data() + offset ) );
}

template <class Type, int i, class RawType>
REQUIRES(, !std::is_pointer_v<Type>, Type )
Matrix::get() const {
    assert( !m_vector.empty() );
    assert( m_vector.size() == m_size );
    const auto offset = getOffset<RawType>( i );
    assert( 0 <= offset && offset < m_size );
    return reinterpret_cast<Type>( *( m_vector.data() + offset ) );
}

inline void Matrix::clear() {
    m_vector.clear();
}

inline void Matrix::init() {
    assert( m_size > 0 );
    m_vector.resize( m_size );
}

inline const std::vector<Node>& Matrix::getNodes() const {
    return m_nodes;
}

} // namespace hub
