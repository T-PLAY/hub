#pragma once

#include <span>
#include <vector>

#include "Macros.hpp"
#include "Tuple.hpp"
#include "Vector.hpp"

namespace hub {

// template <Size_t size_>
// class BufferI {
//   public:

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
//};

//template <Size_t Size, class Type>
template <class... Types>
class ArrayBuffer
{
  public:
//    static constexpr auto size = sizeof( Data );
    static constexpr auto Size() { return  (sizeof(Types) + ...); };
//    static constexpr auto size() { return Size * sizeof(Type); };

//    template <class... Args>
    constexpr ArrayBuffer( Types... types )
        : m_tuple{types...}
//        : m_data{datas...}
    {
    }

    template <Size_t i>
    constexpr auto get() const {
        return std::get<i>(m_tuple);
    }

    template <class Type>
    constexpr auto get() const {
        return std::get<Type>(m_tuple);
    }

//    template <class ArrayBuffer
//    constexpr bool operator==(const ArrayBuffer & other) const {
//        return data == other.data;
//    }
    template <class... Types_>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ArrayBuffer<Types_...>& buffer );

//    Data data;
    std::tuple<Types...> m_tuple;
//    std::array<Type, Size> m_data;
};

template <class... Types>
SRC_API std::ostream& operator<<( std::ostream& os, const ArrayBuffer<Types...>& buffer ) {
//    std::vector<Data_t> vector { buffer.data, buffer.data + buffer.size };
    os << "(constexpr)";
    ::operator<<( os, buffer.m_tuple );
    os << ", size: " << buffer.size();
    return os;
}

template <class Data>
static constexpr inline auto Buffer( Data data ) {
    return ArrayBuffer{ data };
}

/////////////////////////////////////////////////////////////////////////


template <Size_t size_>
class ConstantBuffer
{
  public:
    static constexpr auto size = size_;
    //    static constexpr auto size() {
    //        return size;
    //    }

    ConstantBuffer( const Data_t* data_ ) : data { data_ } {}
    ConstantBuffer( const ConstantBuffer& ) = default;

    template <class BufferT>
    void operator=( const BufferT& other ) = delete;

    template <class BufferT>
    //    constexpr bool operator==( const ConstantBuffer<size_>& other ) {
    constexpr bool operator==( const BufferT& other ) {
        static_assert( size == BufferT::size );
        return std::memcmp( data, other.data, size ) == 0;
    }

    template <Size_t size__>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ConstantBuffer& buffer );

    const Data_t* const data;
};

template <Size_t size>
SRC_API std::ostream& operator<<( std::ostream& os, const ConstantBuffer<size>& buffer ) {
    std::vector<Data_t> vector { buffer.data, buffer.data + buffer.size };
    //    os << vector;
    os << "(const)";
    ::operator<<( os, vector );
    return os;
}

///////////////////////////////////////////////////////////////////////////////////

template <Size_t size_>
class MutableBuffer
{
  public:
    static constexpr auto size = size_;
    void operator=( const MutableBuffer<size_>& other ) const {
        std::memcpy( data, other.data, size );
    }
    void operator=( const ConstantBuffer<size_>& other ) const {
        std::memcpy( data, other.data, size );
    }

    MutableBuffer( Data_t* data_ ) : data { data_ } {}
    MutableBuffer( const MutableBuffer& ) = default;

    template <class BufferT>
    constexpr bool operator==( const BufferT& other ) const {
        static_assert( size == BufferT::size );
        return std::memcmp( data, other.data, size ) == 0;
    }

    template <Size_t size__>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const MutableBuffer& buffer );

    Data_t* const data;
};

template <Size_t size>
SRC_API std::ostream& operator<<( std::ostream& os, const MutableBuffer<size>& buffer ) {
    std::vector<Data_t> vector { buffer.data, buffer.data + buffer.size };
    os << "(mutable)";
    ::operator<<( os, vector );
    return os;
}

template <Size_t size>
static constexpr inline auto Buffer( Data_t* data ) {
    return MutableBuffer<size> { data };
}

template <Size_t size>
static constexpr inline auto Buffer( const Data_t* data ) {
    return ConstantBuffer<size> { data };
}


//template <class Type>
//static constexpr inline auto Buffer( const Type& type ) {
//    //    return MutableBuffer<sizeof(Type)> { reinterpret_cast<Data_t*>(&type) };
//    return ConstantBuffer<sizeof( Type )> { (const Data_t*)( &type ) };
//}

//template <class Type>
//static constexpr inline auto Buffer( Type& type ) {
//    return MutableBuffer<sizeof( Type )> { (Data_t*)( &type ) };
//    //    return ConstantBuffer<sizeof(Type)> { (const Data_t*)(&type) };
//}

// namespace hub {

// #define Buffer( _data_, _size_ ) make_buffer<_size_>( _data_ )

//}

// class SpanBuffer {
//   public:
//     std::span<Data_t> span;
// };

// template <class T>
// constexpr auto Buffer(T t) {
//     return SpanBuffer{std::span(t)};
////    std::span span(std::forward<T&&>(t));
////    std::span span(t);
////    return MutableBuffer{span.data(), span.size()};
//}

// constexpr auto Buffer(Data_t * data, Size_t size ) {
//     return MutableBuffer<size>{data};
// }

// constexpr auto Buffer(const Data_t * data, Size_t size ) {
//     return ConstantBuffer{data, size};
// }

} // namespace hub
