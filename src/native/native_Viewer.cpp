

#include "client/Viewer.hpp"

#define HUB_CPP_SOURCE
#include "native_Viewer.hpp"

namespace hub {
namespace native {

client::Viewer*
createViewer( const char* name, client::ViewerHandler* viewerHandler, const char* ipv4, int port ) {

    client::Viewer* viewer = new client::Viewer( name, std::move( *viewerHandler ), ipv4, port );
    return viewer;
}

void freeViewer( client::Viewer* viewer ) {
    assert( viewer != nullptr );
    delete viewer;
}

void viewer_setIpv4( client::Viewer* viewer, const char* ipv4 ) {
    viewer->setIpv4( ipv4 );
}

void viewer_setPort( client::Viewer* viewer, int port ) {
    viewer->setPort( port );
}

int viewer_getPort( const client::Viewer* viewer ) {
    assert( viewer != nullptr );
    return viewer->getPort();
}

void viewer_getIpv4( const client::Viewer* viewer, char* ipv4 ) {
    const auto& ipv4Str = viewer->getIpv4();
    const int len       = ipv4Str.size(); // todo
    memcpy( ipv4, ipv4Str.data(), len + 1 );
    ipv4[len] = 0;
}

bool viewer_isConnected( const client::Viewer* viewer ) {
    return viewer->isConnected();
}

int viewer_nStream( const client::Viewer* viewer ) {
    return viewer->nStream();
}

int viewer_nStreaming( const client::Viewer* viewer ) {
    return viewer->nStreaming();
}

} // namespace native
} // namespace hub
