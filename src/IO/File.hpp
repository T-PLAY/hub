#pragma once

#include <fstream>

#include "IO/Interface.hpp"

namespace hub {
namespace io {

class SRC_API File : public hub::io::Interface
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

    void close() override;

    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;

  protected:
  private:
    std::fstream& mFile;
};

} // namespace io
} // namespace hub
