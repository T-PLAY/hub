#pragma once

#include <fstream>
#include <memory>

#include "Interface.hpp"

namespace hub {
namespace io {

class SRC_API File : public InputOutputInterface
{
    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

  public:
    File( std::fstream&& file );
    File( const File& fileIO ) = delete;
    File( File&& fileIO )      = default;

    ~File();

    void close() override;

    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;

  protected:
  private:
    std::unique_ptr<std::fstream> m_file;
};

} // namespace io
} // namespace hub
