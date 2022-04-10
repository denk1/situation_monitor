#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/write.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

class NetworkClient {
    public:
        NetworkClient();
        void connect(const std::string &host, const std::string &service,
               boost::asio::chrono::steady_clock::duration timeout);
        std::string read_line(boost::asio::chrono::steady_clock::duration timeout);
        void write_line(const std::string &line,
                  boost::asio::chrono::steady_clock::duration timeout);
        bool getData();

    private:
        void run(boost::asio::chrono::steady_clock::duration timeout);
        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::socket socket_;
        std::string input_buffer_;
        const std::string test_msg_ = "msg";
};
#endif