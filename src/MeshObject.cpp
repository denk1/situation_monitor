#include <iostream>

#include "MeshObject.h"
#include "SituationMonitor.h"

using byte = unsigned char;

MeshObject::MeshObject(SituationMonitor* ptrSituationMonitor): mPtrSituationMonitor_(ptrSituationMonitor) {
    
}

void MeshObject::set_str_buff(const std::string& str_buff) {
    buff_scene_ = str_buff;
}

void MeshObject::convert_to_scene_obj() {
    byte s_in_bytes[8];
    byte id_in_bytes[8];
    byte* byte_mat4;
    Ogre::Real mat4[16];
    byte_mat4 = reinterpret_cast<byte*>(mat4);
    
    std::copy(buff_scene_.begin() + 2, buff_scene_.begin() + 10, s_in_bytes);
    const size_t* s = reinterpret_cast<const size_t*>(s_in_bytes);
    buff_map_mat4_.clear();
    for(size_t i = 0; i < *s; i++) {
        size_t size_of_mat4 = i * 72;
        std::copy(buff_scene_.begin() + 10 + size_of_mat4, buff_scene_.begin() + 18 + size_of_mat4, id_in_bytes);
        std::copy(buff_scene_.begin() + 18 + size_of_mat4, buff_scene_.begin() + 72 + size_of_mat4 , byte_mat4);
        const size_t* id = reinterpret_cast<const size_t*>(id_in_bytes);
        if(buff_map_mat4_.end() == buff_map_mat4_.find(*id)) {
            buff_map_mat4_.emplace( std::make_pair(*id, Ogre::Matrix4( 
                                            mat4[0], mat4[1], mat4[2], mat4[3], 
                                            mat4[4], mat4[5], mat4[6], mat4[7],
                                            mat4[8], mat4[9], mat4[10], mat4[11],
                                            mat4[12], mat4[13], mat4[14], mat4[15])
            ));
        } else {
            buff_map_mat4_.at(*id) = std::move(Ogre::Matrix4( 
                                            mat4[0], mat4[1], mat4[2], mat4[3], 
                                            mat4[4], mat4[5], mat4[6], mat4[7],
                                            mat4[8], mat4[9], mat4[10], mat4[11],
                                            mat4[12], mat4[13], mat4[14], mat4[15]));
        }
    }
}

void MeshObject::create_scene_nodes() {
   
    Ogre::SceneManager* sceneManager = mPtrSituationMonitor_->getSceneManagerS();
    
    for(auto& sn_obj : buff_map_mat4_) {
        const Ogre::Affine3 affine3(sn_obj.second);
        Vector3 position; 
        Vector3 scale; 
        Quaternion orientation;
        
        affine3.decomposition(position, scale, orientation);
        if(sn_obj.first == 0) {
            own_position_ = position;
        }
        else {
            Vector3 curr_position = position - own_position_;
            //Ogre::Entity* unitCubeEnt = sceneManager->createEntity( "CubeObserver.mesh");
            //Ogre::SceneNode* sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
            
        }
        
    }
}