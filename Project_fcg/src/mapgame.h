#ifndef _mapgame_H
#define _mapgame_H

#include <string.h>
#include <stdlib.h>

#include <windows.h>
#include <string>

#define XMAP 24
#define YMAP 24

#define MAPGAME_OBJS 0
#define MAPGAME_FLOOR 1
#define MAPGAME_FREE 2

// Camera
#define CAMERA_1 0
#define CAMERA_2 1
#define CAMERA_3 2
#define CAMERA_4 3
#define CAMERA_5 4

#define CAM_COUNT 5

// texture def
#define TEXTURE_1 0
#define TEXTURE_2 25
#define TEXTURE_3 50
#define TEXTURE_4 75
#define TEXTURE_5 100
#define TEXTURE_6 125
#define TEXTURE_7 150
#define TEXTURE_8 175

// object def
#define OBJ_FREE 0
#define OBJ_PENGO 25
#define OBJ_MONSTER1 50
#define OBJ_CUBE1 75
#define OBJ_SOLID1 100
#define OBJ_5 125
#define OBJ_6 150
#define OBJ_7 175
#define OBJ_8 200
#define OBJ_9 225

// codes definition
#define OBJ_ITEM 0

typedef struct{

    int matrixCodes[XMAP][YMAP];
    int matrixTypes[XMAP][YMAP];
    int objCenter[XMAP][YMAP][2];

}mapStruct;
std::string  mapString(mapStruct* mapGame);

#endif





