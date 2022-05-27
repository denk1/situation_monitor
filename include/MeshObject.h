#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include<OGRE/Ogre.h>


class SituationMonitor;




class MeshObject {
public:
    typedef std::map<size_t, Ogre::Matrix4> map_mat4;
    struct cmp {
        bool operator()(size_t i, const map_mat4::value_type& p) const
        {
            return i < p.first;
        }

        bool operator()(const map_mat4::value_type& p, size_t i) const
        {
            return p.first < i;
        }
    };
    MeshObject(SituationMonitor* ptrSituationMonitor);
    
    void set_str_buff(const std::string& str_buff);
    void convert_to_scene_obj();
    void create_scene_nodes();
    
private:
    void copy_to_prev(map_mat4& m, std::set<int>& s);
    void del_prev_nodes(const std::vector<size_t>& vDiff);
    std::string buff_scene_;
    map_mat4 buff_map_mat4_;
    SituationMonitor* mPtrSituationMonitor_;
    Ogre::Vector3 own_position_;
    std::set<size_t> sPrevEntities_;
    Ogre::SceneManager* mSceneManager_;
    Ogre::SceneNode* mRootSceneNode_;
};
#endif