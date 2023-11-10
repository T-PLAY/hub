
#pragma once

#include <span>
#include <type_traits>
#include <vector>

#include "Macros.hpp"
#include "Traits.hpp"
//#include "traits/Span.hpp"

namespace hub {

// namespace data {
enum BufferOption {
    StaticMemory  = 0x1,
    DynamicMemory = 0x2,
};
//}

template <class Type, Size_t Size, BufferOption Option>
class BufferBase
{
  public:
    using Span = std::span<Type, Size>;
//    using Span = std::span<Data_t, Size * sizeof(Type)>;
    //    template <Size_t Size_, BufferOption Option_>
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const BufferBase<Size_, Option_>&
    //    data );

    //    constexpr BufferBase() = default;
//    constexpr BufferBase() = default;
//    constexpr BufferBase(const Span & span) {
//        setData(span);
//    }

    void getData( Span& span ) const {
        const auto& internalSpan = getSpan();
        std::copy( internalSpan.begin(), internalSpan.end(), span.begin() );
    }
    void setData( const Span& span ) {
        const auto& internalSpan = getSpan();
        std::copy( span.begin(), span.end(), internalSpan.begin() );
    }

    virtual Data_t * data() = 0;

    virtual constexpr std::string toString() const = 0;

//    virtual Data_t * getData
    //  protected:
//    virtual Span getSpan() = 0;
    virtual Span getSpan() = 0;
    //    virtual std::string toString() const = 0;
};
static_assert( sizeof( BufferBase<int, 1'000, StaticMemory> ) == 8 );
static_assert( sizeof( BufferBase<int, 1'000, DynamicMemory> ) == 8 );

//template <class Type, Size_t Size, BufferOption Option>
//SRC_API std::ostream& operator<<( std::ostream& os, const BufferBase<Type, Size, Option>& data ) {
////    if ( Option == DynamicMemory ) { os << "(dynamic)"; }
////    else { os << "(static)"; }
////    os << "[" << TYPE_NAME( Data_t ) << ", " << PRETTY_BYTES( Size ) << "] = ";
////    auto span = const_cast<BufferBase<Size, Option>&>( data ).getSpan();
////    //    auto span = data.getSpan();
////    ::operator<<( os, span );
//    os << data.toString();
//    return os;
//}

////////////////////////////////////////////////////////////////////

template <class Type, Size_t Size,
          BufferOption Option = ( Size <= MAX_STACK_SIZE ) ? ( StaticMemory ) : ( DynamicMemory )>
class Buffer : public BufferBase<Type, Size, Option>
{
  public:
    //    constexpr std::span<Data_t, Size> getSpan() const override { return std::span<Data_t,
    //    Size> { m_data }; } std::array<Data_t, Size> m_data;
};
// static_assert( sizeof( Buffer<1'000> ) == 8 );

////////////////////////////////////////////////////////////////////

template <class Type, Size_t Size>
//    requires (Size <= MAX_STACK_SIZE)
class Buffer<Type, Size, StaticMemory> : public BufferBase<Type, Size, StaticMemory>
//class StaticBuffer : public BufferBase<Size, StaticMemory>
{
  public:
    using Span = BufferBase<Type, Size, StaticMemory>::Span;

    //    static_assert(Size <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");
    //    STATIC_WARNING(Size <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");
    //    StaticBuffer<Size, BufferOption::StaticMemory>() requires (Size > MAX_STACK_SIZE) {}
    //    [[deprecated("Stack size reached, please use static memory.")]]
    //    StaticBuffer<Size, BufferOption::StaticMemory>() requires (Size <= MAX_STACK_SIZE) {}
    //    constexpr StaticBuffer() = default;
    //    constexpr StaticBuffer() : m_data{0}
    //        , m_span{m_data.begin(), m_data.end()}
    //    {
    //    }
    constexpr Buffer() : m_array{0} {}
//    constexpr StaticBuffer() = default;
//    template <Type... types>
    template <class... Args>
//    constexpr Buffer(Args&&... args) : m_array{std::forward<Type&&>(args)...} {}
//    constexpr Buffer(Args&&... args) : m_array{std::forward<Args&&>(args) {}
    constexpr Buffer(Args&&... args) : m_array{std::forward<Type&&>(args)...} {}
//        , m_span{m_array} {}
//        , m_span{m_array} {}

//    template <class Type>
//    constexpr StaticBuffer(Span span) {}

    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
    //  protected:
    Span getSpan() override {
//        return m_span;
        //        return m_span;
        return Span { m_array.begin(), m_array.end() };
//        auto * noConstThis = const_cast<Buffer<Type, Size, StaticMemory>*>(this);
//        return std::span<Type, Size> { noConstThis->m_array.begin(), noConstThis->m_array.end() };
//        return Span { noConstThis->m_array.begin(), noConstThis->m_array.end() };
//        return Span();
    }

    constexpr std::string toString() const override {
        std::string str;
        str += "(static)";
//        str += "[" + TYPE_NAME( Data_t ) + ", " + PRETTY_BYTES( Size ) + "] = ";
        str += ::toString( m_array );
        return str;
    }

//    auto begin() {
//        return m_array.begin();
//    }
    Data_t * data() override {
        return (Data_t*)m_array.data();
    }

  private:
    std::array<Type, Size> m_array;
//    Span m_span;
};
static_assert( sizeof( Buffer<unsigned char, 1'000, StaticMemory> ) == 1'000 + 8 );

////////////////////////////////////////////////////////////////////

template <class Type, Size_t Size>
class Buffer<Type, Size, DynamicMemory> : public BufferBase<Type, Size, DynamicMemory>
//class DynamicBuffer : public BufferBase<Size, DynamicMemory>
{
  public:
//    using Span = std::span<Data_t, Size>;
    using Span = BufferBase<Type, Size, DynamicMemory>::Span;

//    template <class... Args>
//    constexpr Data(Args... args) : m_data{args...} {}
//    constexpr Buffer() : m_vector(Size), m_span{m_vector} {}
    constexpr Buffer() : m_vector(Size) {}

    template <class... Args>
    constexpr Buffer(Args&&... args) : m_vector{std::forward<Type&&>(args)...} {}

    Span getSpan() override { return std::span<Type, Size> { m_vector.begin(), m_vector.end() }; }
//    Span getSpan() const { return m_span; };

    constexpr std::string toString() const override {
        std::string str;
        str += "(dynamic)";
//        str += "[" + TYPE_NAME( Data_t ) + ", " + PRETTY_BYTES( Size ) + "] = ";
        str += ::toString( m_vector );
        return str;
    }

//    auto begin() {
//        return m_data.begin();
//    }
    Data_t * data() override {
        return (Data_t*)m_vector.data();
    }

  private:
    std::vector<Type> m_vector;
//    Span m_span;
};
static_assert( sizeof( Buffer<unsigned char, 1'000, DynamicMemory> ) == 32  );

} // namespace hub
