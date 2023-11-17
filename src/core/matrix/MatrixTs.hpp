
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
    static constexpr auto Capacity = 1;
    //    static constexpr auto Size     = ( sizeof( Types ) + ... );
    static constexpr auto Size = sizeof_<Types...>();
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };
    //    static constexpr auto types() { return {Types...}; };
    //    static_assert(size() < MAX_STACK_SIZE);
    //    static_assert( size() <= MAX_STACK_SIZE, "Stack size reached, please use static memory."
    //    );

    static constexpr auto nType() { return sizeof...( Types ); };
    static constexpr auto nDim() { return 1; };
    template <Size_t i>
        requires( i == 0 )
    static constexpr auto getDim() {
        return 1;
    }

    const Data_t* data() const { return m_buffer.data(); }
    //    Size_t size() const {
    //        return m_buffer.size();
    //    }

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
        static_assert( hasType<Type>() );
        return ( nType<Type, Types>() + ... );
    }

    template <Size_t i>
    using getType = typename std::tuple_element<i, std::tuple<Types...>>::type;

  public:
    //    constexpr MatrixTs() : m_data { 0 } {}
    template <class... Args>
    constexpr MatrixTs( Args&&... args ) : m_buffer { std::forward<Data_t&&>( args )... } {}

    //    MatrixTs(MatrixTs &&) = default;
    //    MatrixTs(const MatrixTs &) = delete;

  public:
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

    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    Type get() {
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        //        return reinterpret_cast<Type>( m_buffer.data() + offset );
        return (Type)( m_buffer.data() + offset );
    }
    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    Type get() const {
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        //        return reinterpret_cast<Type>( m_buffer.data() + offset );
        return (Type)( m_buffer.data() + offset );
    }

    //    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    //        requires( std::is_pointer_v<Type> && hasType<RawType>() && i < nType<RawType>() )
    //    const Type get() const {
    //        const auto offset = getOffset<i, 0, RawType, Types...>();
    //        static_assert( 0 <= offset && offset < Size );
    //        return reinterpret_cast<Type>( m_buffer.data() + offset );
    //    }

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

    //    constexpr bool operator==( const MatrixTs& matrix ) const { return m_data ==
    //    matrix.m_data; }

    static constexpr std::string name() { return printName<Types...>(); }

    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    template <class Type, int i = 0>
        requires( hasType<Type>() && i < nType<Type>() )
    static constexpr Size_t getOffset() {
        return getOffset<i, 0, Type, Types...>();
    }

    //    Matrix::Node getNode() const {
    //                Matrix::Node node;
    //                node.m_hashCode = typeid(Type).hash_code();
    //                node.m_dims = std::vector<int>{Ns...};
    //                node.m_name = TYPE_NAME(Type);
    //                node.m_size = Size;
    //                matrix.m_nodes.push_back(std::move(node));
    //                matrix.m_size = Size;
    //    }

    template <class Type_, class... Types_>
        requires( !isMatrix<Type_> )
    void serialize( Matrix& matrix ) const {
        //        Matrix::Node node;
        //        node.m_hashCode = typeid(Type_).hash_code();
        //        node.m_dims = std::vector<int>{1};
        //        node.m_name = TYPE_NAME(Type_);
        //        node.m_size = sizeof(Type_);
        //        auto node = Matrix::Node({1});
        auto matrix2 = make_matrix<Type_>();
        //        matrix.push_back( std::move( matrix2 ) );
        matrix << matrix2;

        if constexpr ( sizeof...( Types_ ) > 0 ) { serialize<Types_...>( matrix ); }
    }

    template <class Matrix, class... Types_>
        requires( isMatrix<Matrix> )
    void serialize( Matrix& matrix ) const {
        Matrix::serialize( matrix );

        if constexpr ( sizeof...( Types_ ) > 0 ) { serialize<Types_...>( matrix ); }
    }

    void serialize( Matrix& matrix ) const {
        //            Matrix::serialize( matrix );
        //            if ( sizeof...( Types_ ) > 0 )
        serialize<Types...>( matrix );
    }

    Matrix getMatrix() const {
        Matrix matrix;
        serialize( matrix );
        //        serialize<Types...>( matrix );
        return matrix;
    }

    bool operator==( const Matrix& matrix ) {
        if (Size == matrix.size() && nType() == matrix.nType()) {
            const Matrix & me = getMatrix();
            if (me == matrix) {
                return memcmp(data(), matrix.data(), Size) == 0;
            }
            else {
                return false;
            }
        }
            return false;
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

    template <class Type_, class... Types_>
    static constexpr auto printName() {
        std::string str;
        str += TYPE_NAME( Type_ );
        str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );

        if constexpr ( sizeof...( Types_ ) > 0 ) { return str + "_" + printName<Types_...>(); }
        else { return str + "(" + std::to_string( Size ) + ")"; }
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
