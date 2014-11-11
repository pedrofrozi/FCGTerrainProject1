#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <windows.h>
#include <gl/glut.h>

#include "mapgame.h"
#include "gameobj.h"
#include "3DObject.h"
#include "Floor.h"
//bitmap class to load bitmaps for textures
#include "bitmap.h"

#define SMOOTH 0
#define SMOOTH_MATERIAL 1
#define SMOOTH_MATERIAL_TEXTURE 2

class CEnvironment
{
    public:

        int       SelectedCam;

        CGameObj  *Player;
        CGameObj  *ptrMapObjs[XMAP][YMAP];
        CGameObj  mapObjects[XMAP][YMAP];

        CFloor    *Floor;

        mapStruct* mapGame;

        // Constructor / Destructor
        CEnvironment();
        virtual ~CEnvironment();

        // Methods
        void InitScene(std::string mapName);
        void ChangeCam();
        void UpdateCam();
        void UpdateState();
        void RenderScene();

        // Keys
        void SetUp(bool pressed);
        void SetDown(bool pressed);
        void SetLeft(bool pressed);
        void SetRight(bool pressed);
        void SetView(bool pressed);

    protected:

    private:

        int codes = 0;

        GLubyte     rgb[3];
        GLubyte     *ptr;            /* Pointer into bit buffer */
        GLubyte	    *bits;           /* Bitmap RGB pixels */

        BITMAPINFO	*info;           /* Bitmap information */

        // keys press
        bool upPressed;
        bool downPressed;
        bool leftPressed;
        bool rightPressed;
        bool alterViewPressed;

        float backgrundColor[4] = {0.0f,0.0f,0.0f,1.0f};



};

#endif // ENVIRONMENT_H
