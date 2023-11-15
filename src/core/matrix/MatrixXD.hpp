
#pragma once

#include <type_traits>

#include "MatrixBase.hpp"
#include "core/Matrix.hpp"

namespace hub {

/////////////////////////////////////////// TEMPLATES //////////////////////////////////////////////////

namespace _ {

template <class Type, Size_t... Ns>
    requires(sizeof...(Ns) > 0 && ((Ns > 1) && ...))
class MatrixXDBase
{
  public:
//    using Types = Type;
    static constexpr auto Capacity = ( Ns * ... );
//    static constexpr auto Size     = sizeof( Type ) * Capacity;
    static constexpr auto Size     = sizeof_<Type>() * Capacity;
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };
//    static_assert(size() < MAX_STACK_SIZE);
//    static_assert(size() <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");

    static constexpr auto nType() { return 1; };
    static constexpr auto nDim() { return sizeof...( Ns ); };
    template <Size_t i>
        requires( 0 <= i && i < nDim() )
    static constexpr auto getDim() {
        auto j = 0;
        for ( auto dim : { Ns... } ) {
            if ( j == i ) return dim;
            ++j;
        }
        return (Size_t)0;
    }
    const Data_t * data() const {
//        return m_buffer.getSpan();
        return m_buffer.data();
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

    template <class Type_>
    static constexpr int nType() {
        return std::is_same_v<Type, Type_>;
    }



//    template <int ith, int i, class Type_, class... Types_>
////        requires( hasType<Type_>() )
//    static constexpr Size_t getOffset() {
//        static_assert(hasType<Type_>());
////        static_assert(ith == 0 && i == 0);
//        if constexpr (ith == i) {
//            return 0;
//        }
//        else {
//            return Size;
//        }
//        return 0;
//    }

//    template <class Type_, int ith = 0>
////        requires( hasType<Type_>() )
//    static constexpr Size_t getOffset() {
//        static_assert(hasType<Type_>());
//        static_assert(ith == 0);
//        return 0;
//    }

    template <Size_t ith>
        requires( ith == 0 )
    using getType = Type;

  public:
//    using Data = std::array<Data_t, Size>;

//    constexpr MatrixXDBase(Args&&... args) : m_data{std::forward<Type&&>(args)...} {}
//    constexpr MatrixXDBase(Args&&... args) : m_data{std::forward<Type&&>(args)...} {}
//    constexpr MatrixXDBase(Args... args) : m_data{std::forward<Size>(args)...} {}
//    constexpr MatrixXDBase(Args... args) : m_data(std::forward<Args&&>(args)...) {}
//    template <class... Args>
//    constexpr MatrixXDBase(Args... args) : m_data{args...} {};

    template <class... Args>
    constexpr MatrixXDBase(Args&&... args) : m_buffer{std::forward<Type&&>(args)...} {}

//    template <class T>
//    constexpr MatrixXDBase(T t) : m_data{t} {};

//    template <class... Args>
//    constexpr MatrixXDBase() = default;
//    MatrixXDBase(std::initializer_list<Type> && list) : m_data { std::span<Data_t, list.size() * sizeof(Type)>(list.begin()) } {}

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
        return name() + " = " + m_buffer.toString();
    }

    template <int i = 0>
    const Type & get() {
        //        const auto offset = 0;
        // todo
        static_assert( 0 <= i && i < Capacity );
        //        return reinterpret_cast<Type>( m_data.ptr() + offset );
        return reinterpret_cast<const Type&>( *(m_buffer.data() + i) );
    }
//    template <Size_t i>
////        requires (Buffer<Type, Capacity>::_Option == BufferOption::StaticMemory)
////        requires (std::is_same_v<Buffer<Type, Capacity>, Buffer<Type, Capacity, BufferOption::StaticMemory>>)
//    constexpr Type & get() const {
//        static_assert( 0 <= i && i < Capacity );
////        return m_buffer.get<
////        if constexpr (requires { m_buffer.get<i>(); }; ) {
////        return dynamic_cast<const Buffer<Type, Capacity, BufferOption::StaticMemory> &>(m_buffer).get<i>();
////        return m_buffer.get<i>();

//        return reinterpret_cast<Type&>( *(m_buffer.data() + i) );
//    }

//    template <class Type_, Size_t... Ns_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const MatrixXDBase<Type, Ns...>& matrix );

//    constexpr bool operator==( const MatrixXDBase& matrix ) const { return m_data == matrix.m_data; }

    void serialize(Matrix & matrix) const {
//        Matrix::Node node;
//        node.m_hashCode = typeid(Type).hash_code();
//        node.m_dims = std::vector<int>{Ns...};
//        node.m_name = TYPE_NAME(Type);
//        node.m_size = Size;
//        auto node = Matrix::make_node<Type, Ns...>();
        auto matrix2 = make_matrix<Type, Ns...>();
        matrix << matrix2;
//        matrix.push_back(std::move(node));
    }

    Matrix getMatrix() const {
        Matrix matrix;
        serialize(matrix);
        return matrix;
    }

  private:
//    Data<Size, BufferOption::StaticMemory> m_data;
    Buffer<Type, Capacity> m_buffer;
//    Buffer<Data_t, Size> m_data;
//    Data<
//    Data m_data;
};
//static_assert(sizeof(MatrixXDBase<int, 2, 3, 4, 5, 6>) == 1);
static_assert(sizeof(MatrixXDBase<int, 2, 3, 4, 5, 6>) == sizeof(int) * 2 * 3 * 4 * 5 * 6 + 8);
static_assert(isMatrix<MatrixXDBase<int, 2>>);

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
