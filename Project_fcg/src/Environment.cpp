#include "Environment.h"

CEnvironment::CEnvironment()
{
    mapGame = (mapStruct *)malloc(sizeof(mapStruct));
    if (mapGame == NULL)
    {
        // error
    }

    Floor = (CFloor *)malloc(sizeof(CFloor));
    if (Floor == NULL)
    {
        // error
    }
}

CEnvironment::~CEnvironment()
{
    //dtor
}

void CEnvironment::ChangeCam(){
    SelectedCam++;
    SelectedCam %= CAM_COUNT;
}
void CEnvironment::UpdateCam(){

    switch(SelectedCam){
        case CAMERA_1:{         // camera Player 3a
                gluLookAt(Player->GetPosX() - OBJ_SPEED * 15.0 * sin(Player->GetFace()*PI/180),3.0,Player->GetPosZ()- OBJ_SPEED * 15.0 * cos(Player->GetFace()*PI/180),
                          Player->GetPosX(),1.5,Player->GetPosZ(),
                          0.0,1.0,0.0);
            break;
        }
        case CAMERA_2:{     // camera Player 1a
            gluLookAt(Player->GetPosX() - OBJ_SPEED * 1.0 * sin(Player->GetFace()*PI/180),2.5,Player->GetPosZ()- OBJ_SPEED * 1.0 * cos(Player->GetFace()*PI/180),
                          Player->GetPosX(),2.40,Player->GetPosZ(),
                          0.0,1.0,0.0);


            break;
        }
        case CAMERA_3:{
            gluLookAt(0.0,30.0,1.0,
                      0.0,0.0,0.0,
                      0.0,1.0,0.0);
            break;
        }
        case CAMERA_4:{
            gluLookAt(0.0,2.0,3.0,
                      Player->GetPosX(),Player->GetPosY(),Player->GetPosZ(),
                      0.0,1.0,0.0);
            break;
        }
        case CAMERA_5:{
            gluLookAt(0.0,10.0,3.0,
                      Player->GetPosX(),Player->GetPosY(),Player->GetPosZ(),
                      0.0,1.0,0.0);
            break;
        }
    }



	// atualiza a posição do listener e da origen do som, são as mesmas da camera, já que os passos vem de onde o personagem está
//	listenerPos[0] = posX;
//	listenerPos[1] = posY;
//	listenerPos[2] = posZ;
//	source0Pos[0] = posX;
//	source0Pos[1] = posY;
//	source0Pos[2] = posZ;

    //updateLight();

}

void CEnvironment::InitScene(std::string mapName){

    printf ("\nLoading objects..\n");

    SelectedCam = CAMERA_1;

    bits = LoadDIBitmap(std::string((std::string)RES_PATH + "maps\\" + mapName + ".bmp").c_str(), &info);

    if (bits == (GLubyte *)0) {
		printf ("Error loading objects!\n\n");
		return;
	}

    ptr = bits;

    for(int x = 0; x<info->bmiHeader.biWidth ;x++)
    {
        for(int y = 0; y<info->bmiHeader.biHeight ;y++, ptr += 3)
        {
            rgb[0] = ptr[2];
            rgb[1] = ptr[1];
            rgb[2] = ptr[0];

            if(rgb[MAPGAME_OBJS] == OBJ_PENGO
               || rgb[MAPGAME_OBJS] == OBJ_MONSTER1
               || rgb[MAPGAME_OBJS] == OBJ_CUBE1
               || rgb[MAPGAME_OBJS] == OBJ_SOLID1){

                codes++;
                mapGame->matrixCodes[x][y] = codes;
                mapGame->matrixTypes[x][y] = rgb[MAPGAME_OBJS];

                mapObjects[x][y].SetMapGame(mapGame);
                mapObjects[x][y].SetTypeObject(rgb[MAPGAME_OBJS]);
                mapObjects[x][y].SetPlaneSize(PLANE_SIZE);
                mapObjects[x][y].SetX(x);
                mapObjects[x][y].SetY(y);

                switch(rgb[MAPGAME_OBJS]){
                    case OBJ_PENGO:{

                        mapObjects[x][y].SetObj(codes
                                               //,std::string((std::string)MODELS_PATH + "penguin.obj").c_str()
                                               ,std::string((std::string)MODELS_PATH + "ball_monster.obj").c_str()
                                               ,SMOOTH_MATERIAL_TEXTURE
                                               ,true);

                        Player = &mapObjects[x][y];
                        break;
                    }
                    case OBJ_MONSTER1:{

                        mapObjects[x][y].SetObj(codes
                                               ,std::string((std::string)MODELS_PATH + "ball_monster.obj").c_str()
                                               ,SMOOTH_MATERIAL_TEXTURE
                                               ,true);
                        break;
                    }
                    case OBJ_CUBE1:{

                        mapObjects[x][y].SetObj(codes
                                               ,std::string((std::string)MODELS_PATH + "cube.obj").c_str()
                                               ,SMOOTH_MATERIAL_TEXTURE
                                               ,true);
                        break;
                    }
                    case OBJ_SOLID1:{

                        mapObjects[x][y].SetObj(codes
                                               ,std::string((std::string)MODELS_PATH + "solid_cube.obj").c_str()
                                               ,SMOOTH_MATERIAL_TEXTURE
                                               ,true);
                        break;
                    }
                }
            }
            else{
                mapGame->matrixCodes[x][y] = OBJ_ITEM;
                mapGame->matrixTypes[x][y] = OBJ_FREE;

                mapObjects[x][y].Free();
            }

            ptrMapObjs[x][y] = &mapObjects[x][y];

            for (int i=0;i<XMAP;i++){
               for(int j = 0; j<YMAP; j++)
               {
                   mapObjects[i][j].SetCompleteMap(ptrMapObjs[x][y],x,y);
               }
            }
        }
    }



    printf("\nMAP Loaded:\n%s\n%d objects\n", ((std::string)mapString(mapGame)).c_str(),codes);

}

void CEnvironment::RenderScene() {

	glClearColor(backgrundColor[0],backgrundColor[1],backgrundColor[2],backgrundColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // limpar o depth buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	UpdateCam();

    for (int i=0;i<XMAP;i++){
       for(int j = 0; j<YMAP; j++)
       {
            glPushMatrix();

            if(mapObjects[i][j].IsObj()){
                mapObjects[i][j].Draw();

            }
            glPopMatrix();
       }
    }


    // sets the bmp file already loaded to the OpenGL parameters
    Floor->SetTextureToOpengl();
	Floor->RenderFloor();

}



void CEnvironment::UpdateState(){

    for (int i=0;i<XMAP;i++){
       for(int j = 0; j<YMAP; j++)
       {
            glPushMatrix();

            if(mapObjects[i][j].IsObj()){
                mapObjects[i][j].Animate();
                if(mapObjects[i][j].GetTypeObject()==OBJ_MONSTER1){
                    mapObjects[i][j].MoveRand();
                }
            }
            glPopMatrix();
       }
    }

    if(alterViewPressed){
        ChangeCam();
        alterViewPressed = false;
    }

    Player->MoveUp(upPressed);
    Player->MoveDown(downPressed);

    Player->MoveLeft(leftPressed);
    Player->MoveRight(rightPressed);

}
void CEnvironment::SetDown(bool pressed){

    downPressed = pressed;

}
void CEnvironment::SetUp(bool pressed){

    upPressed = pressed;

}
void CEnvironment::SetLeft(bool pressed){

    leftPressed = pressed;

}
void CEnvironment::SetRight(bool pressed){

    rightPressed = pressed;

}
void CEnvironment::SetView(bool pressed){

    alterViewPressed = pressed;

}
