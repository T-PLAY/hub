#pragma once

#include <fstream>
#include <memory>

#include "Interface.hpp"

namespace hub {
namespace io {

///
/// \brief The File class
/// is an encloser of a std file stream allowing to record or play acquisition streams in offline.
/// This class can be use to play recorded stream as a real sensor stream.
///
class SRC_API File : public InputOutputInterface
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
    /// constructor from a std::fstream, if you use an InputSensor uses the std::ios::in constructor option,
    /// if you use an OutputSensor the std::ios::out option when creating a std::fstream.
    /// \param file
    /// is a movable fstream you want to use as a sensor communication bus.
    ///
    File( std::fstream&& file );

    File( const File& fileIO ) = delete;

    ///
    /// \param fileIO
    ///
    File( File&& fileIO)      = default;

    ~File();

  protected:

    ///
    /// \brief close
    /// close the file
    /// @copydoc Interface::close()
    ///
    void close() const override;

    ///
    /// \brief isOpen
    /// check if file is openning.
    /// \return
    /// true if file is openned.\n
    /// false if file is closed.
    /// @copydoc Interface::isOpen()
    ///
    bool isOpen() const override;

    ///
    /// \brief isEnd
    /// check if end of file.
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
    ///
    /// \brief m_file
    ///
    std::unique_ptr<std::fstream> m_file;
};

} // namespace io
} // namespace hub
