
#pragma once

#include "MatrixBase.hpp"

namespace hub {

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

template <class... Types>
    requires( sizeof...( Types ) > 1 )
class MatrixTs
{
  public:
    //    using getTypes = Types;
    static constexpr auto Capacity = 1;
    //    static constexpr auto Size     = ( sizeof( Types ) + ... );
    static constexpr auto Size = sizeof_<Types...>();
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };
    //    static constexpr auto types() { return {Types...}; };
    //    static_assert(size() < MAX_STACK_SIZE);
    static_assert( size() <= MAX_STACK_SIZE, "Stack size reached, please use static memory." );

    static constexpr auto nType() { return sizeof...( Types ); };
    static constexpr auto nDim() { return 1; };
    template <Size_t ith>
        requires( ith == 0 )
    static constexpr auto getDim() {
        return 1;
    }
    //    using Tuple = std::tuple<Types...>;
    //    using getType = typename std::tuple_element<ith, Tuple>::type;
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
        return ( nType<Type, Types>() + ... );
    }

    template <Size_t ith>
    using getType = typename std::tuple_element<ith, std::tuple<Types...>>::type;

  public:
    //    using Data = std::array<Data_t, size()>;

    //    constexpr MatrixTs() : m_data { 0 } {}

    //    constexpr MatrixTs( Types... types ) {
    //        Size_t offset = 0;
    //        for ( auto type : { types... } ) {
    //            //            auto * data = (Data_t*)&type;
    //            //            std::copy(data, data + sizeof(type), m_data.data() + offset);
    //            //		    std::copy( span.begin(), span.end(), m_data.begin() );
    //            //            offset += sizeof(type);
    //        }
    //    }

  public:
    //    template <Size_t i, class Type_, std::size_t Size_>
    //        requires( size() == Size_ )
    //    void setData( const std::span<Type_, Size_>& span ) {
    //        std::copy( span.begin(), span.end(), m_data.begin() );
    //    }

    //    template <Size_t ith>
    //    constexpr auto& getData() {
    //        const auto offset = getOffset<ith, 0, Types...>();
    //        static_assert( 0 <= offset && offset < size() );
    //        using Type = getType<ith>;

    //        return reinterpret_cast<Type&>( *( m_data.begin() + offset ) );
    //    }
    template <class Type, class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    static constexpr int nType() {
        if constexpr ( sizeof...( Types_ ) ) {
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

    //    template <class Type>
    template <class Type, int ith = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> && hasType<RawType>() && ith < nType<RawType>() )
    Type get() {
        const auto offset = getOffset<ith, 0, RawType, Types...>();
        //        const auto offset = 0;
        // todo
        static_assert( 0 <= offset && offset < Size );
        //        return reinterpret_cast<Type>( m_data.ptr() + offset );
        return reinterpret_cast<Type>( m_buffer.data() + offset );
    }
    //    template <class Type>
    template <class Type, int ith = 0, class RawType = std::remove_cvref_t<Type>>
        requires( !std::is_pointer_v<Type> && hasType<RawType>() && ith < nType<RawType>() )
    Type get() {
        const auto offset = getOffset<ith, 0, RawType, Types...>();
        //        const auto offset = 0;
        // todo
        static_assert( 0 <= offset && offset < Size );
        //        return reinterpret_cast<Type>( m_data.ptr() + offset );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
    }

    //    constexpr bool operator==( const MatrixTs& matrix ) const { return m_data ==
    //    matrix.m_data; }

    static constexpr std::string name() { return printName<Types...>(); }

    //    static constexpr auto toString() { return name() + " = " + ::toString(m_data); };
    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    //    template <class... Types_>
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const MatrixTs<Types_...>&
    //    matrix );

    template <class Type, int ith = 0>
        requires( hasType<Type>() && ith < nType<Type>() )
    static constexpr Size_t getOffset() {
        return getOffset<ith, 0, Type, Types...>();
    }

  private:
    template <class Type, class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    static constexpr auto isSame() {
        //        return ( std::is_same<Type, Types>() || ... );
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            //            return std::is_same<Type, Type_>() || isSame<Type, Types_...>();
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
                //                return Matrix::template getOffset<ith, i + 1, targetType,
                //                Types_...>();
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
            else {
                return sizeof_<Type_>();
                //                return 0;
            }
        }
    }

    //    template <const Size_t ith, Size_t i = 0, class Type_, class... Types_>
    //    static constexpr auto getOffset() {
    //        if constexpr ( ith == i ) { return 0; }
    //        else {
    //            if constexpr ( sizeof...( Types_ ) > 0 ) {
    //                return sizeof( Type_ ) + getOffset<ith, i + 1, Types_...>();
    //            }
    //            else { return sizeof( Type_ ); }
    //        }
    //    }

    template <class Type_, class... Types_>
    static constexpr auto printName() {
        std::string str;
        //        using type = Type_();
        //        if constexpr ( requires { Type_::name(); } ) { str += Type_::name(); }
        //        else { str += TYPE_NAME( Type_ ); }
        str += TYPE_NAME( Type_ );

        //        std::replace(str.begin(), str.end(), ' ', '_');
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

// template <class... Types>
// SRC_API std::ostream& operator<<( std::ostream& os, const MatrixTs<Types...>& matrix ) {
//     //    os << "(constexpr)";
//     os << matrix.toString();
////    ::operator<<( os, matrix.m_data );
//    //    ::operator<<( os, matrix.Tuple() );
//    //    ::operator<<( os, typename MatrixTs<Types...>::Tuple() );
//    //    os << ", size: " << buffer.size();
//    return os;
//}

} // namespace hub
