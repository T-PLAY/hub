/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/07

#pragma once

#include <list>

#include "core/Macros.hpp"

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
    explicit MemoryT( Container& container ) {
        m_data.clear();
        m_data.insert( m_data.end(), container.begin(), container.end() );
    }

    MemoryT() = default;
    // MemoryT() {
        // m_data.reserve(20'000'000);
    // }

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        assert( !m_data.empty() );

        // std::cout << "[Memory] data " << m_data << std::endl;

        std::copy( m_data.begin(), std::next( m_data.begin(), size ), data );
        m_data.erase( m_data.begin(), std::next( m_data.begin(), size ) );

        std::vector<Data_t> vector( data, data + size );
        // std::cout << "[Memory] read " << vector << std::endl;

        // std::cout << "[Memory] data " << m_data << std::endl;
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        std::vector<Data_t> vector( data, data + size );
        // std::cout << "[Memory] data " << m_data << std::endl;
        // std::cout << "[Memory] write " << vector << std::endl;

        assert( size > 0 );
        const auto prevSize = m_data.size();
        m_data.resize(prevSize + size);
        // m_data.insert( m_data.end(), data, data + size );
        std::copy(data, data + size, std::next(m_data.begin(), prevSize));

        // std::cout << "[Memory] data " << m_data << std::endl;
    }

    void close() override {};

    bool isOpen() const override { return true; }

    bool isEnd() const override { return m_data.empty(); }

    void clear() override { m_data.clear(); }

    void setRetain( bool retained ) override {}

    const Datas_t & getData() const {
        return m_data;
        // Datas_t datas( m_data.begin(), m_data.end() );
        // return datas;
    }



  private:
    // std::list<hub::Data_t> m_data;
    // std::vector<hub::Data_t> m_data;
    Datas_t m_data;
};

using Memory = MemoryT<>;

} // namespace io
} // namespace hub
