
#pragma once

#include "MatrixBase.hpp"
#include "core/Matrix.hpp"

namespace hub {

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

template <class... Types>
    requires( sizeof...( Types ) > 1 )
class MatrixTs
{
  public:
    static struct {} matrix;
    static constexpr auto Capacity = 1;
    static constexpr auto Size     = sizeof_<Types...>();
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };

    static constexpr auto nType() { return sizeof...( Types ); };
    static constexpr auto nDim() { return 1; };
    template <Size_t i>
        requires( i == 0 )
    static constexpr auto getDim() {
        return 1;
    }

    const Data_t* data() const { return m_buffer.data(); }
    Data_t* data() { return m_buffer.data(); };

    template <class Type>
    static constexpr auto hasType() {
        return ( isSame<Type, Types>() || ... );
    }

    template <class... Types_>
        requires( sizeof...( Types_ ) > 1 )
    static constexpr auto hasType() {
        return ( hasType<Types_>() && ... );
    }

    template <class Type>
    static constexpr int nType() {
        // static_assert( hasType<Type>() );
        return ( nType<Type, Types>() + ... );
    }

    template <Size_t i>
    using getType = typename std::tuple_element<i, std::tuple<Types...>>::type;

    template <class... Args>
    constexpr MatrixTs( Args&&... args ) : m_buffer { std::forward<Data_t&&>( args )... } {}

    template <class Type, class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    static constexpr int nType() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return Matrix::template nType<Type>() + ( nType<Type, Types_>() + ... );
        }
        else { return Matrix::template nType<Type>(); }
    }

    template <class Type, class Type2, class... Types_>
    static constexpr int nType() {
        if constexpr ( sizeof...( Types_ ) ) {
            return std::is_same_v<Type, Type2> + nType<Type, Types_...>();
        }
        else { return std::is_same_v<Type, Type2>; }
    }

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    Type get() {
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return (Type)( m_buffer.data() + offset );
    }
    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    Type get() const {
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return (Type)( m_buffer.data() + offset );
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
        requires( !std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    Type get() {
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
        requires( !std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    Type get() const {
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
        //        return (Type)( *( m_buffer.data() + offset ) );
    }

    static constexpr std::string name() { return printName<Types...>(); }

    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    template <class Type, int i = 0>
        requires( hasType<Type>() && i < nType<Type>() )
    static constexpr Size_t getOffset() {
        return getOffset<i, 0, Type, Types...>();
    }

    template <class Type_, class... Types_>
        requires( !isMatrix<Type_> )
    void serialize( Matrix& matrix ) const {
        auto matrix2 = make_matrix<Type_>();
        matrix |= matrix2;
        if constexpr ( sizeof...( Types_ ) > 0 ) { serialize<Types_...>( matrix ); }
    }

    template <class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    void serialize( Matrix& matrix ) const {
        Matrix::serialize( matrix );

        if constexpr ( sizeof...( Types_ ) > 0 ) { serialize<Types_...>( matrix ); }
    }

    void serialize( Matrix& matrix ) const { serialize<Types...>( matrix ); }

    Matrix getMatrix() const {
        Matrix matrix;
        serialize( matrix );
        matrix.setData( m_buffer.data(), m_buffer.size() );
        return matrix;
    }

    bool operator==( const Matrix& matrix ) {
        if ( Size == matrix.size() && nType() == matrix.nType() ) {
            const Matrix& myselfAsMatrix = getMatrix();
            return myselfAsMatrix == matrix;
        }
        return false;
    }

  private:
    template <class Type, class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    static constexpr auto isSame() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return Matrix::template hasType<Type>() || isSame<Type, Types_...>();
        }
        else { return Matrix::template hasType<Type>(); }
    }

    template <class Type, class Type_, class... Types_>
        requires( !isMatrix<Type_> )
    static constexpr auto isSame() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return std::is_same<Type, Type_>() || isSame<Type, Types_...>();
        }
        else { return std::is_same<Type, Type_>(); }
    }

    template <int ith, int i, class targetType, class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    static constexpr Size_t getOffset() {
        if constexpr ( Matrix::template hasType<targetType>() ) {
            if ( ith == i ) { return 0; }
            else {
                if constexpr ( sizeof...( Types_ ) > 0 ) {
                    return Matrix::Size + getOffset<ith, i + 1, targetType, Types_...>();
                }
                else { return Matrix::Size; }
            }
        }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return Matrix::Size + getOffset<ith, i, targetType, Types_...>();
            }
            else { return Matrix::Size; }
        }
    }

    template <int ith, int i, class targetType, class Type_, class... Types_>
        requires( !isMatrix<Type_> )
    static constexpr Size_t getOffset() {
        if constexpr ( std::is_same_v<targetType, Type_> ) {
            if ( ith == i ) { return 0; }
            else {
                if constexpr ( sizeof...( Types_ ) > 0 ) {
                    return sizeof_<Type_>() + getOffset<ith, i + 1, targetType, Types_...>();
                }
                else { return sizeof_<Type_>(); }
            }
        }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return sizeof_<Type_>() + getOffset<ith, i, targetType, Types_...>();
            }
            else { return sizeof_<Type_>(); }
        }
    }

    template <class Type_, class... Types_>
    static constexpr auto printName() {
        std::string str;
        str += TYPE_NAME( Type_ );
        str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );

        if constexpr ( sizeof...( Types_ ) > 0 ) { return str + "_" + printName<Types_...>(); }
        else { return str; }
    }

  private:
    Buffer<Data_t, Size> m_buffer;
};
static_assert( sizeof( MatrixTs<int, double, float> ) ==
               sizeof( int ) + sizeof( double ) + sizeof( float ) + 8 );
static_assert( isMatrix<MatrixTs<int, double>> );

} // namespace hub
