#pragma once

#include <span>
#include <vector>

#include "Macros.hpp"
#include "Vector.hpp"

namespace hub {

//template <Size_t size_>
//class BufferI {
//  public:

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
//};

template <Size_t size_>
class ConstantBuffer
{
  public:
    static constexpr auto size = size_;
    //    static constexpr auto size() {
    //        return size;
    //    }

    template <class BufferT>
    void operator=( const BufferT& other ) = delete;

    template <class BufferT>
//    constexpr bool operator==( const ConstantBuffer<size_>& other ) {
    constexpr bool operator==( const BufferT& other ) {
        static_assert(size == BufferT::size);
        return std::memcmp(data, other.data, size) == 0;
//        static_assert(size == other.size);
//        if ( size != other.size ) return false;
//        for ( int i = 0; i < size; ++i ) {
//            if ( data[i] != other.data[i] ) return false;
//        }
//        return true;
    }

    template <Size_t size__>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ConstantBuffer& buffer );

//  private:
    const Data_t* const data;
    //    constexpr auto size() const { return size; };
    //    const Size_t size;
};

template <Size_t size>
SRC_API std::ostream& operator<<( std::ostream& os, const ConstantBuffer<size>& buffer ) {
    std::vector<Data_t> vector{buffer.data, buffer.data + buffer.size};
//    os << vector;
    os << "(const)";
    ::operator<<(os, vector);
    return os;
}

///////////////////////////////////////////////////////////////////////////////////

template <Size_t size_>
class MutableBuffer
{
  public:
    static constexpr auto size = size_;
    //    static constexpr auto getSize = size;
//    template <class BufferT>
    void operator=( const MutableBuffer<size_>& other ) const {
//        assert( size == other.size );
        std::memcpy( data, other.data, size );
//        return *this;
    }

//    template <class BufferT>
//    constexpr bool operator==( const MutableBuffer<size_>& other ) const {
//        return std::memcmp(data, other.data, size) == 0;
////        static_assert(size == other.size);
////        if ( size != other.size ) return false;
////        for ( int i = 0; i < size; ++i ) {
////            if ( data[i] != other.data[i] ) return false;
////        }
////        return true;
//    }

    template <class BufferT>
    constexpr bool operator==( const BufferT& other ) const {
        static_assert(size == BufferT::size);
        return std::memcmp(data, other.data, size) == 0;
//        if ( size != other.size ) return false;
//        for ( int i = 0; i < size; ++i ) {
//            if ( data[i] != other.data[i] ) return false;
//        }
//        return true;
    }


    template <Size_t size__>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const MutableBuffer& buffer );

//  private:
    Data_t* const data;
    //    const Size_t size;
};

template <Size_t size>
SRC_API std::ostream& operator<<( std::ostream& os, const MutableBuffer<size>& buffer ) {
    std::vector<Data_t> vector{buffer.data, buffer.data + buffer.size};
//    os << vector;
    os << "(mutable)";
    ::operator<<(os, vector);
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

// namespace hub {

//#define Buffer( _data_, _size_ ) make_buffer<_size_>( _data_ )

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
