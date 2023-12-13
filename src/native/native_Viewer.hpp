#pragma once

#include "core/Macros.hpp"

// #include "client/Viewer.hpp"

namespace hub {

#ifndef CPP_SOURCE
namespace client {
    class Viewer;
    class ViewerHandler;
}
#endif

///
/// @brief native
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    SRC_API client::Viewer*
    createViewer( const char* name,
                  client::ViewerHandler* viewerHandler,
                  // const char* ipv4 = input::InputStream::s_defaultIpv4.c_str(),
                  // int port         = input::InputStream::s_defaultPort );
                  const char* ipv4 = "127.0.0.1",
                  int port         = 4042 );

    ///
    /// \brief freeViewer
    /// \param viewer
    ///
    SRC_API void freeViewer( client::Viewer* viewer );

    ///
    /// \brief viewer_setIpv4
    /// \param viewer
    /// \param ipv4
    ///
    SRC_API void viewer_setIpv4( client::Viewer* viewer, const char* ipv4 );

    ///
    /// \brief viewer_setPort
    /// \param viewer
    /// \param port
    ///
    SRC_API void viewer_setPort( client::Viewer* viewer, int port );

    ///
    /// \brief viewer_getPort
    /// \param viewer
    /// \return
    ///
    SRC_API int viewer_getPort( const client::Viewer* viewer );

    ///
    /// \brief viewer_getIpv4
    /// \param viewer
    /// \param ipv4
    ///
    SRC_API void viewer_getIpv4( const client::Viewer* viewer, char* ipv4 );

    ///
    /// \brief viewer_isConnected
    /// \param viewer
    /// \return
    ///
    SRC_API bool viewer_isConnected( const client::Viewer* viewer );

    int viewer_nStream(const client::Viewer* viewer);

    int viewer_nStreaming(const client::Viewer* viewer);

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
