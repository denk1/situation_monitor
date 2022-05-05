#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include<OGRE/Ogre.h>


class MeshObject {
public:
    void set_str_buff(const std::string& str_buff);
    void convert_to_scene_obj();
    
private:
    std::string buff_scene_;

};
#endif