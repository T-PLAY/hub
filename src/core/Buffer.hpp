
#pragma once

#include <span>
#include <type_traits>
#include <vector>
#include <array>

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
//    using Span = std::span<Data_t, Size * sizeof(Type)>;
    //    template <Size_t Size_, BufferOption Option_>
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const BufferBase<Size_, Option_>&
    //    data );

    //    constexpr BufferBase() = default;
//    constexpr BufferBase() = default;
//    constexpr BufferBase(const Span & span) {
//        setData(span);
//    }


    virtual Data_t * data() = 0;
    virtual const Data_t * data() const = 0;
    virtual Size_t size() const = 0;

    virtual CONSTEXPR20 std::string toString() const = 0;

//    virtual Data_t * getData
    //  protected:
//    virtual Span getSpan() = 0;
#if CPP_VERSION >= 20
    using Span = std::span<Type, Size>;
    virtual Span getSpan() = 0;
    void getData( Span& span ) const {
        const auto& internalSpan = getSpan();
        std::copy( internalSpan.begin(), internalSpan.end(), span.begin() );
    }
    void setData( const Span& span ) {
        const auto& internalSpan = getSpan();
        std::copy( span.begin(), span.end(), internalSpan.begin() );
    }
#endif
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
    BufferOption _Option = Option;
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
//    constexpr Buffer() : m_array{0} {}
//    constexpr StaticBuffer() = default;
//    template <Type... types>
    template <class... Args>
    constexpr Buffer(Args&&... args) : m_array{std::forward<Type&&>(args)...} {}
//    constexpr Buffer(Args&&... args) : m_array{std::forward<Args&&>(args)...} {}
//    constexpr Buffer(Args... args) : m_array{std::forward<Type>(args)...} {}
//    constexpr Buffer(Args&&... args) : m_array{std::forward<Type&&>(args)...} {}
//    constexpr Buffer(Args... args) : m_array{args...} {}
//        , m_span{m_array} {}
//        , m_span{m_array} {}

//    template <class Type>
//    constexpr StaticBuffer(Span span) {}

    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
    //  protected:

    template <Size_t i>
    constexpr Type get() const {
        return m_array.at(i);
    }

    CONSTEXPR20 std::string toString() const override {
        std::string str;
        str += "(static)";
//        str += "[" + TYPE_NAME( Data_t ) + ", " + PRETTY_BYTES( Size ) + "] = ";
        // str += hub::toString( m_array );
        str += hub::to_string( m_array );
        return str;
    }

//    auto begin() {
//        return m_array.begin();
//    }
    Data_t * data() override {
        return (Data_t*)m_array.data();
    }

    const Data_t * data() const override {
        return (const Data_t*)m_array.data();
    }
    Size_t size() const override {
        return m_array.size();
    }

#if CPP_VERSION >= 20
    using Span = typename BufferBase<Type, Size, StaticMemory>::Span;
    Span getSpan() override {
//        return m_span;
        //        return m_span;
        return Span { m_array.begin(), m_array.end() };
//        auto * noConstThis = const_cast<Buffer<Type, Size, StaticMemory>*>(this);
//        return std::span<Type, Size> { noConstThis->m_array.begin(), noConstThis->m_array.end() };
//        return Span { noConstThis->m_array.begin(), noConstThis->m_array.end() };
//        return Span();
    }
#endif


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

//    template <class... Args>
//    constexpr Data(Args... args) : m_data{args...} {}
//    constexpr Buffer() : m_vector(Size), m_span{m_vector} {}
    constexpr Buffer() : m_vector(Size) {}

    template <class... Args>
    constexpr Buffer(Args&&... args) : m_vector{std::forward<Type&&>(args)...} {}

//    Span getSpan() const { return m_span; };

//    template <Size_t i>
//    constexpr Type get() const {
//        return m_vector.at(i);
//    }

    CONSTEXPR20 std::string toString() const override {
        std::string str;
        str += "(dynamic)";
//        str += "[" + TYPE_NAME( Data_t ) + ", " + PRETTY_BYTES( Size ) + "] = ";
        str += toString( m_vector );
        return str;
    }

//    auto begin() {
//        return m_data.begin();
//    }
    Data_t * data() override {
        return (Data_t*)m_vector.data();
    }
    const Data_t * data() const override {
        return (const Data_t*)m_vector.data();
    }
    Size_t size() const override {
        return m_vector.size();
    }

#if CPP_VERSION >= 20
    using Span = typename BufferBase<Type, Size, DynamicMemory>::Span;
    Span getSpan() override { return std::span<Type, Size> { m_vector.begin(), m_vector.end() }; }
#endif

  private:
    std::vector<Type> m_vector;
//    Span m_span;
};
//static_assert( sizeof( Buffer<unsigned char, 1'000, DynamicMemory> ) == 32 ); // unix
//static_assert( sizeof( Buffer<unsigned char, 1'000, DynamicMemory> ) == 40  ); // win

} // namespace hub
