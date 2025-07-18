
#pragma once

#include "core/Base.hpp"

namespace hub {
namespace server {

///
/// \brief The ServerI class
/// is an interface specifies basic server features.
/// You can implement your own server, please uses this interface.
/// That allow us to check the efficiency of your proposal and integrate to the main branch.
///
class SRC_API ServerI
{
public:

  ///
  /// \brief Start server routine.
  /// Routine is the process which accept new client and creates thread for each client life.
  ///
  virtual void run()   = 0;

  ///
  /// \brief Start server routine in different thread than the caller thread
  ///
  virtual void asyncRun()   = 0;

  ///
  /// \brief Stop the routine
  ///
  virtual void stop()  = 0;

  ///
  /// \brief Print info of server for example the number of connected clients.
  ///
  virtual void printStatus() const  = 0;

  ///
  /// \brief Server is running when routine started
  /// \return true if the routine is running\n
  /// false otherwise
  ///
  virtual bool running() const  = 0;

  ///
  /// \brief
  /// \return the number of connected sreamers
  ///
  virtual int nStreamer() const  = 0;

  ///
  /// \brief
  /// \return The number of connected clients
  ///
  virtual int nClient() const  = 0;


};

} // namespace server
} // namespace hub
