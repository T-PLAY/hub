#pragma once

#include <vector>

// #include "io/input/InputI.hpp"
// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"
#include "core/Macros.hpp"

#include "SerializerI.hpp"

namespace hub {
namespace serializer {

class SerializerImpl : public SerializerI
{
  public:
    //    SerializerImpl() = default;
    template <class T>
    const auto & serialize( const T& t ) {
        m_serial.clear();
//        pack(t);
        m_result.size = m_serial.size();
        return m_result;
        //        return Result { (const Data_t*)m_serial.data(), (Size_t)m_serial.size() };
    }

  private:
    std::vector<Data_t> m_serial;
    Result m_result{m_serial.data(), 0};
};

} // namespace serializer
} // namespace hub
