

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <iostream>
#include <thread>

std::string read_( boost::asio::ip::tcp::socket& socket ) {
    boost::asio::streambuf buf;
    boost::asio::read_until( socket, buf, "\n" );
    std::string data = boost::asio::buffer_cast<const char*>( buf.data() );
    return data;
}
void send_( boost::asio::ip::tcp::socket& socket, const std::string& message ) {
    const std::string msg = message + "\n";
    boost::asio::write( socket, boost::asio::buffer( message ) );
}

int main() {

    // server side
    std::thread thread( []() {
        boost::asio::io_service io_service;
        // listen for new connection
        boost::asio::ip::tcp::acceptor acceptor_(
            io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), 1234 ) );
        // socket creation
        boost::asio::ip::tcp::socket socket_( io_service );
        // waiting for connection
        acceptor_.accept( socket_ );
        // read operation
        std::string message = read_( socket_ );
        std::cout << message << std::endl;
        // write operation
        send_( socket_, "Hello From Server!" );
        std::cout << "Servent sent Hello message to Client!" << std::endl;
    } );

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    // client side
    {
        boost::asio::io_service io_service;
        // socket creation
        boost::asio::ip::tcp::socket socket( io_service );
        // connection
        socket.connect( boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string( "127.0.0.1" ), 1234 ) );
        // request/message from client
        const std::string msg = "Hello from Client!\n";
        boost::system::error_code error;
        boost::asio::write( socket, boost::asio::buffer( msg ), error );
        if ( !error ) { std::cout << "Client sent hello message!" << std::endl; }
        else { std::cout << "send failed: " << error.message() << std::endl; }
        // getting response from server
        boost::asio::streambuf receive_buffer;
        boost::asio::read( socket, receive_buffer, boost::asio::transfer_all(), error );
        if ( error && error != boost::asio::error::eof ) {
            std::cout << "receive failed: " << error.message() << std::endl;
        }
        else {
            const char* data = boost::asio::buffer_cast<const char*>( receive_buffer.data() );
            std::cout << data << std::endl;
        }
    }

    thread.join();

    return 0;
}
