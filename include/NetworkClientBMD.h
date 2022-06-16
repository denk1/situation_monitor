#ifndef NETWORK_CLIENT_BMD_H
#define NETWORK_CLIENT_BMD_H
#include "NetworkClient.h"


using byte = unsigned char;

class NetworkClientBMD {
    public:
        NetworkClientBMD();
        void connect(const std::string &host, const std::string &service,
               const int timeout);
        std::string getData();
        bool isOpened();
        std::string getDataFromServer(std::string str);

    private:
        void requestConnection();
        
        void sendData();
        bool is_connected_ = false;
        NetworkClient networkClient_;
        std::string host_;
        std::string service_;
        int timeout_;
};
#endif