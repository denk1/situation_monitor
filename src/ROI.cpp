#include "ROI.h"
#include "ConfigMonitor.h"

using namespace std;

ROI::ROI():pathFinder_() {
   
}

void ROI::connect() {
    try {
        pathFinder_.start();
        networkClient_.connect(ConfigMonitor::getSingleton().mConfigItems["CAR_HOST"],
                               ConfigMonitor::getSingleton().mConfigItems["CAR_PORT"], 
                               10);
    } catch( boost::system::system_error err) {
        std::cerr << "connection error: \n" << err.what() << std::endl; 
    } catch( exception e) {
        cerr << "error: \n" << e.what() << std::endl;
    }
}

bool ROI::getData(std::shared_ptr<MeshObject> ptrMeshObject) {
    if(networkClient_.isOpened()) {
        const string& str_data = networkClient_.getData();
        ptrMeshObject->set_str_buff(str_data);
        ptrMeshObject->convert_to_scene_obj();
        ptrMeshObject->create_scene_nodes();
        pathFinder_.setDataAsync(str_data);
        return true;
    }
    return false;
}

Path ROI::getPath() {
    return pathFinder_.getTargetPath();
}