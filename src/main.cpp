/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  test app.
 *
 *        Version:  1.0
 *        Created:  01/14/2009 03:40:30 PM
 *
 *         Author:  Denk1 (nikki)
 *
 * =====================================================================================
 */

#include "ConfigMonitor.h"
#include "SituationMonitor.h"
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main() function. Need say more?
 * =====================================================================================
 */

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    ConfigMonitor configMonitor;
    SituationMonitor app;
    SituationMonitor::getSingleton().initApp();
    SituationMonitor::getSingleton().getRoot()->startRendering();
    SituationMonitor::getSingleton().closeApp();

    return 0;
}
