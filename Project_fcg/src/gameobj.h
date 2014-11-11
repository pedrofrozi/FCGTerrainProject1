#ifndef _GameObj_H
#define _GameObj_H

#include "mapgame.h"
#include "3DObject.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <cstdlib>

#define ERR_MOVE     0.3
#define ROTATE_SPEED 5.0
#define OBJ_SPEED    0.3
#define OBJ_HSPEED   0.6
#define PI           3.14159265

#define STATE_LIVE   0
#define STATE_DEAD   1
#define STATE_HSPEED 2

#define OBJ_RAID 1.5

class CGameObj {

protected:

    int state;
    int codeName;

    /// Map
    mapStruct* mapGame;
    CGameObj  *completeMap[XMAP][YMAP];

    int myBlocks;

    /// model
    C3DObject obj;

    /// position
    int prevXMap;
    int prevYMap;

    int xMap;
    int yMap;

    // timer
    int t;

    float face;

    GLfloat posX;
    GLfloat posY;
    GLfloat posZ;

    bool isObj;
    bool isMoving;
    bool isSolid;

    bool movingLeft;
    bool movingRight;
    bool movingUp;
    bool movingDown;

    int  option;
    int  planeSize;

    int typeObj;

    void update_t();
//-----------------------------------------------------------------------------
public:

    /// constructor
    CGameObj();

    /// destructor
    ~CGameObj();

    void SetMapGame(mapStruct* mapG);
    void SetCompleteMap(CGameObj *mapO,int i,int j);
    void UpdateMapGame();
    bool IsCollision(int newXMap, int newYMap);

    void Free();

    void SetX(int X);
    void SetY(int Y);

    float GetFace();

    GLfloat GetPosX();
    GLfloat GetPosY();
    GLfloat GetPosZ();

    void SetPlaneSize(int w);
    void SetTypeObject(int tObj);

    int GetTypeObject();
    int GetCodeName();

    bool IsMoving();
    bool IsObj();

    void SetObj(int code, const char *pszFilename, int option, bool isSolid);

    void Move(int x,int y);

    void Animate();

    /// draw the object
    void Draw();

    void UpdateState();

    void UpdatePosition();
    void UpdateMap();

    void MoveUp(bool enabled);
    void MoveDown(bool enabled);
    void MoveLeft(bool enabled);
    void MoveRight(bool enabled);

    void MoveRand();

};

#endif
