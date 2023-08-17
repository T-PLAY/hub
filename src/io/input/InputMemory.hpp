#pragma once

#include <fstream>

#include "Input.hpp"
//#include "io/Memory.hpp"
#include "io/MemoryInterface.hpp"

namespace hub {
using namespace io;
namespace input {

template <class Container>
class InputMemory : public Input, public MemoryInterface<Container>
{
  public:

//  protected:
    explicit InputMemory( Container& buff ) : MemoryInterface<Container>(buff) {}


    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t ) {
        Input::read( t );
    }


  public:
//  protected:
    ///
    /// @copydoc InputOutput::close()
    ///
    void close() override { MemoryInterface<Container>::m_closed = true; }

    ///
    /// @copydoc InputOutput::isOpen()
    ///
    bool isOpen() const override { return !MemoryInterface<Container>::m_closed; }

    ///
    /// @copydoc InputOutput::isEnd()
    ///
    bool isEnd() const override { return MemoryInterface<Container>::m_container.empty(); }


  public:

    ///
    /// @copydoc InputOutput::read()
    ///
    void read( unsigned char* data, size_t len ) override {
        assert( data != nullptr );
        memcpy( data, MemoryInterface<Container>::m_container.data(), len );
        MemoryInterface<Container>::m_container.erase( MemoryInterface<Container>::m_container.begin(), MemoryInterface<Container>::m_container.begin() + len );
    }


  private:
};



} // namespace input
} // namespace hub
