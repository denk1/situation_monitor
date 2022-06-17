#include "NetworkClient.h"
#include <sstream>

using boost::asio::ip::tcp;
using boost::lambda::_1;
using boost::lambda::_2;
using boost::lambda::bind;
using boost::lambda::var;
using namespace std::chrono_literals;


NetworkClient::NetworkClient(): socket_(io_context_), read_n_() {

}

void NetworkClient::connect(const std::string &host, const std::string &service,
               boost::asio::chrono::steady_clock::duration timeout) {
    // Resolve the host name and service to a list of endpoints.
    tcp::resolver::results_type endpoints =
        tcp::resolver(io_context_).resolve(host, service);

    // Start the asynchronous operation itself. The boost::lambda function
    // object is used as a callback and will update the ec variable when the
    // operation completes. The blocking_udp_client.cpp example shows how you
    // can use boost::bind rather than boost::lambda.
    boost::system::error_code ec;
    boost::asio::async_connect(socket_, endpoints, var(ec) = _1);

    // Run the operation until it completes, or until the timeout.
    run(timeout);

    // Determine whether a connection was successfully established.
    if (ec)
      throw boost::system::system_error(ec);

    //requestConnection();

}

std::string NetworkClient::read_line(boost::asio::chrono::steady_clock::duration timeout)
{
    // Start the asynchronous operation. The boost::lambda function object is
    // used as a callback and will update the ec variable when the operation
    // completes. The blocking_udp_client.cpp example shows how you can use
    // boost::bind rather than boost::lambda.
    
    
    boost::asio::async_read(socket_, boost::asio::dynamic_buffer(input_buffer_), boost::asio::transfer_at_least(1), 
                                    bind(&NetworkClient::handle_read, shared_from_this(), boost::placeholders::_1, boost::placeholders::_2));                              
    // Run the operation until it completes, or until the timeout.
    run(timeout);

    std::string line(input_buffer_.substr(0, read_n_));
    input_buffer_.erase(0, read_n_);
    return line;
}

void NetworkClient::handle_read(const boost::system::error_code& ec, std::size_t n) {
    // Determine whether the read completed successfully.
    if (ec)
      throw boost::system::system_error(ec);
    read_n_ = n;
}


void NetworkClient::write_line(const std::string &line,
                  boost::asio::chrono::steady_clock::duration timeout)
{
    std::string data = line + "\n";

    // Start the asynchronous operation. The boost::lambda function object is
    // used as a callback and will update the ec variable when the operation
    // completes. The blocking_udp_client.cpp example shows how you can use
    // boost::bind rather than boost::lambda.
    boost::system::error_code ec;
    boost::asio::async_write(socket_, boost::asio::buffer(data), bind(&NetworkClient::handle_write, shared_from_this(), boost::placeholders::_1));

    // Run the operation until it completes, or until the timeout.
    run(timeout);

    // Determine whether the read completed successfully.
    
}

void NetworkClient::handle_write(const boost::system::error_code& ec) {
    if (ec)
      throw boost::system::system_error(ec);
}

void NetworkClient::run(boost::asio::chrono::steady_clock::duration timeout)
{
    // Restart the io_context, as it may have been left in the "stopped" state
    // by a previous operation.
    io_context_.restart();

    // Block until the asynchronous operation has completed, or timed out. If
    // the pending asynchronous operation is a composed operation, the deadline
    // applies to the entire operation, rather than individual operations on
    // the socket.
    io_context_.run_for(timeout);

    // If the asynchronous operation completed successfully then the io_context
    // would have been stopped due to running out of work. If it was not
    // stopped, then the io_context::run_for call must have timed out.
    if (!io_context_.stopped())
    {
      // Close the socket to cancel the outstanding asynchronous operation.
      socket_.close();

      // Run the io_context again until the operation completes.
      io_context_.run();
    }
}

std::string NetworkClient::getData() {
    std::stringstream ss;
    std::string str_data;
    boost::asio::chrono::steady_clock::time_point time_sent =
          boost::asio::chrono::steady_clock::now();
    

    ss << (byte)0x79;
    ss << (byte)0x88;

    str_data = getDataFromServer(ss.str());
    boost::asio::chrono::steady_clock::time_point time_received =
      boost::asio::chrono::steady_clock::now();
    std::cout << "Round trip time: ";
    std::cout << boost::asio::chrono::duration_cast<
      boost::asio::chrono::microseconds>(
        time_received - time_sent).count();
    std::cout << " microseconds\n";
    return str_data;
}

 bool NetworkClient::isOpened() {
   return socket_.is_open() && is_connected_;

 }

std::string NetworkClient::getDataFromServer(std::string str){
   write_line(str, boost::asio::chrono::seconds(10));
   std::string line;
    for (;;)
    {
      line = read_line(boost::asio::chrono::seconds(10));
      const byte* byte_line = reinterpret_cast<const byte*>(line.c_str());
      
      const size_t s = *reinterpret_cast<const size_t*>(byte_line + 2);
      std::cout << (size_t)byte_line[0] << ' ' << (size_t)byte_line[1] << ' ' << s << ' ' << line.size() << std::endl; 

      // Keep going until we get back the line that was sent.
      //if (line == test_msg_)
        break;
    }
    
    return line;
 }

 void NetworkClient::requestConnection() {
     std::stringstream ss;
     ss << (byte)0x79;
     ss << (byte)0x90;
     const std::string line = getDataFromServer(ss.str());
     if((byte)line[0] == (byte)0x79 && (byte)line[1] == (byte)0x91) {
            is_connected_ = true;
     }
 }

 bool NetworkClient::isSocketOpened() {
   return socket_.is_open();
 }

 void NetworkClient::stopSocket() {
    io_context_.stop();
 }