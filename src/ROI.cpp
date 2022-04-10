#include "ROI.h"

ROI::ROI() {
    networkClient_.connect("localhost", "15555", boost::asio::chrono::seconds(10));
}

bool ROI::getData() {
    return networkClient_.getData();
}