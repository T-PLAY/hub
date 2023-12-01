
#pragma once

#include <mutex>
#include <thread>

// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"

#include "Client2.hpp"

namespace hub {
namespace server {

/////
///// \brief The AskerClient class
/////
// class AskerClient : public Client
//{
//   private:
//     AskerClient( Server* server, int iClient, net::ClientSocket&& sock );
//     ~AskerClient();

//    std::string headerMsg() const override;

//    void end( io::StreamInterface::ServerMessage message ) override;

//  private:
//    std::thread m_thread;
////    net::ClientSocket m_sock;
//    io::InputOutputSocket m_sock;

//    friend class hub::Server;
//};

} // namespace server
} // namespace hub
