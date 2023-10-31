#pragma once

// #include <vector>

#include <queue>

#include "core/Vector.hpp"
//#include "input/InputZppBits.hpp"
//#include "output/OutputZppBits.hpp"

//#include "InputOutputI.hpp"
//#include "InputOutputImpl.hpp"
//#include "InputOutputZppBits.hpp"
#include "InputOutput.hpp"

namespace hub {
namespace io {

//template <class InputOutputT = InputOutput<>>
template <class SerializerT = Serializer, class InputOutputT = InputOutput<SerializerT>>
class Archive : public InputOutputT
{
  public:
    using InputOutputT::read;
    using InputOutputT::write;

    void read( hub::Data_t* data, hub::Size_t size ) override {
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
#endif
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        std::vector<hub::Data_t> vector( data, data + size );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif
        m_datas.push( std::move( vector ) );

#ifdef DEBUG
        ++m_nCall;
#endif
    }

    void close() override {};

    bool isOpen() const override { return true; }

    bool isEnd() const override { return m_datas.empty(); }

    void clear() override {
        while ( !m_datas.empty() )
            m_datas.pop();
    }

#ifdef DEBUG
    size_t getNCall() const {
        return m_nCall;
    }
#endif

  private:
    std::queue<std::vector<hub::Data_t>> m_datas;
    size_t m_nCall = 0;
};

} // namespace io
} // namespace hub
