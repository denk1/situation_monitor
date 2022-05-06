#ifndef ROI_H
#define ROI_H

#include "NetworkClient.h"
#include "MeshObject.h"


class ROI {
public:
    ROI();
    void connect();
    bool getData(MeshObject* ptrMeshObject);
private:
    NetworkClient networkClient_;
    
 

};
#endif