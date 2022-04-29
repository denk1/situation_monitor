#ifndef ROI_H
#define ROI_H

#include "NetworkClient.h"

class ROI {
public:
    ROI();
    void connect();
    bool getData();
private:
    NetworkClient networkClient_;
    std::string str_buff_data_;
 

};
#endif