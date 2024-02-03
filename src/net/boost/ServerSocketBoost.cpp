#include "ServerSocketBoost.hpp"

#ifdef HUB_USE_BOOST

#    include <cstring>

namespace hub {
namespace net {
namespace boost {

ServerSocketBoost::ServerSocketBoost( int port ) :
    m_port( port ),
    m_acceptor( m_ioService,
                ::boost::asio::ip::tcp::endpoint( ::boost::asio::ip::tcp::v4(), m_port ) ) {}

ServerSocketBoost::~ServerSocketBoost() {}

ClientSocketBoost ServerSocketBoost::waitNewClient() {

    ::boost::asio::ip::tcp::socket socket( m_ioService );
    m_acceptor.accept( socket );

    return ClientSocketBoost( std::move( socket ) );
}

void ServerSocketBoost::initServer() {
    // Socket creation, service may be start (daemon)
}

int ServerSocketBoost::getPort() const {
    return m_port;
}

} // namespace boost
} // namespace net
} // namespace hub

#endif
