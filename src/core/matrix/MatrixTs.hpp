/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09

#pragma once

#include <iostream>

#include "core/Matrix.hpp"
#include "core/MatrixBase.hpp"

namespace hub {

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

///
/// \brief The MatrixTs class
///
template <class... Types>
#if CPP_VERSION >= 20
requires( sizeof...( Types ) > 1 )
#endif
    class MatrixTs
{
    static_assert( sizeof...( Types ) > 1 );

  public:
    static struct {
    } matrix;
    static constexpr auto Capacity = 1;
    static constexpr auto Size     = sizeOf<Types...>();
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };

    static constexpr auto nType() { return sizeof...( Types ); };
    static constexpr auto nDim() { return 1; };
    template <Size_t i>
#if CPP_VERSION >= 20
    requires( i == 0 )
#endif
        static constexpr auto getDim() {
        static_assert( i == 0 );
        return 1;
    }

  private:
    Buffer<Data_t, Size> m_buffer;

  public:
    const Data_t* data() const { return m_buffer.data(); }
    Data_t* data() { return m_buffer.data(); };

    template <class Type>
    static constexpr auto hasType() {
        return ( isSame<Type, Types>() || ... );
    }

    template <class... Types_>
    REQUIRES( static constexpr, sizeof...( Types_ ) > 1, bool )
    hasType() {
        return ( hasType<Types_>() && ... );
    }

    template <class Type>
    static constexpr int nType() {
        return ( nType<Type, Types>() + ... );
    }

    template <class Type, class Matrix, class... Types_>
    REQUIRES( static constexpr, isMatrix<Matrix>, int )
    nType() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return Matrix::template nType<Type>() + ( nType<Type, Types_>() + ... );
        }
        else {
            return Matrix::template nType<Type>();
        }
    }

    template <class Type, class Type2, class... Types_>
    REQUIRES( static constexpr, !isMatrix<Type2>, int )
    nType() {
        if constexpr ( sizeof...( Types_ ) ) {
            return std::is_same_v<Type, Type2> + nType<Type, Types_...>();
        }
        else {
            return std::is_same_v<Type, Type2>;
        }
    }

    template <Size_t i>
    using getType = typename std::tuple_element<i, std::tuple<Types...>>::type;

    template <class... Args>
    constexpr MatrixTs( Args&&... args ) : m_buffer { std::forward<Data_t&&>( args )... } {}

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
//    REQUIRES(, (std::is_pointer_v<Type>&& hasType<RawType>() && i < nType<RawType>()), Type )
//    typename std::enable_if_t<(std::is_pointer_v<Type>&& hasType<RawType>() && i < nType<RawType>()), Type >
    typename std::enable_if_t<(std::is_pointer_v<Type>), Type >
    get() {
        static_assert(hasType<RawType>() && i < nType<RawType>());
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return ( Type )( m_buffer.data() + offset );
    }

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
//    REQUIRES(, std::is_pointer_v<Type>&& hasType<RawType>() && i < nType<RawType>(), Type )
    REQUIRES(, std::is_pointer_v<Type>, Type )
    get() const {
        static_assert(hasType<RawType>() && i < nType<RawType>());
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return ( Type )( m_buffer.data() + offset );
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
//    REQUIRES(, (!std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>()), Type )
//    typename std::enable_if_t<(!std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>()), Type>
    typename std::enable_if_t<(!std::is_pointer_v<Type>), Type>
//    typename std::enable_if_t<!std::is_pointer_v<Type> && hasType<RawType>(), Type>
    get() {
        static_assert(hasType<RawType>() && i < nType<RawType>());
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
//    REQUIRES(, !std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>(), Type )
//    REQUIRES(, !std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>(), Type )
    REQUIRES(, !std::is_pointer_v<Type>, Type )
    get() const {
        static_assert(hasType<RawType>() && i < nType<RawType>());
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
    }

    static CONSTEXPR20 std::string name() { return printName<Types...>(); }

    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    template <class Type, int i = 0>
    REQUIRES( static constexpr, hasType<Type>() && i < nType<Type>(), Size_t )
    getOffset() {
        //std::cout << "[MatrixTs] getOffset(" << TYPE_NAME( Type() ) << " " << i << std::endl;
        return getOffset<i, 0, Type, Types...>();
    }

    void serialize( Matrix& matrix ) const { serialize_<Types...>( matrix ); }

    Matrix getMatrix() const {
        Matrix matrix;
        serialize( matrix );
        matrix.setData( m_buffer.data(), m_buffer.size() );
        return matrix;
    }

    bool operator==( const Matrix& matrix ) const { return getMatrix() == matrix; }

 // private:
    template <class Type_, class... Types_>
    void serialize_( Matrix& matrix ) const {
        // Matrix::serialize_( matrix );
        if constexpr ( isMatrix<Type_> ) { matrix |= Type_().getMatrix(); }
        else {
            matrix |= make_matrix<Type_>();
        }

        if constexpr ( sizeof...( Types_ ) > 0 ) { serialize_<Types_...>( matrix ); }
    }

    template <class Type, class Matrix, class... Types_>
    REQUIRES( static constexpr, isMatrix<Matrix>, bool )
    isSame() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return Matrix::template hasType<Type>() || isSame<Type, Types_...>();
        }
        else {
            return Matrix::template hasType<Type>();
        }
    }

    template <class Type, class Type_, class... Types_>
    REQUIRES( static constexpr, !isMatrix<Type_>, bool )
    isSame() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return std::is_same<Type, Type_>() || isSame<Type, Types_...>();
        }
        else {
            return std::is_same<Type, Type_>();
        }
    }

    template <int ith, int i, class targetType, class Matrix, class... Types_>
    REQUIRES( static constexpr, isMatrix<Matrix>, Size_t )
    getOffset() {
        //std::cout << "[MatrixTs] getOffset(Matrix)" << std::endl;
        if constexpr ( Matrix::template hasType<targetType>() ) {
            if ( ith == i ) { 
                return Matrix::template getOffset<targetType>();
                //return 0; 
            }
            else {
                if constexpr ( sizeof...( Types_ ) > 0 ) {
                    return Matrix::Size + getOffset<ith, i + 1, targetType, Types_...>();
                }
                else {
                    return Matrix::Size;
                }
            }
        }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return Matrix::Size + getOffset<ith, i, targetType, Types_...>();
            }
            else {
                return Matrix::Size;
            }
        }
    }

    template <int ith, int i, class targetType, class Type_, class... Types_>
    REQUIRES( static constexpr, !isMatrix<Type_>, Size_t )
    getOffset() {
        //std::cout << "[MatrixTs] getOffset(! Matrix)" << std::endl;
        if constexpr ( std::is_same_v<targetType, Type_> ) {
            if ( ith == i ) { return 0; }
            else {
                if constexpr ( sizeof...( Types_ ) > 0 ) {
                    return sizeOf<Type_>() + getOffset<ith, i + 1, targetType, Types_...>();
                }
                else {
                    return sizeOf<Type_>();
                }
            }
        }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return sizeOf<Type_>() + getOffset<ith, i, targetType, Types_...>();
            }
            else {
                return sizeOf<Type_>();
            }
        }
    }

    template <class Type_, class... Types_>
    static CONSTEXPR20 auto printName() {
        std::string str;
        str += TYPE_NAME( Type_() );
        str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );

        if constexpr ( sizeof...( Types_ ) > 0 ) { return str + "_" + printName<Types_...>(); }
        else {
            return str;
        }
    }
};
static_assert( isMatrix<MatrixTs<int, double>> );

} // namespace hub
