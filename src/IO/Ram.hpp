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
    static constexpr size_t g_buffLen = 1'000'000;

    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

  public:
    CyclicBuff( size_t size = g_buffLen );
    CyclicBuff( const CyclicBuff& buff ) = delete;
    CyclicBuff( CyclicBuff&& buff )      = default;

    CyclicBuff& operator=( const CyclicBuff& buff ) = delete;
    CyclicBuff& operator=( CyclicBuff&& buff )      = delete;

    ~CyclicBuff();

    void write( const unsigned char* data, size_t len );
    void read( unsigned char* data, size_t len );

    void close() const;
    bool isOpen() const;
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
    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

  public:
    Ram( CyclicBuff& buff );

    void close() const override;
    bool isOpen() const override;
    bool isEnd() const override;

    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;

  protected:
  private:
    CyclicBuff& m_buff;
};

} // namespace io
} // namespace hub
