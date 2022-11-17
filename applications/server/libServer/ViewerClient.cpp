#include "ViewerClient.hpp"

#include "Server.hpp"
#include "StreamerClient.hpp"

ViewerClient::ViewerClient( Server& server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    server.addViewer( this );

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            while ( m_socket.isOpen() ) {
                m_mtxSocket.lock();
                m_socket.write( hub::net::ClientSocket::Message::PING );
                m_mtxSocket.unlock();
                // ping viewer client to know if the connection of this one still alive
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        }
        catch ( std::exception& e ) {
            m_mtxSocket.unlock();

//            s_mtxCout.lock();
            std::cout << headerMsg() << "catch viewer exception : " << e.what() << std::endl;
            //            delete this;
//            m_server.delViewer( this );
            std::thread( [this]() { delete this; } ).detach();
        }
    } );
    //    thread.detach();

    printStatusMessage("new viewer");
//    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
//              << "new viewer" << m_server.getStatus() << std::endl;
//    std::cout << "-------------------------------------------------------------------------"
//                 "--------------------"
//              << std::endl;
}

ViewerClient::~ViewerClient() {
//    s_mtxCout.lock();

    assert( m_thread.joinable() );
    m_thread.join();

    m_server.delViewer(this);
    printStatusMessage("del viewer");
//    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
//              << "del viewer" << m_server.getStatus() << std::endl;
//    std::cout << "-------------------------------------------------------------------------"
//                 "--------------------"
//              << std::endl;
//    m_mtxSocket.unlock();
//    s_mtxCout.unlock();
}

std::string ViewerClient::headerMsg() const {
    return Client::headerMsg() + "[ViewerClient] ";
}

void ViewerClient::notifyNewStreamer(const StreamerClient &streamer) const {

    m_mtxSocket.lock();
    m_socket.write( hub::net::ClientSocket::Message::NEW_STREAMER );
//    m_socket.write( (syncStream == "") ?(streamer.getStreamName()) :(syncStream + " <- " + streamer.getStreamName()) );
    m_socket.write( streamer.getStreamName());
    m_socket.write( streamer.getInputSensor().m_spec );
//    m_socket.write( syncStream );
    m_mtxSocket.unlock();
}

void ViewerClient::notifyDelStreamer(const StreamerClient &streamer) const {
    std::cout << headerMsg() << "notifyDelStreamer " << streamer.getStreamName() << std::endl;
    try {
        m_mtxSocket.lock();
        m_socket.write( hub::net::ClientSocket::Message::DEL_STREAMER );
        m_socket.write( streamer.getStreamName() );
        m_socket.write( streamer.getInputSensor().m_spec );
//        m_socket.write( syncStream );
        m_mtxSocket.unlock();
    }
    catch ( std::exception& e ) {
        m_mtxSocket.unlock();
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        //        delete this;
        //    m_server.delViewer( this );
            std::thread( [this]() { delete this; } ).detach();
    }
}
