/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/25

#pragma once

#include <queue>

#include "core/Macros.hpp"

#include "InputOutput.hpp"

#include "io/Header.hpp"

namespace hub {
namespace io {

///
/// \brief The ArchiveT class
///
template <class SerializerT = Serializer, class InputOutputT = InputOutput<SerializerT>>
class ArchiveT : public InputOutputT
{
  public:
    using InputOutputT::read;
    using InputOutputT::write;

    ArchiveT() = default;
    ArchiveT(const io::Header & header) : m_header{header} {};

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        assert( !m_datas.empty() );
        const auto& vector = m_datas.front();
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(Data_t*, Size_t) : data = " << vector
                  << ", read size = " << size << std::endl;
#endif
        assert( vector.size() == size );

        memcpy( data, vector.data(), size );
        m_datas.pop();

#ifdef DEBUG
        ++m_nCall;
        m_lastCallSize = size;
#endif
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        std::vector<hub::Data_t> vector( data, data + size );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif
        m_datas.push( std::move( vector ) );

#ifdef DEBUG
        ++m_nCall;
        m_lastCallSize = size;
#endif
    }

    void close() override {};

    bool isOpen() const override { return true; }

    bool isEnd() const override { return m_datas.empty(); }

    void clear() override {
        while ( !m_datas.empty() )
            m_datas.pop();
    }

    void setRetain( bool retained ) override {}

    const Header& getHeader() const {
        return
     m_header;
    }

#ifdef DEBUG
    size_t getNCall() const { return m_nCall; }

    size_t getLastCallSize() const { return m_lastCallSize; }
#endif

  private:
    std::queue<std::vector<hub::Data_t>> m_datas;
#ifdef DEBUG
    size_t m_nCall        = 0;
    size_t m_lastCallSize = 0;
#endif
    io::Header m_header;
};

using Archive = ArchiveT<>;

} // namespace io
} // namespace hub
