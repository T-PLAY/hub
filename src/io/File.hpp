/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/13

#pragma once

#include <exception>

#include <stdexcept>
#include <string>

#include "core/Macros.hpp"
#include "io/Header.hpp"

namespace hub {
namespace io {

///
/// \brief The File class
/// is an encloser of a std file stream allowing to record or play acquisition streams in offline.
/// This class can be use to play recorded stream as a real sensor stream.
///
class SRC_API File
{
  public:
    ///
    /// \brief getHeader
    /// \return
    ///
    const Header& getHeader() const;

    ///
    /// \brief getFilePath
    /// \return
    ///
    const std::string & getFilePath() const;

  protected:
    ///
    /// \brief File
    /// constructor from a std::fstream, if you use an InputSensor uses the std::ios::in constructor
    /// option, if you use an OutputSensor the std::ios::out option when creating a std::fstream.
    /// \param file
    /// is a movable fstream you want to use as a sensor communication bus.
    ///
    explicit File( const std::string& filePath );

    ///
    /// \brief File
    /// \param filePath
    ///
    explicit File( const char* filePath );

    ///
    /// \param fileIO
    ///

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
    io::Header m_header;

    ///
    /// \brief m_filePath
    ///
    const std::string m_filePath;
};

} // namespace io
} // namespace hub
