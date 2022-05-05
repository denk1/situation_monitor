#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include<OGRE/Ogre.h>


class MeshObject {
public:
    typedef std::vector<Ogre::Matrix4> vec_mat4;
    void set_str_buff(const std::string& str_buff);
    void convert_to_scene_obj();
    
private:
    std::string buff_scene_;
    vec_mat4 buff_vec_mat4_;

};
#endif