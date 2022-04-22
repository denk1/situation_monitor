#include "ROI.h"

ROI::ROI() {
    
}

void ROI::connect() {
    try {
        networkClient_.connect("localhost", "15555", boost::asio::chrono::seconds(10));
    } catch( boost::system::system_error err) {
        std::cerr << "connection error: " << err.what() << std::endl; 
    }
}

bool ROI::getData() {
    if(networkClient_.isOpened())
        return networkClient_.getData();
    return false;
}