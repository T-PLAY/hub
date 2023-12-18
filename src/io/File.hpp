#pragma once

//#include <fstream>
//#include <memory>
#include <exception>

#include <string>
#include <stdexcept>

#include "core/Macros.hpp"
//#include "InputOutput.hpp"
#include "io/Header.hpp"

namespace hub {
namespace io {

///
/// \brief The File class
/// is an encloser of a std file stream allowing to record or play acquisition streams in offline.
/// This class can be use to play recorded stream as a real sensor stream.
///
//class SRC_API File : public InputOutput
class SRC_API File
{
    //  public:
  public:
    const Header &getHeader() const;

  protected:
    ///
    /// \brief File
    /// constructor from a std::fstream, if you use an InputSensor uses the std::ios::in constructor
    /// option, if you use an OutputSensor the std::ios::out option when creating a std::fstream.
    /// \param file
    /// is a movable fstream you want to use as a sensor communication bus.
    ///
//    explicit File( std::fstream&& file );
//    explicit File( std::basic_ios<char>&& file );
    explicit File( const std::string & filePath );

    explicit File( const char * filePath);

//    File( const File& ) = delete;

    ///
    /// \param fileIO
    ///
//    File( File&& file ) = default;

//    ~File();

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
    ///
    /// \brief m_file
    ///
//    std::unique_ptr<std::fstream> m_file;
//    std::unique_ptr<std::basic_ios<char>> m_file;
    io::Header m_header;
    const std::string m_filePath;
};

} // namespace io
} // namespace hub
