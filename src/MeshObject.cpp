#include "MeshObject.h"
#include <iostream>

using byte = unsigned char;

void MeshObject::set_str_buff(const std::string& str_buff) {
    buff_scene_ = str_buff;
}

void MeshObject::convert_to_scene_obj() {
    byte s_in_bytes[8];
    byte* byte_mat4;
    Ogre::Real mat4[16];
    byte_mat4 = reinterpret_cast<byte*>(mat4);
    
    std::copy(buff_scene_.begin() + 2, buff_scene_.begin() + 10, s_in_bytes);
    const size_t* s = reinterpret_cast<const size_t*>(s_in_bytes);
    buff_vec_mat4_.clear();
    for(size_t i = 0; i < *s; i++) {
        size_t size_of_mat4 = i * 64;
        std::copy(buff_scene_.begin() + 2 + size_of_mat4, buff_scene_.begin() + 64 + size_of_mat4 , byte_mat4);
        buff_vec_mat4_.emplace_back(
                                        mat4[0], mat4[1], mat4[2], mat4[3], 
                                        mat4[4], mat4[5], mat4[6], mat4[7],
                                        mat4[8], mat4[9], mat4[10], mat4[11],
                                        mat4[12], mat4[13], mat4[14], mat4[15]
                                    );
    }
}