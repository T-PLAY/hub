#pragma once

#include "InputOutput.hpp"

namespace hub {
namespace io {

///
/// \brief The Container class
/// allows user to transfer sensor data through the memory (pipe tube).
///
template <class Container>
class SRC_API Memory : public InputOutput
{
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

  public:
    ///
    /// \brief Memory
    /// constructed from shared communication (InputSensor and OutputSensor) data container.
    /// \param buff
    /// [in] is a cyclic buffer already instancied.
    ///
    //    template <class Container>
    explicit Memory( Container& buff ) : m_container( buff ) {}

//    ///
//    /// \brief read
//    /// \param t
//    ///
    template <class T>
    void read( T& t )  {
        Input::read(t);
    }

    template <class T>
    void write( T& t )  {
        Output::write(t);
    }

//  protected:
  public:
    ///
    /// @copydoc Interface::close()
    ///
    void close()  override {}

    ///
    /// @copydoc Interface::isOpen()
    ///
    bool isOpen() const override { return true; }

    ///
    /// @copydoc Interface::isEnd()
    ///
    bool isEnd() const override { return false; }

    ///
    /// @copydoc Interface::write()
    ///
    void write( const unsigned char* data, size_t len )  override {
        m_container.insert(m_container.end(), data, data + len);
    }

    ///
    /// @copydoc Interface::read()
    ///
    void read( unsigned char* data, size_t len )  override {
        memcpy(data, m_container.data(), len);
        m_container.erase(m_container.begin(), m_container.begin() + len);
    }

  private:
    Container& m_container;
};

} // namespace io
} // namespace hub
