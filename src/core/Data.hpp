
#pragma once

#include <span>
#include <vector>
#include <type_traits>

#include "Macros.hpp"
#include "Span.hpp"

namespace hub {

namespace data {
enum class Option {
    Static  = 0x1,
    Dynamic = 0x2,
};
}

template <Size_t Size, data::Option Option>
class DataI
{
  public:
//    template <Size_t Size_, data::Option Option_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const DataI<Size_, Option_>& data );

    using Span = std::span<Data_t, Size>;
    void getData(Span & span) const {
        const auto & internalSpan = getSpan();
        std::copy(internalSpan.begin(), internalSpan.end(), span.begin());
    }
    void setData(const Span & span) {
        const auto & internalSpan = getSpan();
        std::copy(span.begin(), span.end(), internalSpan.begin());
    }

//  protected:
    virtual const Span & getSpan() const = 0;
//    virtual std::string toString() const = 0;
};
static_assert( sizeof( DataI<1'000, data::Option::Static> ) == 8 );
static_assert( sizeof( DataI<1'000, data::Option::Dynamic> ) == 8 );

template <Size_t Size, data::Option Option>
SRC_API std::ostream& operator<<( std::ostream& os, const DataI<Size, Option>& data ) {
    if (Option == data::Option::Dynamic) {
        os << "(dynamic)";
    }
    else {
        os << "(static)";
    }
    os << "[" << TYPE_NAME( Data_t ) << ", " << PRETTY_BYTES( Size ) << "] = ";
//    auto span = const_cast<DataI<Size, Option>&>( data ).getSpan();
    const auto & span = data.getSpan();
    ::operator<<( os, span );
    return os;
}

////////////////////////////////////////////////////////////////////

template <Size_t Size,
          data::Option Option =
              ( Size <= MAX_STACK_SIZE ) ? ( data::Option::Static ) : ( data::Option::Dynamic )>
class Data : public DataI<Size, Option>
{
  public:
    //    constexpr std::span<Data_t, Size> getSpan() const override { return std::span<Data_t,
    //    Size> { m_data }; } std::array<Data_t, Size> m_data;
};

////////////////////////////////////////////////////////////////////

template <Size_t Size>
//    requires (Size <= MAX_STACK_SIZE)
class Data<Size, data::Option::Static> : public DataI<Size, data::Option::Static>
{
  public:
//    static_assert(Size <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");
//    STATIC_WARNING(Size <= MAX_STACK_SIZE, "Stack size reached, please use static memory.");
//    Data<Size, data::Option::Static>() requires (Size > MAX_STACK_SIZE) {}
//    [[deprecated("Stack size reached, please use static memory.")]]
//    Data<Size, data::Option::Static>() requires (Size <= MAX_STACK_SIZE) {}

    using Span = std::span<Data_t, Size>;
    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
//  protected:
    const Span & getSpan() const override {
        return m_span;
//        return std::span<Data_t, Size> { m_data.begin(), m_data.end() };
    }

  private:
    std::array<Data_t, Size> m_data;
    Span m_span{m_data};
};
static_assert( sizeof( Data<1000, data::Option::Static> ) == 1000 + 8 + 8 );

////////////////////////////////////////////////////////////////////

template <Size_t Size>
class Data<Size, data::Option::Dynamic> : public DataI<Size, data::Option::Dynamic>
{
  public:
    using Span = std::span<Data_t, Size>;

    Data() : m_data( Size ) {}
    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
//    std::span<Data_t, Size> getSpan() override {
//        return std::span<Data_t, Size> { m_data.begin(), m_data.end() };
//    }
//  protected:
    const Span & getSpan() const override {
        return m_span;
    }

  private:
    std::vector<Data_t> m_data;
    Span m_span{m_data};
};
static_assert( sizeof( Data<1'000, data::Option::Dynamic> ) == 32 + 8 );

} // namespace hub
