
#pragma once

#include <type_traits>

#include "core/MatrixBase.hpp"
#include "core/Matrix.hpp"

namespace hub {

/////////////////////////////////////////// TEMPLATES
/////////////////////////////////////////////////////

namespace _ {

template <class Type, Size_t... Ns>
#if CPP_VERSION >= 20
    requires( sizeof...( Ns ) > 0 && ( ( Ns > 1 ) && ... ) )
#endif
class MatrixXDBase
{
  public:
    static struct {} matrix;
    static constexpr auto Capacity = ( Ns * ... );
    static constexpr auto Size     = sizeOf<Type>() * Capacity;
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };

    static constexpr auto nType() { return 1; };
    static constexpr auto nDim() { return sizeof...( Ns ); };
    template <Size_t i>
#if CPP_VERSION >= 20
        requires( 0 <= i && i < nDim() )
#endif
    static constexpr auto getDim() {
        auto j = 0;
        for ( auto dim : { Ns... } ) {
            if ( j == i ) return dim;
            ++j;
        }
        return (Size_t)0;
    }
    const Data_t* data() const { return m_buffer.data(); }
    template <class Type_>
    static constexpr auto hasType() {
        return std::is_same<Type, Type_>();
    }

    template <class... Types>
#if CPP_VERSION >= 20
        requires( sizeof...( Types ) > 1 )
#endif
    static constexpr auto hasType() {
        return ( hasType<Types>() && ... );
    }

    template <class Type_>
    static constexpr int nType() {
        return std::is_same_v<Type, Type_>;
    }

    template <Size_t ith>
#if CPP_VERSION >= 20
        requires( ith == 0 )
#endif
    using getType = Type;

  public:
    template <class... Args>
    constexpr MatrixXDBase( Args&&... args ) : m_buffer { std::forward<Type&&>( args )... } {}

    // static constexpr std::string name() {
    static CONSTEXPR20 auto name() {
        std::string str;
        // str += TYPE_NAME( Type );
        str += TYPE_NAME( Type() );

        if ( !( nDim() == 1 && getDim<0>() == 1 ) ) {
            str += ":";
            Size_t i = 0;
            for ( auto dim : { Ns... } ) {
                str += std::to_string( dim );
                if ( i != nDim() - 1 ) str += "x";
                ++i;
            }
        }
        return str;
    }

    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    template <int i = 0>
    const Type& get() {
        static_assert( 0 <= i && i < Capacity );
        // return reinterpret_cast<const Type&>( *( m_buffer.data() + i ) );
        return reinterpret_cast<const Type&>( *( m_buffer.data() + i * sizeof(Type) ) );
    }

    void serialize( Matrix& matrix ) const {
        assert(! matrix.hasValue());
        auto matrix2 = make_matrix<Type, Ns...>();
        matrix |= matrix2;
    }

    Matrix getMatrix() const {
        Matrix matrix;
        serialize( matrix );
        matrix.setData( m_buffer.data(), m_buffer.size() );
        // matrix.setData( m_buffer.data(), Size );
        return matrix;
    }

    bool operator==( const Matrix& matrix ) {
        return getMatrix() == matrix;
    }

  private:
    // Buffer<Type, Capacity> m_buffer;
    Buffer<Data_t, Size> m_buffer;
};
static_assert( sizeof( MatrixXDBase<int, 2, 3, 4, 5, 6> ) ==
               sizeof( int ) * 2 * 3 * 4 * 5 * 6 + 8 );
static_assert( isMatrix<MatrixXDBase<int, 2>> );

} // namespace _

template <class Type, Size_t... Ns>
#if CPP_VERSION >= 20
    requires( sizeof...( Ns ) > 0 && ( ( Ns > 1 ) && ... ) )
#endif
class MatrixXD : public _::MatrixXDBase<Type, Ns...>
{
  public:
    template <int i>
    static constexpr Size_t n() {
        static_assert( 0 <= i && i < _::MatrixXDBase<Type, Ns...>::nDim() );
        int j = 0;
        for ( const auto& n : { Ns... } ) {
            if ( i == j ) return n;
            ++j;
        }
        return 0;
    }
};

template <class Type, Size_t N>
#if CPP_VERSION >= 20
    requires( N > 1 )
#endif
class MatrixXD<Type, N> : public _::MatrixXDBase<Type, N>
{
  public:
    static constexpr auto length() { return N; }
};

template <class Type, Size_t N, Size_t N2>
class MatrixXD<Type, N, N2> : public _::MatrixXDBase<Type, N, N2>
{
  public:
    static constexpr Size_t width() { return N; }
    static constexpr Size_t height() { return N2; }
};

template <class Type, Size_t N, Size_t N2, Size_t N3>
class MatrixXD<Type, N, N2, N3> : public _::MatrixXDBase<Type, N, N2, N3>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
};

template <class Type, Size_t N, Size_t N2, Size_t N3, Size_t N4>
class MatrixXD<Type, N, N2, N3, N4> : public _::MatrixXDBase<Type, N, N2, N3, N4>
{
  public:
    static constexpr Size_t nx() { return N; }
    static constexpr Size_t ny() { return N2; }
    static constexpr Size_t nz() { return N3; }
    static constexpr Size_t nt() { return N4; }
};

template <class Type, Size_t N>
using Matrix1D = MatrixXD<Type, N>;
template <class Type, Size_t N>
using Vector = MatrixXD<Type, N>;

template <class Type, Size_t N, Size_t N2>
using Matrix2D = MatrixXD<Type, N, N2>;
template <class Type, Size_t N, Size_t N2>
using Image = MatrixXD<Type, N, N2>;

template <class Type, Size_t N, Size_t N2, Size_t N3>
using Matrix3D = MatrixXD<Type, N, N2, N3>;
template <class Type, Size_t N, Size_t N2, Size_t N3>
using Volume = MatrixXD<Type, N, N2, N3>;

template <class Type, Size_t N, Size_t N2, Size_t N3, Size_t N4>
using Matrix4D = MatrixXD<Type, N, N2, N3, N4>;

} // namespace hub
