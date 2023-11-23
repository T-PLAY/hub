#pragma once

#include <string>

//#include "io/StreamInterface.hpp"
//#include "net/ClientSocket.hpp"
#include "core/io/InputOutput.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {


class InputOutputSocket : public InputOutput<>, public net::ClientSocket
{
  public:
    using InputOutput<>::read;
    using InputOutput<>::write;

    InputOutputSocket(net::ClientSocket && clientSocket) : net::ClientSocket(std::move(clientSocket)) {}
    InputOutputSocket(InputOutputSocket &&) = default;
    InputOutputSocket(const InputOutputSocket &) = delete;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::read(data, size);
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::write(data, size);
    }

    void close() override {
        disconnect();
    };

    bool isOpen() const override {
        return isConnected();
//        return true;
    }

    bool isEnd() const override { return  false; }

    void clear() override {
    }

    void setRetain(bool retain) override {
    }

};

} // namespace io
} // namespace hub
