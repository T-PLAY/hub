#pragma once

#include "core/Output.hpp"
//#include "io/Memory.hpp"
#include "io/MemoryInterface.hpp"

namespace hub {
namespace output {

template <class Container>
class OutputMemory : public Output, public io::MemoryInterface<Container>
{
  public:
    explicit OutputMemory( Container& buff ) : io::MemoryInterface<Container>(buff)  {}

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( T& t ) {
        Output::write( t );
    }

//  public:
  protected:
    ///
    /// @copydoc InputOutput::close()
    ///
    void close() override { io::MemoryInterface<Container>::m_closed = true; }

    ///
    /// @copydoc InputOutput::isOpen()
    ///
    bool isOpen() const override { return !io::MemoryInterface<Container>::m_closed; }

//    ///
//    /// @copydoc InputOutput::isEnd()
//    ///
//    bool isEnd() const override { return MemoryInterface<Container>::m_container.empty(); }

  public:

    ///
    /// @copydoc InputOutput::write()
    ///
    void write( const unsigned char* data, size_t len ) override {
        assert( data != nullptr );
        io::MemoryInterface<Container>::m_container.insert(
            io::MemoryInterface<Container>::m_container.end(), data, data + len );
    }



  private:
};

} // namespace output
} // namespace hub
