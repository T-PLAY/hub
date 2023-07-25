#pragma once

#include <fstream>
#include <memory>

#include "InputOutput.hpp"

namespace hub {
namespace io {

///
/// \brief The File class
/// is an encloser of a std file stream allowing to record or play acquisition streams in offline.
/// This class can be use to play recorded stream as a real sensor stream.
///
class SRC_API File : public InputOutput
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
    /// \brief File
    /// constructor from a std::fstream, if you use an InputSensor uses the std::ios::in constructor
    /// option, if you use an OutputSensor the std::ios::out option when creating a std::fstream.
    /// \param file
    /// is a movable fstream you want to use as a sensor communication bus.
    ///
    explicit File( std::fstream&& file );

    File( const File& fileIO ) = delete;

    ///
    /// \param fileIO
    ///
    File( File&& fileIO );

    ~File();

  protected:
    ///
    /// @copydoc InputOutput::close()
    ///
    void close() override;

    ///
    /// @copydoc InputOutput::isOpen()
    ///
    bool isOpen() const override;

    ///
    /// @copydoc InputOutput::isEnd()
    ///
    bool isEnd() const override;

    ///
    /// @copydoc InputOutput::write()
    ///
    void write( const unsigned char* data, size_t len ) override;

    ///
    /// @copydoc InputOutput::read()
    ///
    void read( unsigned char* data, size_t len ) override;

  private:
    ///
    /// \brief m_file
    ///
    std::unique_ptr<std::fstream> m_file;
};

} // namespace io
} // namespace hub
