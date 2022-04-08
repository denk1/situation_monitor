#include "NetworkClient.h"

using boost::asio::ip::tcp;
using boost::lambda::_1;
using boost::lambda::_2;
using boost::lambda::bind;
using boost::lambda::var;

NetworkClient::NetworkClient(): socket_(io_context_) {

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

}

std::string NetworkClient::read_line(boost::asio::chrono::steady_clock::duration timeout)
  {
    // Start the asynchronous operation. The boost::lambda function object is
    // used as a callback and will update the ec variable when the operation
    // completes. The blocking_udp_client.cpp example shows how you can use
    // boost::bind rather than boost::lambda.
    boost::system::error_code ec;
    std::size_t n = 0;
    boost::asio::async_read_until(socket_,
                                  boost::asio::dynamic_buffer(input_buffer_),
                                  '\n', (var(ec) = _1, var(n) = _2));

    // Run the operation until it completes, or until the timeout.
    run(timeout);

    // Determine whether the read completed successfully.
    if (ec)
      throw boost::system::system_error(ec);

    std::string line(input_buffer_.substr(0, n - 1));
    input_buffer_.erase(0, n);
    return line;
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
    boost::asio::async_write(socket_, boost::asio::buffer(data), var(ec) = _1);

    // Run the operation until it completes, or until the timeout.
    run(timeout);

    // Determine whether the read completed successfully.
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