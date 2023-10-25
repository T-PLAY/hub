#pragma once

//#include <vector>

#include <queue>

#include "core/Vector.hpp"
//#include "core/Macros.hpp"
//#include "core/Vector.hpp"
//#include "InputOutputI.hpp"
#include "input/InputZppBits.hpp"
#include "output/OutputZppBits.hpp"
//#include "core/Vector.hpp"

//#include "input/InputI.hpp"
//#include "output/OutputI.hpp"
#include "InputOutputI.hpp"

//#ifdef HUB_DEBUG_INPUT
//#    define HEADER_INPUT_MSG                                                             \
//        "\t\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
//                  << "m[Archive:" << this << "]\033[0m "
//#endif
//#ifdef HUB_DEBUG_OUTPUT
//#    define HEADER_OUTPUT_MSG                                                          \
//        "\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
//                << "m[Archive:" << this << "]\033[0m "
//#endif

namespace hub {
namespace io {


//template <class Input = input::InputI, class Output = output::OutputI>
//class Archive : public input::InputI, public output::OutputI
template <class InputOutput = InputOutputI>
//class Archive : public Input, public Output
//class Archive : public Input, public Output
class Archive : public InputOutput
{
  public:
    using InputOutput::read;
    using InputOutput::write;

//    Archive() = default;

//    ~Archive() { std::cout << "[Archive] ~Archive()" << std::endl; }

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( !m_datas.empty() );
        const auto& vector = m_datas.front();
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(Data_t*, Size_t) : data = " << vector << ", read size = " << size << std::endl;
#endif
        assert( vector.size() == size );

        memcpy( data, vector.data(), size );
        m_datas.pop();
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        std::vector<hub::Data_t> vector( data, data + size );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif
        m_datas.push( std::move( vector ) );
    }

    void close() override {};
    //    void close() override { assert( isOpen() );
    //        m_isOpen = false;
    //    };

    bool isOpen() const override { return true; }
    //    bool isOpen() const override { return m_isOpen; };

    bool isEmpty() const override {
        //        auto ret = m_datas.empty();
        //        return ret;
        return m_datas.empty();
    }

    void clear() override {
        while ( !m_datas.empty() )
            m_datas.pop();
    }


  private:
    std::queue<std::vector<hub::Data_t>> m_datas;
//    InputT & m_input;
//    OutputT & m_output;
    //    bool m_isOpen = true;
};


} // namespace io
} // namespace hub
