
#include "io/OutputStream.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

OutputStream::OutputStream( const std::string& streamName, net::ClientSocket&& clientSocket
                            //                            std::function<void()> onServerClosed
                            ) :
    m_clientSocket( std::move( clientSocket ) )
//    m_onServerClosed(onServerClosed)
{
    //    std::cout << "[OutputStream] OutputStream() m_clientSocket : " << &m_clientSocket <<
    //    std::endl;

    Output::write( net::ClientSocket::Type::STREAMER );

    Output::write( streamName );

    net::ClientSocket::Message mess;
    m_clientSocket.read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        m_clientSocket.close();
        throw net::Socket::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );

    //    std::cout << "[OutputStream] OutputStream() m_clientSocket : " << &m_clientSocket <<
    //    std::endl;
    m_thread = std::make_unique<std::thread>( [&]() {
        //        std::cout << "[OutputStream][thread] OutputStream() m_clientSocket : " <<
        //        &m_clientSocket << std::endl;
        hub::net::ClientSocket::Message message;
        assert( m_clientSocket.isOpen() );
        //        std::cout << "[OutputStream][thread] m_clientSocket : " << &m_clientSocket <<
        //        std::endl;
        m_clientSocket.read( message );
        //        assert( message == net::ClientSocket::Message::SERVER_CLOSED );
        if ( message == net::ClientSocket::Message::SERVER_CLOSED ) {

            std::cout << "[OutputStream] server closed" << std::endl;
            m_serverClosed = true;
        }
        else if ( message == net::ClientSocket::Message::STREAMER_CLOSED ) {
            std::cout << "[OutputStream] streamer closed" << std::endl;
            m_streamerClosed = true;
        }
        else { assert( false ); }

        //        std::cout << "[OutputStream][thread] readed m_clientSocket : " << &m_clientSocket
        //        << std::endl;
        if ( m_clientSocket.isOpen() )
            m_clientSocket.write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
        //        std::cout << "[OutputStream][thread] wrote m_clientSocket : " << &m_clientSocket
        //        << std::endl;

        //        m_clientSocket.close();
        //        OutputStream::close();
        //        close();

        //        if ( m_clientSocket.isOpen() ) {
        //            assert( m_clientSocket.isOpen() );
        //        m_clientSocket.write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
        //        Output::write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
        //        std::cout << "-------------------------------" << std::endl;
        //        }

        //        if (m_onServerClosed) m_onServerClosed();

        //                assert(m_clientSocket.isOpen());
        //                m_clientSocket.write(net::ClientSocket::Message::OUTPUT_STREAM_CLOSED);

        //                if (m_clientSocket.isOpen())
        //                    m_clientSocket.close();
        //                m_serverClosed = true;
        //                assert(isOpen());
        //                close();

        //            assert(OutputStream::isOpen());
        //            OutputStream::close();
    } );
}

OutputStream::OutputStream( OutputStream&& outputStream ) :
    m_clientSocket( std::move( outputStream.m_clientSocket ) ),
    m_thread( std::move( outputStream.m_thread ) ),
    m_moved( outputStream.m_moved )
//    m_onServerClosed(std::move(outputStream.m_onServerClosed))
{
    outputStream.m_moved = true;
}

OutputStream::~OutputStream() {

    if ( !m_moved ) {
        //        std::cout << "[OutputStream] ~OutputStream() started" << std::endl;
        assert( !OutputStream::isOpen() );
        //        if ( OutputStream::isOpen() ) OutputStream::close();
        //        assert( !OutputStream::isOpen() );
        //        std::cout << "[OutputStream] ~OutputStream() ended" << std::endl;
        assert( m_thread->joinable() );
        m_thread->join();
    }
}

//    std::cout << "[OutputStream] ~OutputStream()" << std::endl;
//    net::ClientSocket::Message mess;
//    if ( m_clientSocket.isOpen() ) {
//        m_clientSocket.read( mess );
//        assert( mess == net::ClientSocket::Message::CLOSED );
//    }
//    //    if ( mess != net::ClientSocket::Message::CLOSED ) {
//    // #ifdef DEBUG_SOCKET
//    //        DEBUG_MSG( getHeader( m_fdSock ) << "[OutputStream] exception sensor '" <<
//    streamName
//    //                                         << "' is not attached to server" );
//    // #endif
//    //        throw net::ClientSocket::exception(
//    //            ( std::string( "stream '" ) + streamName + "' is not attached to server"
//    ).c_str()
//    //            );
//    //    }
//    //    assert( mess == net::ClientSocket::Message::OK );
//}

} // namespace io
} // namespace hub
