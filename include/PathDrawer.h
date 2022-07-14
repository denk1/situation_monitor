#ifndef PATH_DRAWER_H
#define PATH_DRAWER_H
#include "OgreApplicationContext.h"
class PathDrawer
{
public:
    PathDrawer(float width, float height): x_roi_(width), y_roi_(height) {

    }

    void setPathData(uint* data, std::vector<float>& path ) {
        for(size_t i = 0; i < path.size(); i+=2) 
        {
            data[  (size_t)path[i + 1] * (size_t)x_roi_ + (size_t)x_roi_/2 + (size_t)path[i]];
        }
        /*
        for(size_t y = 0; y < (size_t)y_roi_; ++y) {
            for(size_t x = 0; x < (size_t)x_roi_; ++x) {
                data[  y * (size_t)x_roi_ + (size_t)x_roi_/2]   = 0xFF0000FF;
            }
        }
        */
    }
private:
    const float x_roi_, y_roi_;
};

#endif