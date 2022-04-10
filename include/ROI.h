#ifndef ROI_H
#define ROI_H

#include "NetworkClient.h"

class ROI {
public:
    ROI();
    bool getData();
private:
    NetworkClient networkClient_;
 

};
#endif