
#pragma once

#include <span>
#include <vector>

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
    template <Size_t Size_, data::Option Option_>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const DataI<Size_, Option_>& data );

    virtual std::span<Data_t, Size> getSpan() = 0;
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
    auto span = const_cast<DataI<Size, Option>&>( data ).getSpan();
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
class Data<Size, data::Option::Static> : public DataI<Size, data::Option::Static>
{
  public:
    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
    std::span<Data_t, Size> getSpan() override {
        return std::span<Data_t, Size> { m_data.begin(), m_data.end() };
    }

  private:
    std::array<Data_t, Size> m_data;
};
static_assert( sizeof( Data<1000, data::Option::Static> ) == 1000 + 8 );

////////////////////////////////////////////////////////////////////

template <Size_t Size>
class Data<Size, data::Option::Dynamic> : public DataI<Size, data::Option::Dynamic>
{
  public:
    Data() : m_data( Size ) {}
    //    std::span<Data_t, Size> getSpan() override { return std::span<Data_t, Size> {
    //    m_data.begin(), m_data.end() }; }
    std::span<Data_t, Size> getSpan() override {
        return std::span<Data_t, Size> { m_data.begin(), m_data.end() };
    }

  private:
    std::vector<Data_t> m_data;
};
static_assert( sizeof( Data<1'000, data::Option::Dynamic> ) == 32 );

} // namespace hub
