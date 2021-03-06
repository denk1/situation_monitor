#ifndef SITUATION_MONITOR_H
#define SITUATION_MONITOR_H
#include <memory>
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMovableObject.h"
#include "OgreMeshManager.h"
#include "ROI.h"
#include "MeshObject.h"
#include "ConfigMonitor.h"
#include "PathDrawer.h"


using namespace Ogre;

/*
 * =====================================================================================
 *        Class:  BtOgreTestApplication
 *  Description:  Derives from ExampleApplication and overrides stuff.
 * =====================================================================================
 */

class ROI;
class MeshObject;

class SituationMonitor: public Ogre::Singleton<SituationMonitor>,
						public OgreBites::ApplicationContext,
						public OgreBites::InputListener, 
						std::enable_shared_from_this<SituationMonitor>
{

	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	Ogre::SceneNode *mNinjaNode;
	Ogre::Entity *mNinjaEntity;
	Ogre::Entity *mGroundEntity;

	OgreBites::CameraMan *mCamMan;
	const float x_roi = 400, y_roi = 200; 
    ROI roi;
	PathDrawer pathDrawer;
	std::shared_ptr<MeshObject> ptrMeshObject_;
	Ogre::String mNetworkCfg_ = "network.cfg";
	Ogre::HardwarePixelBufferSharedPtr mPixelBuffer;
	bool mDebugOn;
    

    public:

	SituationMonitor() : OgreBites::ApplicationContext("Situation Monitor"), roi(x_roi, y_roi), pathDrawer(x_roi, y_roi)
	{
		mDebugOn = true;
	}
	
	void shutdown()
	{
		OgreBites::ApplicationContext::shutdown();
	}

	void setup(void)
	{
		OgreBites::ApplicationContext::setup();
		// config initialization must be before everything
		ConfigMonitor::getSingletonPtr()->setConfig(mNetworkCfg_);
		roi.connect();
	    
		
	    addInputListener(this);

	    mSceneMgr = getRoot()->createSceneManager();

	    // register our scene with the RTSS
	    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	    shadergen->addSceneManager(mSceneMgr);

	    // without light we would just get a black screen
        Ogre::Light* light = mSceneMgr->createLight("MainLight");
        Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        lightNode->setPosition(0, 100, 15);
        lightNode->attachObject(light);

	    // create the camera
	    mCamera = mSceneMgr->createCamera("myCam");
	    mCamera->setAutoAspectRatio(true);

	    Ogre::SceneNode* camnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CameraNode");
	    camnode->attachObject(mCamera);
		camnode->setPosition(Ogre::Vector3(0, 1, 0.0f));

	    // and tell it to render into the main window
	    getRenderWindow()->addViewport(mCamera);

	    //mCamMan = new OgreBites::CameraMan(camnode);
	    //mCamMan->setStyle(OgreBites::CS_ORBIT);
	    //mCamMan->setYawPitchDist(Ogre::Degree(0), Ogre::Degree(0), 50);
	    //addInputListener(mCamMan);

	    //Some normal stuff.
	    mSceneMgr->setAmbientLight(ColourValue(0.7,0.7,0.7));

	    mCamera->setNearClipDistance(0.05);
	    LogManager::getSingleton().setLogDetail(LL_BOREME);

	    //----------------------------------------------------------
	    // Debug drawing!
	    //----------------------------------------------------------

		Plane plane;
    	plane.normal = Vector3::UNIT_Y;
    	plane.d = 0;

		MeshManager::getSingleton().createPlane("Myplane",
                                            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                            400, 200, 40, 40, true, 1, 1, 1, Vector3::UNIT_Z);
    	Entity *pPlaneEnt = mSceneMgr->createEntity("plane", "Myplane");
		
    	pPlaneEnt->setCastShadows(false);
		
    	// Create Ogre stuff.
		//mSceneMgr->getSceneNode("CameraNode")->createChildSceneNode(Vector3(0, 99, 0))->attachObject(pPlaneEnt);
    	mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 0, 0))->attachObject(pPlaneEnt);
		ptrMeshObject_ = std::make_shared<MeshObject>(this);
		Ogre::String texName = "dynamic_texture";
		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(texName,
																			 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
																			 Ogre::TEX_TYPE_2D,
																			 400, 200, 10,
																			 0,
																			 Ogre::PF_R8G8B8A8,
																			 Ogre::TU_DYNAMIC
																			);
		texture->createShaderAccessPoint(0);
		
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(texName,
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		
		pPlaneEnt->setMaterial(material);
		//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(texture);
		Ogre::TextureUnitState* textureUnitState = material->getTechnique(0)->getPass(0)->createTextureUnitState(texName);
		//textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		texture = Ogre::TextureManager::getSingletonPtr()->getByName(texName);
		mPixelBuffer = texture->getBuffer(0,0);
		

	}

	bool keyPressed(const OgreBites::KeyboardEvent& evt)
	{
		using namespace OgreBites;

	    if (evt.keysym.sym == SDLK_ESCAPE)
	    {
	        getRoot()->queueEndRendering();
	    }
	    else if(evt.keysym.sym == SDLK_F3)
		{
	        mDebugOn = !mDebugOn;

		}
	    return true;
	}

    bool frameStarted(const FrameEvent &evt)
    {
        OgreBites::ApplicationContext::frameStarted(evt);

		mPixelBuffer->lock(HardwareBuffer::HBL_DISCARD);
		const Ogre::PixelBox& pb = mPixelBuffer->getCurrentLock();

		{
            uint* data = reinterpret_cast<uint*>(pb.data);
            size_t height = pb.getHeight();
            size_t width = pb.getWidth();

            // 0xAABBGGRR -> fill the buffer with yellow pixels
            std::fill(data, data + width * height, 0x00FFFF15);

			roi.getData(ptrMeshObject_);
			const Path path = roi.getPath();
			pathDrawer.setPathData(data, path);
            // Unlock the buffer again (frees it for use by the GPU)
            mPixelBuffer->unlock();
        }
		
		

        return true;
    }

	Ogre::SceneManager *getSceneManagerS()
	{
    	return getSingleton().mSceneMgr;
	}
};

#endif