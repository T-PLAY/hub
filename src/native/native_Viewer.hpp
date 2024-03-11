/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/07

#pragma once

#include "core/Macros.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE
namespace client {
class Viewer;
class ViewerHandler;
} // namespace client
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief createViewer
    /// \param name
    /// \param viewerHandler
    /// \param ipv4
    /// \param port
    /// \return
    ///
    SRC_API client::Viewer* createViewer( const char* name,
                                          client::ViewerHandler* viewerHandler,
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

    ///
    /// \brief viewer_nStream
    /// \param viewer
    /// \return
    ///
    SRC_API int viewer_nStream( const client::Viewer* viewer );

    ///
    /// \brief viewer_nStreaming
    /// \param viewer
    /// \return
    ///
    SRC_API int viewer_nStreaming( const client::Viewer* viewer );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
