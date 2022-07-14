#ifndef ROI_H
#define ROI_H

#include <memory>
#include "NetworkClientITS.h"
#include "MeshObject.h"
#include "PathFinder.h"


class ROI {
public:
    ROI(float width, float height);
    void connect();
    bool getData(std::shared_ptr<MeshObject> ptrMeshObject);
    Path getPath();
private:
    NetworkClientITS networkClient_;
    PathFinder pathFinder_;
    const float width_, height_;
};
#endif