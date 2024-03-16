/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/07

#pragma once

#include <list>

#include "core/Macros.hpp"

#include "InputOutput.hpp"

namespace hub {
namespace io {

///
/// \brief The MemoryT class
///
template <class SerializerT = Serializer, class InputOutputT = InputOutput<SerializerT>>
class MemoryT : public InputOutputT
{
  public:
    using InputOutputT::read;
    using InputOutputT::write;

    ///
    /// \brief MemoryT
    /// \param container
    ///
    template <class Container>
    explicit MemoryT( Container& container ) {
        m_data.clear();
        m_data.insert( m_data.end(), container.begin(), container.end() );
    }

    ///
    /// \brief MemoryT
    ///
    MemoryT() = default;
    // MemoryT() {

    ///
    /// \brief read
    /// \param data
    /// \param size
    ///
    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        assert( !m_data.empty() );


        std::copy( m_data.begin(), std::next( m_data.begin(), size ), data );
        m_data.erase( m_data.begin(), std::next( m_data.begin(), size ) );

#ifdef DEBUG_MEMORY
        std::vector<Data_t> vector( data, data + size );
        std::cout << "[Memory] read " << vector << std::endl;
#endif

    }

    ///
    /// \brief write
    /// \param data
    /// \param size
    ///
    void write( const hub::Data_t* data, hub::Size_t size ) override {
#ifdef DEBUG_MEMORY
        std::vector<Data_t> vector( data, data + size );
        std::cout << "[Memory] write " << vector << std::endl;
#endif

        assert( size > 0 );
        const auto prevSize = m_data.size();
        m_data.resize(prevSize + size);
        std::copy(data, data + size, std::next(m_data.begin(), prevSize));

    }

    ///
    /// \brief close
    ///
    void close() override {};

    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override { return true; }

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override { return m_data.empty(); }

    ///
    /// \brief clear
    ///
    void clear() override { m_data.clear(); }

    ///
    /// \brief setRetain
    /// \param retained
    ///
    void setRetain( bool retained ) override {}

    ///
    /// \brief getData
    /// \return
    ///
    const Datas_t & getData() const {
        return m_data;
        // Datas_t datas( m_data.begin(), m_data.end() );
    }

  private:
    Datas_t m_data;
};

///
/// \brief Memory
///
using Memory = MemoryT<>;

} // namespace io
} // namespace hub
