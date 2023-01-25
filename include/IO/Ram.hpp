#pragma once

#include "Interface.hpp"

namespace hub {
namespace io {

///
/// \brief The CyclicBuff class
/// represents a memory buffer in RAM.
///
class SRC_API CyclicBuff
{
    ///
    /// \brief g_buffLen
    ///
    static constexpr size_t g_buffLen = 1'000'000; // 1Mo

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
    /// \brief CyclicBuff
    /// is a buffer in memory.
    /// \param size
    /// [in] is constant size of the granted buffer.
    ///
    CyclicBuff( size_t size = g_buffLen );

    CyclicBuff( const CyclicBuff& buff ) = delete;

    ///
    /// \param buff
    ///
    CyclicBuff( CyclicBuff&& buff )      = default;

    ///
    /// \brief operator =
    /// \param buff
    /// \return
    ///
    CyclicBuff& operator=( const CyclicBuff& buff ) = delete;

    ///
    /// \brief operator =
    /// \param buff
    /// \return
    ///
    CyclicBuff& operator=( CyclicBuff&& buff ) = delete;

    ~CyclicBuff();

    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    void write( const unsigned char* data, size_t len );

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( unsigned char* data, size_t len );

    ///
    /// \brief close
    ///
    void close() const;

    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const;

  private:
    unsigned char* const m_buff;
    const size_t m_buffLen;
    size_t m_writeHead = 0;
    size_t m_readHead  = 0;
    mutable bool m_outputSensorWantsToClose = false;
    mutable bool m_inputSensorClose         = false;
};

///
/// \brief The Ram class
/// allows user to transfer sensor data through the memory (pipe tube).
///
class SRC_API Ram : public InputOutputInterface
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
    /// \brief Ram
    /// constructed from shared communication (InputSensor and OutputSensor) data container.
    /// \param buff
    /// [in] is a cyclic buffer already instancied.
    ///
    Ram( CyclicBuff& buff );

  protected:
    ///
    /// \brief close
    /// @copydoc Interface::close()
    ///
    void close() const override;

    ///
    /// \brief isOpen
    /// \return
    /// @copydoc Interface::isOpen()
    ///
    bool isOpen() const override;

    ///
    /// \brief isEnd
    /// \return
    /// @copydoc Interface::isEnd()
    ///
    bool isEnd() const override;

    ///
    /// \brief write
    /// \param data
    /// \param len
    /// @copydoc Interface::write()
    ///
    void write( const unsigned char* data, size_t len ) const override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    /// @copydoc Interface::read()
    ///
    void read( unsigned char* data, size_t len ) const override;

  private:
    CyclicBuff& m_buff;
};

} // namespace io
} // namespace hub
