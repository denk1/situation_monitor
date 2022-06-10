#ifndef ROI_H
#define ROI_H

#include <memory>
#include "NetworkClientITS.h"
#include "MeshObject.h"
#include "PathFinder.h"


class ROI {
public:
    ROI();
    void connect();
    bool getData(std::shared_ptr<MeshObject> ptrMeshObject);
private:
    NetworkClientITS networkClient_;
    PathFinder pathFinder_;
};
#endif