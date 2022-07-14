cmake_minimum_required(VERSION 2.8.6)

project(situation_monitor)

set(CMAKE_CXX_STANDARD 20)
find_package(OGRE REQUIRED CONFIG)

include_directories(
    ${PROJECT_SOURCE_DIR}/../btogre/include
    ${PROJECT_SOURCE_DIR}/include/
)


add_executable(${PROJECT_NAME}
                 src/SituationMonitor.cpp
                 src/ROI.cpp
                 src/NetworkClient.cpp
                 src/NetworkClientITS.cpp
                 src/NetworkClientBMD.cpp
                 src/PathFinder.cpp
                 src/MeshObject.cpp
                 src/ConfigMonitor.cpp
                 src/PathDrawer.cpp
                 src/main.cpp
                 )
target_link_libraries(situation_monitor OgreMain OgreBites BtOgre tinyxml)
configure_file(data/resources.cfg.in ${CMAKE_BINARY_DIR}/resources.cfg @ONLY)
configure_file(data/network.cfg.in ${CMAKE_BINARY_DIR}/network.cfg @ONLY)
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${CMAKE_BINARY_DIR})