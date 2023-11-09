
#pragma once

#include "MatrixBase.hpp"

namespace hub {

namespace _ {

template <class Type, Size_t... Ns>
    requires(sizeof...(Ns) > 0 && ((Ns > 1) && ...))
class MatrixXDBase
{
  public:
    static constexpr auto Capacity = ( Ns * ... );
//    static constexpr auto Size     = sizeof( Type ) * Capacity;
    static constexpr auto Size     = sizeof_<Type>() * Capacity;
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };
//    static_assert(size() < MAX_STACK_SIZE);
//    static_assert(size() <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");

    static constexpr auto nType() { return 1; };
    static constexpr auto nDim() { return sizeof...( Ns ); };
    template <Size_t ith>
        requires( 0 <= ith && ith < nDim() )
    static constexpr auto getDim() {
        auto i = 0;
        for ( auto dim : { Ns... } ) {
            if ( i == ith ) return dim;
            ++i;
        }
        return (Size_t)0;
    }
    template <class Type_>
    static constexpr auto hasType() {
        return  std::is_same<Type, Type_>();
    }

    template <class... Types>
        requires( sizeof...( Types ) > 1 )
    static constexpr auto hasType() {
        return ( hasType<Types>() && ... );
    }

    template <Size_t ith>
        requires( ith == 0 )
    using getType = Type;

  public:
//    using Data = std::array<Data_t, Size>;

//    constexpr MatrixXDBase() : m_data { 0 } {}

//    template <class Type_, std::size_t Size_>
//        requires( Size == Size_ )
//    void setData( const std::span<Type_, Size_>& span ) {
//        std::copy( span.begin(), span.end(), m_data.begin() );
//    }

//    template <class Type_, std::size_t Size_>
//        requires( Size == Size_ )
//    std::span<Type_, Size_> getData( ) {
//    const auto & getData() const {
//        return m_data;
//    }
//    auto getData( ) const {
//        return std::span<Data_t, size()>{m_data};
//        std::copy( m_data.begin(), m_data.end(), span.begin() );
//    }

    static constexpr std::string name() {
        std::string str;
//        if constexpr ( requires { Type::name(); } ) { str += Type::name(); }
//        else { str += TYPE_NAME( Type ); }
        str += TYPE_NAME(Type);

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

    constexpr auto toString() const {
        return name() + " = " + m_data.toString();
    }

//    template <class Type_, Size_t... Ns_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const MatrixXDBase<Type, Ns...>& matrix );

//    constexpr bool operator==( const MatrixXDBase& matrix ) const { return m_data == matrix.m_data; }

  private:
//    Data<Size, DataOption::StaticMemory> m_data;
    Data<Size> m_data;
//    Data<
//    Data m_data;
};
//static_assert(sizeof(MatrixXDBase<int, 2, 3, 4, 5, 6>) == 1);
static_assert(sizeof(MatrixXDBase<int, 2, 3, 4, 5, 6>) == sizeof(int) * 2 * 3 * 4 * 5 * 6 + 8);

//template <class Type, Size_t... Ns>
//SRC_API std::ostream& operator<<( std::ostream& os, const MatrixXDBase<Type, Ns...>& matrix ) {
//    //    os << matrix.m_data;
////    os << matrix.name();
//    os << matrix.toString();
////    matrix.getData();
////    ::operator<<( os, matrix.m_data );
////    ::operator<<( os, matrix.getData() );
//    return os;
//}

} // namespace _

template <class Type, Size_t... Ns>
    requires(sizeof...(Ns) > 0 && ((Ns > 1) && ...))
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
//static_assert(sizeof(MatrixXD<int, 2, 3, 4, 5, 6>) == 1);

//template <class Type>
//class MatrixXD<Type> : public _::MatrixXDBase<Type>
//{
//  public:
//    static struct {
//    } isScalar;
//    //    static constexpr auto n() { return 1; }
//};

template <class Type, Size_t N>
    requires( N > 1 )
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

//template <class Type>
//using Scalar = MatrixXD<Type>;

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
