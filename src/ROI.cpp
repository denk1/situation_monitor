#include "ROI.h"

using namespace std;

ROI::ROI():pathFinder_() {
   
}

void ROI::connect() {
    try {
        pathFinder_.start();
        networkClient_.connect("localhost", "15555", 10);
    } catch( boost::system::system_error err) {
        std::cerr << "connection error: " << err.what() << std::endl; 
    }
}

bool ROI::getData(std::shared_ptr<MeshObject> ptrMeshObject) {
    if(networkClient_.isOpened()) {
        const string& str_data = networkClient_.getData();
        ptrMeshObject->set_str_buff(str_data);
        ptrMeshObject->convert_to_scene_obj();
        ptrMeshObject->create_scene_nodes();
        pathFinder_.setData(str_data);
        pathFinder_.convertBuff();
        return true;
    }
    return false;
}
