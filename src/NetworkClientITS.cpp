#include "NetworkClientITS.h"

NetworkClientITS::NetworkClientITS() {

}

void NetworkClientITS::connect(const std::string &host, const std::string &service,
               const int timeout_sec) {
                   networkClient_.connect(host, service, boost::asio::chrono::seconds(timeout_sec));
                    requestConnection();
               }

std::string NetworkClientITS::getData() {
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

 bool NetworkClientITS::isOpened() {
   return networkClient_.isSocketOpened() && is_connected_;

 }

std::string NetworkClientITS::getDataFromServer(std::string str){
   networkClient_.write_line(str, boost::asio::chrono::seconds(10));
   std::string line;
    for (;;)
    {
      line = networkClient_.read_line(boost::asio::chrono::seconds(10));
      const byte* byte_line = reinterpret_cast<const byte*>(line.c_str());
      
      const size_t s = *reinterpret_cast<const size_t*>(byte_line + 2);
      std::cout << (size_t)byte_line[0] << ' ' << (size_t)byte_line[1] << ' ' << s << ' ' << line.size() << std::endl; 

      // Keep going until we get back the line that was sent.
      //if (line == test_msg_)
        break;
    }
    
    return line;
 }

 void NetworkClientITS::requestConnection() {
     std::stringstream ss;
     ss << (byte)0x79;
     ss << (byte)0x90;
     const std::string line = getDataFromServer(ss.str());
     if((byte)line[0] == (byte)0x79 && (byte)line[1] == (byte)0x91) {
            is_connected_ = true;
     }
 }