
#pragma once

#include <span>
#include <type_traits>
#include <vector>

#include "Macros.hpp"
#include "Traits.hpp"
//#include "traits/Span.hpp"

namespace hub {

// namespace data {
enum DataOption {
    StaticMemory  = 0x1,
    DynamicMemory = 0x2,
};
//}

template <Size_t Size, DataOption Option>
class DataI
{
  public:
    //    template <Size_t Size_, DataOption Option_>
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const DataI<Size_, Option_>&
    //    data );

    //    constexpr DataI() = default;

    using Span = std::span<Data_t, Size>;
    void getData( Span& span ) const {
        const auto& internalSpan = getSpan();
        std::copy( internalSpan.begin(), internalSpan.end(), span.begin() );
    }
    void setData( const Span& span ) {
        const auto& internalSpan = getSpan();
        std::copy( span.begin(), span.end(), internalSpan.begin() );
    }

    virtual constexpr std::string toString() const = 0;

    //  protected:
    virtual Span getSpan() = 0;
    //    virtual std::string toString() const = 0;
};
static_assert( sizeof( DataI<1'000, StaticMemory> ) == 8 );
static_assert( sizeof( DataI<1'000, DynamicMemory> ) == 8 );

template <Size_t Size, DataOption Option>
SRC_API std::ostream& operator<<( std::ostream& os, const DataI<Size, Option>& data ) {
//    if ( Option == DynamicMemory ) { os << "(dynamic)"; }
//    else { os << "(static)"; }
//    os << "[" << TYPE_NAME( Data_t ) << ", " << PRETTY_BYTES( Size ) << "] = ";
//    auto span = const_cast<DataI<Size, Option>&>( data ).getSpan();
//    //    auto span = data.getSpan();
//    ::operator<<( os, span );
    os << data.toString();
    return os;
}

////////////////////////////////////////////////////////////////////

template <Size_t Size,
          DataOption Option = ( Size <= MAX_STACK_SIZE ) ? ( StaticMemory ) : ( DynamicMemory )>
class Data : public DataI<Size, Option>
{
  public:
    //    constexpr std::span<Data_t, Size> getSpan() const override { return std::span<Data_t,
    //    Size> { m_data }; } std::array<Data_t, Size> m_data;
};
// static_assert( sizeof( Data<1'000> ) == 8 );

////////////////////////////////////////////////////////////////////

template <Size_t Size>
//    requires (Size <= MAX_STACK_SIZE)
class Data<Size, StaticMemory> : public DataI<Size, StaticMemory>
{
  public:
    //    static_assert(Size <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");
    //    STATIC_WARNING(Size <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");
    //    Data<Size, DataOption::StaticMemory>() requires (Size > MAX_STACK_SIZE) {}
    //    [[deprecated("Stack size reached, please use static memory.")]]
    //    Data<Size, DataOption::StaticMemory>() requires (Size <= MAX_STACK_SIZE) {}
    //    constexpr Data() = default;
    //    constexpr Data() : m_data{0}
    //        , m_span{m_data.begin(), m_data.end()}
    //    {
    //    }

    using Span = std::span<Data_t, Size>;
    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
    //  protected:
    Span getSpan() override {
        //        return m_span;
        //        return std::span<Data_t, Size> { m_data};
        return std::span<Data_t, Size> { m_data.begin(), m_data.end() };
    }

    constexpr std::string toString() const override {
        std::string str;
        str += "(static)";
//        str += "[" + TYPE_NAME( Data_t ) + ", " + PRETTY_BYTES( Size ) + "] = ";
        str += ::toString( m_data );
        return str;
    }

  private:
    std::array<Data_t, Size> m_data;
    //    Span m_span;
};
static_assert( sizeof( Data<1'000, StaticMemory> ) == 1'000 + 8 );

////////////////////////////////////////////////////////////////////

template <Size_t Size>
class Data<Size, DynamicMemory> : public DataI<Size, DynamicMemory>
{
  public:
    using Span = std::span<Data_t, Size>;

    constexpr Data() : m_data( Size ) {}
    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
    //    std::span<Data_t, Size> getSpan() override {
    //        return std::span<Data_t, Size> { m_data.begin(), m_data.end() };
    //    }
    //  protected:
    Span getSpan() override { return std::span<Data_t, Size> { m_data.begin(), m_data.end() }; }

    constexpr std::string toString() const override {
        std::string str;
        str += "(dynamic)";
//        str += "[" + TYPE_NAME( Data_t ) + ", " + PRETTY_BYTES( Size ) + "] = ";
        str += ::toString( m_data );
        return str;
    }

  private:
    std::vector<Data_t> m_data;
    //    Span m_span{m_data};
};
static_assert( sizeof( Data<1'000, DynamicMemory> ) == 32 );

} // namespace hub
