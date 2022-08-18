#pragma once

#include <fstream>

#include "IO.hpp"

namespace hub {

class SRC_API FileIO : public IO
{
    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

  public:
    FileIO( std::fstream&& file );
    FileIO( const FileIO& fileIO ) = delete;
    FileIO( FileIO&& fileIO )      = default;

    void close() override;

    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;

  protected:
  private:
    std::fstream& mFile;
};

} // namespace hub
