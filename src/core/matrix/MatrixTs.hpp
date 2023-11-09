
#pragma once


#include "MatrixBase.hpp"

namespace hub {

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

template <class... Types>
    requires( sizeof...( Types ) > 1 )
class MatrixTs
{
  public:
    static constexpr auto Capacity = 1;
//    static constexpr auto Size     = ( sizeof( Types ) + ... );
    static constexpr auto Size     = sizeof_<Types...>();
    static constexpr auto capacity() { return Capacity; };
    static constexpr auto size() { return Size; };
//    static_assert(size() < MAX_STACK_SIZE);
    static_assert(size() <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");

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
        return ( std::is_same<Type, Types>() || ... );
    }

    template <class... Types_>
        requires( sizeof...( Types_ ) > 1 )
    static constexpr auto hasType() {
        return ( hasType<Types_>() && ... );
    }

    template <Size_t ith>
    using getType = typename std::tuple_element<ith, std::tuple<Types...>>::type;

  public:
    using Data = std::array<Data_t, size()>;

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

//    constexpr bool operator==( const MatrixTs& matrix ) const { return m_data == matrix.m_data; }

    static constexpr std::string name() { return printName<Types...>(); }

    template <class... Types_>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const MatrixTs<Types_...>& matrix );

  private:
    template <const Size_t ith, Size_t i = 0, class Type_, class... Types_>
    static constexpr auto getOffset() {
        if constexpr ( ith == i ) { return 0; }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return sizeof( Type_ ) + getOffset<ith, i + 1, Types_...>();
            }
            else { return sizeof( Type_ ); }
        }
    }

    template <class Type_, class... Types_>
    static constexpr auto printName() {
        std::string str;
        //        using type = Type_();
        if constexpr ( requires { Type_::name(); } ) { str += Type_::name(); }
        else { str += TYPE_NAME( Type_ ); }

        //        std::replace(str.begin(), str.end(), ' ', '_');
        str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );

        if constexpr ( sizeof...( Types_ ) > 0 ) { return str + "_" + printName<Types_...>(); }
        else { return str; }
    }

  private:
//    Data m_data;
};
static_assert(sizeof(MatrixTs<int, double, float>) == 1);

template <class... Types>
SRC_API std::ostream& operator<<( std::ostream& os, const MatrixTs<Types...>& matrix ) {
    //    os << "(constexpr)";
    os << matrix.name() << " = ";
//    ::operator<<( os, matrix.m_data );
    //    ::operator<<( os, matrix.Tuple() );
    //    ::operator<<( os, typename MatrixTs<Types...>::Tuple() );
    //    os << ", size: " << buffer.size();
    return os;
}


} // namespace hub
