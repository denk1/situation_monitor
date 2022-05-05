#include "MeshObject.h"
#include <iostream>

using byte = unsigned char;

void MeshObject::set_str_buff(const std::string& str_buff) {
    buff_scene_ = str_buff;
}

void MeshObject::convert_to_scene_obj() {
    byte s_in_bytes[8];
    Ogre::Real mat4[16];
    //byte 
    
    std::copy(buff_scene_.begin() + 2, buff_scene_.begin() + 10, s_in_bytes);
    const size_t* s = reinterpret_cast<const size_t*>(s_in_bytes);
    for(size_t i = 0; i < *s; i++) {

    }
}