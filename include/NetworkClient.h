#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind/bind.hpp>
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

using byte = unsigned char;



class NetworkClient: public boost::enable_shared_from_this<NetworkClient> {
    public:
        NetworkClient();
        void connect(const std::string &host, const std::string &service,
               boost::asio::chrono::steady_clock::duration timeout);
        std::string read_line(boost::asio::chrono::steady_clock::duration timeout);
        void write_line(const std::string &line,
                  boost::asio::chrono::steady_clock::duration timeout);
        std::string getData();
        bool isOpened();
        bool isSocketOpened();
        void resetSocket();
        void stopSocket();

        typedef boost::shared_ptr<NetworkClient> NetworkClientPtr;

    private:
        void requestConnection();
        std::string getDataFromServer(std::string str);
        void run(boost::asio::chrono::steady_clock::duration timeout);
        void handle_read(const boost::system::error_code& ec, std::size_t n);
        void handle_write(const boost::system::error_code& ec);
        bool is_connected_ = false;
        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::socket socket_;
        std::string input_buffer_;
        size_t read_n_;
};
#endif