#ifndef PATH_DRAWER_H
#define PATH_DRAWER_H
#include "OgreApplicationContext.h"
#include "PathFinder.h"
class PathDrawer
{
public:
    PathDrawer(float width, float height): x_roi_(width), y_roi_(height) {

    }

    void setPathData(uint* data, const Path& path ) {
        const std::vector<float>& vPath = path.v_path_value; 
        for(size_t i = 0; i < vPath.size(); i+=2) 
        {
            data[  (size_t)vPath[i + 1] * (size_t)x_roi_ + (size_t)x_roi_/2 + (size_t)vPath[i]] = 0xFF0000FF;
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