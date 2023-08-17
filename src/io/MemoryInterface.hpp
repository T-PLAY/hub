#pragma once

#include <stdexcept>

//#include "InputOutput.hpp"
#include "Macros.hpp"
#include <cassert>


namespace hub {
namespace io {

///
/// \brief The Container class
/// allows user to transfer sensor data through the memory (pipe tube).
///
template <class Container>
//class SRC_API Memory : public InputOutput
//class SRC_API Memory
class SRC_API MemoryInterface
{
  protected:
    ///
    /// \brief The exception class
    ///
    class exception : public std::runtime_error
    {
      public:
        ///
        /// \brief exception
        /// \param message
        ///
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}

        ///
        /// \brief what
        /// \return
        ///
        const char* what() const throw() { return std::runtime_error::what(); }
    };

//  public:
    ///
    /// \brief Memory
    /// constructed from shared communication (InputSensor and OutputSensor) data container.
    /// \param buff
    /// [in] is a cyclic buffer already instancied.
    ///
    explicit MemoryInterface( Container& buff ) : m_container( buff ) {}

//    Memory(Memory &&) = default;
//    Memory(const Memory &) = default;


//  private:
  protected:
    Container& m_container;
    bool m_closed = false;
};


} // namespace io
} // namespace hub
