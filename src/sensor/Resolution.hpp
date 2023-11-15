
#pragma once

//#include <type_traits>

//#include "Format.hpp"
//#include "core/Array.hpp"
//#include "core/Macros.hpp"
//#include "core/Span.hpp"
//#include "core/Tuple.hpp"

namespace hub {
namespace sensor {

//namespace resolution {

//using Full_HD      = ResolutionT<format::RGB8, 1920, 1080>;
//using Full_HD_Plus = ResolutionT<format::RGB8, 2160, 1080>;
//using UltraWide    = ResolutionT<format::RGB8, 2560, 1080>;

//} // namespace resolution

//using NDim = std::vector<Size_t>;

//class Resolution
//{
//  public:
//    Format format;
//    //    Format format;
//    NDim nDim;

//    //    template <class Serial>
//    //    void serialize( Serial& serial ) {
//    //        serial(nDim);
//    //    }

//    bool operator==( const Resolution& other ) const {
//        return format == other.format && nDim == other.nDim;
//    }

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
//};

////////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

//template <class Type, Size_t... Ns>
//class MatrixXD
//{
//  public:
//    //    using WhoAmI = MatrixXD<Type, Ns...>;
//    //    constexpr auto whoAmI() { return WhoAmI(); };
//    //    static constexpr auto whoAmI = MatrixXD<Type, Ns...>();
//    //    using Capacity = {Ns * ...};
//    static constexpr auto Capacity = ( Ns * ... );
//    static constexpr auto Size     = sizeof( Type ) * Capacity;
//    static constexpr auto size() { return Size; };

//    static constexpr auto nType() { return 1; };
//    static constexpr auto nDim() { return sizeof...( Ns ); };
//    template <Size_t ith>
//        requires( 0 <= ith && ith < nDim() )
//    static constexpr auto getDim() {
//        auto i = 0;
//        for ( auto dim : { Ns... } ) {
//            if ( i == ith ) return dim;
//            ++i;
//        }
//        return (Size_t)0;
//    }
//    //    static constexpr auto capacity() { return (Ns * ...); };
//    //    using Data = std::array<Type, Capacity>;
//    using Data = std::array<Data_t, Size>;
//    //    constexpr Matrix()

//    template <class Type_, Size_t... Ns_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const MatrixXD<Type, Ns...>& matrix );

//    constexpr bool operator==( const MatrixXD& matrix ) const { return m_data == matrix.m_data; }

//    template <class Type_, std::size_t Size_>
//        requires( Size == Size_ )
//    void setData( const std::span<Type_, Size_>& span ) {
//        std::copy( span.begin(), span.end(), m_data.begin() );
//    }

//    template <class Type_, std::size_t Size_>
//        requires( Size == Size_ )
//    void getData( std::span<Type_, Size_>& span ) {
//        std::copy( m_data.begin(), m_data.end(), span.begin() );
//    }

//    static constexpr std::string name() {
//        //        std::string str = "<";
//        std::string str;
//        if constexpr ( requires { Type::name(); } ) { str += Type::name(); }
//        else { str += TYPE_NAME( Type ); }

//        if ( !( nDim() == 1 && getDim<0>() == 1 ) ) {
//            str += ":";
//            Size_t i = 0;
//            for ( auto dim : { Ns... } ) {
//                str += std::to_string( dim );
//                if ( i != nDim() - 1 ) str += "x";
//                ++i;
//            }
//        }
//        //        str += ">";
//        return str;
//    }

//    //  private:
//    Data m_data;
//    //    std::span<Type, Capacity> m_span{m_data};
//};

//template <class Type, Size_t... Ns>
//SRC_API std::ostream& operator<<( std::ostream& os, const MatrixXD<Type, Ns...>& matrix ) {
//    //    os << matrix.m_data;
//    os << matrix.name() << " = ";
//    ::operator<<( os, matrix.m_data );
//    return os;
//}

//template <class... Types>
//    requires( sizeof...( Types ) > 0 )
//class Matrix
//{
//  public:
//    //    using WhoAmI = Matrix<Types...>;
//    //    constexpr auto whoAmI() { return WhoAmI(); };
//    //    static constexpr auto whoAmI = Matrix<Types...>();

//    static constexpr auto Capacity = 1;
//    //    static constexpr auto Size = (sizeof(Types) + ...);
//    static constexpr auto size() { return ( sizeof( Types ) + ... ); };
//    using Data = std::array<Data_t, size()>;

//    //    using nType = (sizeof...(Types));
//    static constexpr auto nType() { return sizeof...( Types ); };
//    static constexpr auto nDim() { return 1; };
//    template <Size_t ith>
//        requires( ith == 0 )
//    static constexpr auto getDim() {
//        return 1;
//    }

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

//    using Tuple = std::tuple<Types...>;
//    template <Size_t ith>
//    using getType = typename std::tuple_element<ith, Tuple>::type;

//    constexpr Matrix() {}

//    constexpr Matrix( Types... types )
////        :
////        m_data { types... } //        : m_tuple{types...}
//    {
//        Size_t offset = 0;
//        for (auto type : {types...}) {
////            auto * data = (Data_t*)&type;
////            std::copy(data, data + sizeof(type), m_data.data() + offset);
////		    std::copy( span.begin(), span.end(), m_data.begin() );
////            offset += sizeof(type);
//        }
//    }

//    template <class Type>
//    //    using hasType = (std::is_same<Type, Types>() || ...);
//    static constexpr auto hasType() {
//        return ( std::is_same<Type, Types>() || ... );
//    }

//    template <class... Types_>
//        requires( sizeof...( Types_ ) > 1 )
//    static constexpr auto hasType() {
//        return ( hasType<Types_>() && ... );
//    }

//    constexpr bool operator==( const Matrix& matrix ) const { return m_data == matrix.m_data; }

//    template <Size_t i, class Type_, std::size_t Size_>
//        requires( size() == Size_ )
//    void setData( const std::span<Type_, Size_>& span ) {
//        std::copy( span.begin(), span.end(), m_data.begin() );
//    }

//    template <Size_t ith>
//    //        requires( size() == Size_ )
//    constexpr auto& getData() {
//        //        int offset = 0;
//        const auto offset = getOffset<ith, 0, Types...>();
//        static_assert( 0 <= offset && offset < size() );
//        using Type = getType<ith>;

//        return reinterpret_cast<Type&>( *( m_data.begin() + offset ) );
//        //        using size = sizeof(Type);
//        //        const auto dataSize = sizeof(Type);
//        //        auto span = std::span<Data_t, size()>{m_data};
//        //        return span;
//        //        for (int i = 0; i < ith; ++i) {
//        //            offset += sizeof(getType<i>());
//        //        }
//        //        std::span<Type_, Size_>& span;
//        //        std::span<
//        //        std::copy( m_data.begin(), m_data.end(), span.begin() );
//    }

//    //    template <Size_t i>
//    //    constexpr auto get() const {
//    //        return std::get<i>(m_tuple);
//    //    }

//    //    template <class Type>
//    //    constexpr auto has() const {
//    //        return std::get<Type>(m_tuple);
//    //    }

//    //    template <class Type>
//    //    constexpr auto get() const {
//    //        return std::get<Type>(m_tuple);
//    //    }

//    //    template <class Id, class Type>
//    //    constexpr auto get() const {
//    //        return std::get<Id, Type>(m_tuple);
//    //    }

//    template <class Type_, class... Types_>
//    static constexpr auto printName() {
//        std::string str;
//        //        using type = Type_();
//        if constexpr ( requires { Type_::name(); } ) { str += Type_::name(); }
//        else { str += TYPE_NAME( Type_ ); }

//        //        std::replace(str.begin(), str.end(), ' ', '_');
//        str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );

//        if constexpr ( sizeof...( Types_ ) > 0 ) { return str + "_" + printName<Types_...>(); }
//        else { return str; }
//    }

//    static constexpr std::string name() { return printName<Types...>(); }

//    template <class... Types_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Matrix<Types_...>& matrix );
//    //    using Data = std::array<Type, Capacity>;
//    //    Tuple m_tuple;
//    Data m_data;
//};

//template <class... Types>
//SRC_API std::ostream& operator<<( std::ostream& os, const Matrix<Types...>& matrix ) {
//    //    os << "(constexpr)";
//    os << matrix.name() << " = ";
//    ::operator<<( os, matrix.m_data );
//    //    ::operator<<( os, matrix.Tuple() );
//    //    ::operator<<( os, typename Matrix<Types...>::Tuple() );
//    //    os << ", size: " << buffer.size();
//    return os;
//}

/////////////////////////////////////////////////////////////////////////////////////////////

//namespace _ { // private section
//// using namespace hub::sensor;

//template <Size_t N = 1, Size_t... Ns>
//    requires( N > 0 && ( ( Ns > 1 ) && ... ) )
//class DimsT
//{
//  public:
//    static constexpr auto size = 1 + sizeof...( Ns );
//    static constexpr auto capacity() {
//        Size_t ret = N;
//        if constexpr ( sizeof...( Ns ) > 0 ) {
//            for ( auto n : { Ns... } ) {
//                ret *= n;
//            }
//        }
//        return ret;
//    }

//    static constexpr auto getDim( int iDim ) {
//        auto i = 0;
//        for ( auto dim : { N, Ns... } ) {
//            if ( i == iDim ) return dim;
//            ++i;
//        }
//        return (Size_t)0;
//    }

//    static constexpr auto to_string() {
//        std::string str = "";
//        Size_t i        = 0;
//        for ( auto dim : { N, Ns... } ) {
//            str += std::to_string( dim );
//            if ( i != size - 1 ) str += "x";
//            ++i;
//        }
//        return str;
//    }

//    static constexpr auto getNDim() { return NDim { N, Ns... }; }
//};

//// template <class... Types>
////     requires( sizeof...( Types ) > 0 )
//// class Type
////{
////   public:
////     static constexpr auto name() {
////         std::string str;
////         //        if constexpr ( requires { Type::name(); } ) { str += Type::name(); }
////         //        else { str += TYPE_NAME( Type ); }
////         str = ( TYPE_NAME( Types ) + ... );
////         return str;
////     }
////     static constexpr auto interpolable() { return false; }
////     static constexpr auto nByte() { return ( sizeof( Types ) + ... ); }
//// };

//template <class DimsT, class TypeT>
//class ResolutionTI
//{
//  public:
//    //    template <class Type>
//    //        requires(std::is_same_v<TypeT, Type>)
//    //    using get = ResolutionTI;

//    //    using whoAmI = ResolutionTI<DimsT, TypeT>;
//    //    using type = TypeT();
//    //    using Type = TypeT;
//    //    static constexpr auto type = std::remove_cvref_t<TypeT()>();
//    //    static constexpr auto type = TypeT();
//    //    using getType = TypeT;
//    using Type = TypeT;
//    static constexpr auto type() { return TypeT(); };
//    //    static constexpr auto whoAmI() { return decltype(ResolutionTI<DimsT, TypeT>()); };

//    static constexpr auto capacity() { return DimsT::capacity(); };
//    //    using DimsT::capacity;
//    //        size *= DimsT::capacity();

//    template <class... Type_>
//        requires( sizeof...( Type_ ) > 0 )
//    static constexpr bool hasType() {
//        if constexpr ( sizeof...( Type_ ) == 1 ) { return std::is_same_v<TypeT, Type_...>; }
//        return false;
//    }

//    template <Size_t i>
//        requires( i == 0 )
//    using getResolution = ResolutionTI;

//    //    template <class Type_>
//    //    static constexpr bool has() {
//    //        return std::is_same_v<TypeT, Type_>;
//    //    }

//    //    using getFormatT = TypeT;
//    //    static constexpr Size_t depth() {
//    //        if constexpr ( requires { TypeT::depth(); } ) { return TypeT::depth() + 1; }
//    //        else { return 0; }
//    //    }

//    //    template <class Output>
//    //    static void write( Output& output ) {
//    //        output.write( getResolution() );
//    //    }

//    static struct {
//    } notReadable;

//    static constexpr auto nByte() {
//        auto size = 0;
//        if constexpr ( requires { TypeT::nByte(); } ) { size = TypeT::nByte(); }
//        else { size = sizeof( TypeT ); }
//        size *= DimsT::capacity();
//        return size;
//    }
//    static_assert( nByte() > 0 );

//    static constexpr auto nDim() { return DimsT::size; };
//    static constexpr auto getDim( int iDim ) { return DimsT::getDim( iDim ); }

//    static constexpr std::string name() {
//        //        std::string str = "<";
//        std::string str;
//        if constexpr ( requires { TypeT::name(); } ) { str += TypeT::name(); }
//        else { str += TYPE_NAME( TypeT ); }

//        if ( !( nDim() == 1 && getDim( 0 ) == 1 ) ) { str += ":" + DimsT::to_string(); }
//        //        str += ">";
//        return str;
//    }

//    //    static constexpr auto has() {
//    //        return std::is_same_v<TypeT, Type_>();
//    //        return true;
//    //    }

//    static constexpr auto nResolution = 1;

//    template <class DimsT_, class FormatT_>
//    constexpr bool operator==( const ResolutionTI<DimsT_, FormatT_>& ) const {
//        return std::is_same_v<DimsT, DimsT_> && std::is_same_v<TypeT, FormatT_>;
//    }

//    template <class DimsT_, class FormatT_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const ResolutionTI<DimsT_, FormatT_>& resolution );

//    //    constexpr auto operator==( const Resolution& other ) const { return getResolution() ==
//    //    other; } static constexpr Resolution getResolution() {
//    //        return Resolution { getFormat(), DimsT::getNDim() };
//    //    }

//  private:
//    static constexpr Format getFormat() {
//        if constexpr ( requires { TypeT::name(); } ) {
//            return Format { sizeof( TypeT ), TypeT::name(), TypeT::interpolable() };
//        }
//        else { return Format { sizeof( TypeT ), TYPE_NAME( TypeT ), true }; }
//    }
//};

//template <class DimsT_, class FormatT_>
//SRC_API std::ostream& operator<<( std::ostream& os,
//                                  const ResolutionTI<DimsT_, FormatT_>& resolution ) {
//    //    os << "[ResolutionT] ";
//    os << resolution.name();
//    return os;
//}

//} // namespace _

//// template <Size_t N, class... Formats>
//// using Resolution1DT = _::ResolutionTI<_::DimsT<N>, _::TypeT<Formats...>>;
////  template <Size_t N, class... Formats>
////  using Res1DT = _::ResolutionTI<Dims<N>, TypeT<Formats...>>;

//// template <Size_t N, Size_t N2, class... Formats>
//// using Resolution2DT = _::ResolutionTI<_::DimsT<N, N2>, _::TypeT<Formats...>>;
////  template <Size_t N, Size_t N2, class... Formats>
////  using Res2DT = _::ResolutionTI<Dims<N, N2>, TypeT<Formats...>>;

//// template <class Type, Size_t... NDimTs>
//// static_assert(sizeof(ResolutionTI<Type, NDimTs...>()) == 0);
//// static_assert(sizeof(Reso))

//template <class Type, Size_t... Dims>
////    requires( N >= 1 && ( ( Dims > 1 ) && ... ) )
////    requires(  ( Dims > 1 ) && ...  )
//// typename std::enable_if<(Dims > 0) ...>, class>
//// requires (sizeof...(Dims) > 0)
//class ResolutionT : public _::ResolutionTI<_::DimsT<Dims...>, Type>
//{
//  public:
//    template <int i>
//    //    requires(sizeof...(Dims) > 0)
//    static constexpr Size_t n() {
//        static_assert( 0 <= i && i < _::ResolutionTI<_::DimsT<Dims...>, Type>::nDim() );
//        int j = 0;
//        for ( const auto& n : { Dims... } ) {
//            if ( i == j ) return n;
//            ++j;
//        }
//        return 0;
//    }
//};

//template <class Type>
//class ResolutionT<Type> : public _::ResolutionTI<_::DimsT<>, Type>
//{
//  public:
//    static struct {
//    } isScalar;
//    static constexpr auto n() { return 1; }
//};

//template <class Type, Size_t N>
//    requires( N > 1 )
//class ResolutionT<Type, N> : public _::ResolutionTI<_::DimsT<N>, Type>
//{
//  public:
//    static constexpr auto length() { return N; }
//};

//template <class Type, Size_t N, Size_t N2>
//class ResolutionT<Type, N, N2> : public _::ResolutionTI<_::DimsT<N, N2>, Type>
//{
//  public:
//    //    using _::ResolutionTI<_::DimsT<N, N2>, Type>::has;

//    //    using type = ResolutionT<Type, N, N2>;
//    static constexpr Size_t width() { return N; }
//    static constexpr Size_t height() { return N2; }
//};

//template <class Type, Size_t N, Size_t N2, Size_t N3>
//class ResolutionT<Type, N, N2, N3> : public _::ResolutionTI<_::DimsT<N, N2, N3>, Type>
//{
//  public:
//    static constexpr Size_t nx() { return N; }
//    static constexpr Size_t ny() { return N2; }
//    static constexpr Size_t nz() { return N3; }
//};

//template <class Type, Size_t N, Size_t N2, Size_t N3, Size_t N4>
//class ResolutionT<Type, N, N2, N3, N4> : public _::ResolutionTI<_::DimsT<N, N2, N3, N4>, Type>
//{
//  public:
//    static constexpr Size_t nx() { return N; }
//    static constexpr Size_t ny() { return N2; }
//    static constexpr Size_t nz() { return N3; }
//    static constexpr Size_t nt() { return N4; }
//};

//// template <class Type, Size_t... Dims>
//// constexpr auto Resolution() {
////     return ResolutionT<Type, Dims...>();
//// }

} // namespace sensor
} // namespace hub

// namespace hub {
// }
