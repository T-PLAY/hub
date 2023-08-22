#pragma once

#include <fstream>

#include "Input.hpp"
//#include "io/Memory.hpp"
#include "io/MemoryInterface.hpp"

namespace hub {
namespace input {

template <class Container>
class InputMemory : public io::Input, public io::MemoryInterface<Container>
{
  public:

//  protected:
    explicit InputMemory( Container& buff ) : io::MemoryInterface<Container>(buff) {}


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
    void close() override { io::MemoryInterface<Container>::m_closed = true; }

    ///
    /// @copydoc InputOutput::isOpen()
    ///
    bool isOpen() const override { return !io::MemoryInterface<Container>::m_closed; }

    ///
    /// @copydoc InputOutput::isEnd()
    ///
    bool isEnd() const override { return io::MemoryInterface<Container>::m_container.empty(); }


  public:

    ///
    /// @copydoc InputOutput::read()
    ///
    void read( unsigned char* data, size_t len ) override {
        assert( data != nullptr );
        memcpy( data, io::MemoryInterface<Container>::m_container.data(), len );
        io::MemoryInterface<Container>::m_container.erase( io::MemoryInterface<Container>::m_container.begin(), io::MemoryInterface<Container>::m_container.begin() + len );
    }


  private:
};



} // namespace input
} // namespace hub
