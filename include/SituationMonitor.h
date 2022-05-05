#ifndef SITUATION_MONITOR_H
#define SITUATION_MONITOR_H
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

using namespace Ogre;

/*
 * =====================================================================================
 *        Class:  BtOgreTestApplication
 *  Description:  Derives from ExampleApplication and overrides stuff.
 * =====================================================================================
 */

class SituationMonitor: public Ogre::Singleton<SituationMonitor>, public OgreBites::ApplicationContext, public OgreBites::InputListener
{

	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	Ogre::SceneNode *mNinjaNode;
	Ogre::Entity *mNinjaEntity;
	Ogre::Entity *mGroundEntity;

	OgreBites::CameraMan *mCamMan;

    ROI roi;
	MeshObject meshObject_;

	bool mDebugOn;
    

    public:

	SituationMonitor() : OgreBites::ApplicationContext("Situation Monitor")
	{
		mDebugOn = true;
	}
	
	void shutdown()
	{
		OgreBites::ApplicationContext::shutdown();
	}

	void setup(void)
	{
		roi.connect();
	    OgreBites::ApplicationContext::setup();
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

	    Ogre::SceneNode* camnode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	    camnode->attachObject(mCamera);

	    // and tell it to render into the main window
	    getRenderWindow()->addViewport(mCamera);

	    mCamMan = new OgreBites::CameraMan(camnode);
	    mCamMan->setStyle(OgreBites::CS_ORBIT);
	    mCamMan->setYawPitchDist(Ogre::Degree(0), Ogre::Degree(0), 50);
	    addInputListener(mCamMan);

	    //Some normal stuff.
	    mSceneMgr->setAmbientLight(ColourValue(0.7,0.7,0.7));

	    mCamera->setNearClipDistance(0.05);
	    LogManager::getSingleton().setLogDetail(LL_BOREME);

	    //----------------------------------------------------------
	    // Debug drawing!
	    //----------------------------------------------------------

		Plane plane;
    	plane.normal = Vector3::UNIT_Y;
    	plane.d = 100;

		MeshManager::getSingleton().createPlane("Myplane",
                                            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                            100, 100, 40, 40, true, 1, 60, 60, Vector3::UNIT_Z);
    	Entity *pPlaneEnt = mSceneMgr->createEntity("plane", "Myplane");
		pPlaneEnt->setMaterialName("Examples/Rockwall");
    	pPlaneEnt->setCastShadows(false);
    	// Create Ogre stuff.
    	mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 99, 0))->attachObject(pPlaneEnt);


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
		
		roi.getData(&meshObject_);

        return true;
    }

	Ogre::SceneManager *getSceneManagerS()
	{
    	return getSingleton().mSceneMgr;
	}

	

};

#endif