#include "ROI.h"

ROI::ROI() {
    try {
        networkClient_.connect("localhost", "15555", boost::asio::chrono::seconds(10));
    } catch( boost::system::system_error err) {
        std::cerr << "connection error: " << err.what() << std::endl; 
    }
}

bool ROI::getData() {
    return networkClient_.getData();
}