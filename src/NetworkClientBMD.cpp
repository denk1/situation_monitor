#include "NetworkClientBMD.h"
#include <boost/smart_ptr/make_shared.hpp>

using namespace std::chrono_literals;

NetworkClientBMD::NetworkClientBMD(): timeout_(10) {

}

void NetworkClientBMD::connect(const std::string &host, const std::string &service,
               const int timeout_sec) {
                   host_ = host;
                   service_ = service;
                   timeout_ = timeout_sec;
                   networkClient_ = boost::make_shared<NetworkClient>();
                   networkClient_->connect(host, service, boost::asio::chrono::seconds(timeout_sec));
                   requestConnection();
               }

void NetworkClientBMD::requestConnection() {
     std::stringstream ss;
     ss << (byte)0x79;
     ss << (byte)0x95;
     const std::string line = getDataFromServer(ss.str());
     if((byte)line[0] == (byte)0x79 && (byte)line[1] == (byte)0x96) {
            std::stringstream ss2;
            ss2 << (byte)0x79;
            ss2 << (byte)0x97;
            networkClient_->write_line(ss2.str(), boost::asio::chrono::seconds(10));
            is_connected_ = true;
     }
 }

 void NetworkClientBMD::sendData() {
   
}

 bool NetworkClientBMD::isOpened() {
   return networkClient_->isSocketOpened() && is_connected_;

 }

std::string NetworkClientBMD::getDataFromServer(std::string str){
   size_t s_str = str.size();
   networkClient_->write_line(str, boost::asio::chrono::seconds(10));
   std::string line;
    for (;;)
    {
      try {
        line = networkClient_->read_line(boost::asio::chrono::seconds(10));
        const byte* byte_line = reinterpret_cast<const byte*>(line.c_str());
      
        const size_t s = *reinterpret_cast<const size_t*>(byte_line + 2);
        std::cout << (size_t)byte_line[0] << ' ' << (size_t)byte_line[1] << ' ' << s << ' ' << line.size() << std::endl; 

        // Keep going until we get back the line that was sent.
        //if (line == test_msg_)
      }
      catch(boost::system::system_error er) {
        std::cout <<  er.what() << std::endl;
        networkClient_.reset(new NetworkClient());
        networkClient_->connect(host_, service_, boost::asio::chrono::seconds(timeout_));
        requestConnection();
        std::this_thread::sleep_for(100ms);
      }
      break;
    }
    
    return line;
 }