#pragma once

// #include <vector>

#include <queue>

#include "core/Macros.hpp"
//#include "core/Vector.hpp"
//#include "input/InputZppBits.hpp"
//#include "output/OutputZppBits.hpp"

//#include "InputOutputBase.hpp"
//#include "InputOutputImpl.hpp"
//#include "InputOutputZppBits.hpp"
#include "InputOutput.hpp"

namespace hub {
namespace io {

template <class SerializerT = Serializer, class InputOutputT = InputOutput<SerializerT>>
class MemoryT : public InputOutputT
{
  public:
    using InputOutputT::read;
    using InputOutputT::write;

    template <class Container>
    explicit MemoryT(Container & container) {
        m_data.clear();
        m_data.insert(m_data.end(), container.begin(), container.end());
    }

    MemoryT() = default;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert(size > 0);
        assert( !m_data.empty() );
        // const auto& vector = m_data.front();
// #ifdef HUB_DEBUG_INPUT
//         std::cout << "\t" << HEADER << "read(Data_t*, Size_t) : data = " << vector
//                   << ", read size = " << size << std::endl;
// #endif
        // assert( vector.size() == size );

        // memcpy( data, vector.data(), size );
        std::copy( m_data.begin(), std::next(m_data.begin(), size), data);
        m_data.erase(m_data.begin(), std::next(m_data.begin(), size));
        // m_data.pop();

    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        assert(size > 0);
        // std::vector<hub::Data_t> vector( data, data + size );
// #ifdef HUB_DEBUG_OUTPUT
        // std::cout << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
// #endif
        // m_data.push( std::move( vector ) );
        m_data.insert(m_data.end(), data, data + size);

    }

    void close() override {
    };

    bool isOpen() const override { return true; }

    bool isEnd() const override { return m_data.empty(); }

    void clear() override {
        m_data.clear();
    }

    void setRetain(bool retained) override {
    }

  private:
    std::list<hub::Data_t> m_data;
};

using Memory = MemoryT<>;

} // namespace io
} // namespace hub
