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

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        assert( !m_data.empty() );

        std::copy( m_data.begin(), std::next( m_data.begin(), size ), data );
        m_data.erase( m_data.begin(), std::next( m_data.begin(), size ) );
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        m_data.insert( m_data.end(), data, data + size );
    }

    void close() override {};

    bool isOpen() const override { return true; }

    bool isEnd() const override { return m_data.empty(); }

    void clear() override { m_data.clear(); }

    void setRetain( bool retained ) override {}

    Datas_t getData() const {
        Datas_t datas( m_data.begin(), m_data.end() );
        return datas;
    }

  private:
    std::list<hub::Data_t> m_data;
};

using Memory = MemoryT<>;

} // namespace io
} // namespace hub
