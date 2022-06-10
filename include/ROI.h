#ifndef ROI_H
#define ROI_H

#include <memory>
#include "NetworkClient.h"
#include "MeshObject.h"


class ROI {
public:
    ROI();
    void connect();
    bool getData(std::shared_ptr<MeshObject> ptrMeshObject);
private:
    NetworkClient networkClient_;
};
#endif