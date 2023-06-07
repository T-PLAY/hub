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
    explicit Memory( Container& buff ) : m_container( buff ) {}

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t ) {
        Input::read( t );
    }

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( T& t ) {
        Output::write( t );
    }

  public:
    ///
    /// @copydoc InputOutput::close()
    ///
    void close() override {
        m_closed = true;
    }

    ///
    /// @copydoc InputOutput::isOpen()
    ///
    bool isOpen() const override {
        return ! m_closed;
    }

    ///
    /// @copydoc InputOutput::isEnd()
    ///
    bool isEnd() const override { return m_container.empty(); }

    ///
    /// @copydoc InputOutput::write()
    ///
    void write( const unsigned char* data, size_t len ) override {
        assert(data != nullptr);
        m_container.insert( m_container.end(), data, data + len );
    }

//    void write( const char* data, size_t len ) {
//        m_container.insert( m_container.end(), data, data + len );
//    }

    ///
    /// @copydoc InputOutput::read()
    ///
    void read( unsigned char* data, size_t len ) override {
        assert(data != nullptr);
        memcpy( data, m_container.data(), len );
        m_container.erase( m_container.begin(), m_container.begin() + len );
    }

  private:
    Container& m_container;
    bool m_closed = false;
};

} // namespace io
} // namespace hub
