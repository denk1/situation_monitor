#ifndef NETWORK_CLIENT_ITS_H
#define NETWORK_CLIENT_ITS_H
#include "NetworkClient.h"


using byte = unsigned char;

class NetworkClientITS {
    public:
        NetworkClientITS();
        void connect(const std::string &host, const std::string &service,
               const int timeout);
        std::string getData();
        bool isOpened();

    private:
        void requestConnection();
        std::string getDataFromServer(std::string str);
        bool is_connected_ = false;
        
        NetworkClient::NetworkClientPtr networkClient_;
};
#endif