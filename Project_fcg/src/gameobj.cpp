
#include "gameobj.h"
//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
CGameObj::CGameObj() {
    xMap = 0;
    yMap = 0;

    posX = 0;
    posY = 1;
    posZ = 0;
    isObj = false;

}


//-----------------------------------------------------------------------------
// destructor
//-----------------------------------------------------------------------------
CGameObj::~CGameObj() {
    isObj = false;
}
void CGameObj::SetObj(int code, const char *filename,int opTexture, bool isSolid){

    isObj  = true;
    option = opTexture;
    codeName = code;

    posX = 0;
    posY = 1;
    posZ = 0;

    myBlocks = 0;
    state = STATE_LIVE;
    face = 0;

    t = 0;

    movingLeft  = false;
    movingRight = false;
    movingUp    = false;
    movingDown  = false;


    obj.Init();
	obj.Load(filename);

	UpdatePosition();

}

void CGameObj::SetMapGame(mapStruct* mapG){
    mapGame = mapG;
}
void CGameObj::SetCompleteMap(CGameObj *mapO,int i,int j){
    completeMap[i][j] = mapO;
}
void CGameObj::UpdateMapGame(){

    mapGame->matrixCodes[prevXMap][prevYMap] = 0;
    mapGame->matrixTypes[prevXMap][prevYMap] = OBJ_FREE;
    mapGame->objCenter[prevXMap][prevXMap][0] = 0;
    mapGame->objCenter[prevXMap][prevXMap][1] = 0;

    mapGame->matrixCodes[xMap][yMap] = codeName;
    mapGame->matrixTypes[xMap][yMap] = typeObj;

    mapGame->objCenter[xMap][yMap][0] = posX;
    mapGame->objCenter[xMap][yMap][1] = posZ;
}
void CGameObj::Free(){
    isObj  = false;

    codeName = OBJ_ITEM;

    posX = 0;
    posY = 1;
    posZ = 0;

    myBlocks = 0;
    state = STATE_LIVE;
    face = 0;

    t = 0;

    movingLeft  = false;
    movingRight = false;
    movingUp    = false;
    movingDown  = false;
}
bool CGameObj::IsCollision(int newXMap, int newYMap){

    float otherX;
    float otherZ;
    float dist;

    if(newXMap < 0 || newYMap < 0){
       return true;
    }
    if(newXMap > XMAP - 1 || newYMap > XMAP - 1){
       return true;
    }

    for (int i=0;i<XMAP;i++){
       for(int j = 0; j<YMAP; j++)
       {
           if( completeMap[i][j]->IsObj() &&
               completeMap[i][j]->GetCodeName()   != OBJ_ITEM &&
               completeMap[i][j]->GetCodeName()   != codeName &&
               completeMap[i][j]->GetTypeObject() != OBJ_FREE ){

               otherX = completeMap[i][j]->GetPosX();
               otherZ = completeMap[i][j]->GetPosZ();
               dist   = sqrt(pow(otherX - posX,2.0) + pow(otherZ - posZ,2.0));

               if (dist<OBJ_RAID){
                    //printf("COL POSMAP \n\t- X: %d\n\t- Y: %d\n\n",i,j);
                    //printf("col POS \n\t- X: %f\n\t- Y: %f\n\n",otherX,otherZ);
                    //system("pause");
                    return true;
               }
            }
       }
    }
    /*
    // Collision with limits
    if(newXMap < 0 || newYMap < 0){
       return true;
    }
    if(newXMap > XMAP - 1 || newYMap > XMAP - 1){
       return true;
    }
    // Collision with solidObjs
    if(mapGame->matrixCodes[newXMap][newYMap] != OBJ_ITEM &&
       mapGame->matrixCodes[newXMap][newYMap] != codeName){
       return true;
    }
    */
    return false;
}

int CGameObj::GetCodeName(){
    return codeName;
}
void CGameObj::SetX(int X){
    xMap = X;
}
void CGameObj::SetY(int Y){
    yMap = Y;
}

bool CGameObj::IsObj(){

    return isObj;

}
float CGameObj::GetFace(){

    return face;

}

void CGameObj::Move(int x,int y){

    xMap = x;
    yMap = y;

}

void CGameObj::Draw(){

    float speedX = OBJ_SPEED * sin(face*PI/180);
    float speedZ = OBJ_SPEED * cos(face*PI/180);

    float oldX = posX;
    float oldZ = posZ;

    if(movingUp)
    {
        posX += speedX;
        posZ += speedZ;
    }
    if(movingDown)
    {
        posX -= speedX;
        posZ -= speedZ;
    }

    float xAprox = posX + ((float)planeSize/2.0);
    float zAprox = posZ + ((float)planeSize/2.0);

    int newXMap = floor(xAprox);
    int newYMap = floor(zAprox);

    if(IsCollision(newXMap,newYMap)){
        posX = oldX;
        posZ = oldZ;
    }

    if(face >= 360.0)
        face = 0.0;

    if(movingLeft){
        face += ROTATE_SPEED;
    }
    if(movingRight){
        face -= ROTATE_SPEED;
    }

    glTranslatef(posX, posY, posZ);
    glRotatef(face, 0.0,1.0,0.0);
    obj.Draw(option);

    UpdateMap();
    UpdateMapGame();

//    if(typeObj == OBJ_PENGO && t%20==0){
//        printf("PENGO POSMAP \n\t- X: %d\n\t- Y: %d\n\n",xMap,yMap);
//        printf("PENGO POS \n\t- X: %f\n\t- Y: %f\n\n",posX,posY);
//    }

}
void CGameObj::update_t(){
    t++;
    t %= 100;
}

void CGameObj::Animate(){

    update_t();

    switch(typeObj){
        case OBJ_PENGO:{

            GLfloat angle = t;
            break;
        }
        case OBJ_MONSTER1:{

            int duration = 20;

            if(t%duration==0)
                posY = 1;
            else if(t%duration < (duration/2))
                posY += 0.1;
            else if(t%duration < duration)
                posY -= 0.1;

            if(posY<1)
                posY = 1;

            break;
        }
        case OBJ_CUBE1:{

            break;
        }
        case OBJ_SOLID1:{

            break;
        }
        case OBJ_ITEM:{
            movingLeft = true;
            break;
        }
    }

}


GLfloat CGameObj::GetPosX(){
    return posX;
}
GLfloat CGameObj::GetPosY(){
    return posY;
}
GLfloat CGameObj::GetPosZ(){
    return posZ;
}

void CGameObj::SetPlaneSize(int w){
    planeSize = w;
}

void CGameObj::UpdatePosition(){

    posX = (xMap - (planeSize/2));
    posZ = (yMap - (planeSize/2));

}

void CGameObj::UpdateMap(){

     prevXMap = xMap;
     prevYMap = yMap;

     xMap = floor(posX + (planeSize/2));
     yMap = floor(posZ + (planeSize/2));

}

void CGameObj::SetTypeObject(int tObj){

    typeObj = tObj;

}
int CGameObj::GetTypeObject(){

    return typeObj;

}

bool CGameObj::IsMoving(){

    return isMoving;

}

void CGameObj::MoveUp(bool enabled){

    movingUp = enabled;

}
void CGameObj::MoveDown(bool enabled){

    movingDown = enabled;

}
void CGameObj::MoveLeft(bool enabled){

    movingLeft = enabled;

}
void CGameObj::MoveRight(bool enabled){

    movingRight = enabled;

}

void CGameObj::MoveRand(){

    if(t%50==0){
        movingDown  = (std::rand() % 4) == 0;
        movingUp    = (std::rand() % 2) == 0;
        movingLeft  = (std::rand() % 4) == 0;
        movingRight = (std::rand() % 4) == 0;
    }

}

void CGameObj::UpdateState(){

//	if (upPressed || downPressed) {
//
//		if (running) {
//			speedX = 0.05 * sin(roty*PI/180) * 2;
//			speedZ = -0.05 * cos(roty*PI/180) * 2;
//		} else {
//			speedX = 0.05 * sin(roty*PI/180);
//			speedZ = -0.05 * cos(roty*PI/180);
//		}
//
//		// efeito de "sobe e desce" ao andar
//		headPosAux += 8.5f;
//		if (headPosAux > 180.0f) {
//			headPosAux = 0.0f;
//		}
//
//        if (upPressed) {
//            posX += speedX;
//            posZ += speedZ;
//        } else {
//            posX -= speedX;
//            posZ -= speedZ;
//        }
//
//	} else {
//		// parou de andar, para com o efeito de "sobe e desce"
//		headPosAux = fmod(headPosAux, 90) - 1 * headPosAux / 90;
//		headPosAux -= 4.0f;
//		if (headPosAux < 0.0f) {
//			headPosAux = 0.0f;
//		}
//	}
//
//	posY += speedY;
//	if (posY < heightLimit) {
//		posY = heightLimit;
//		speedY = 0.0f;
//		jumping = false;
//	} else {
//		speedY -= gravity;
//	}
//
//	if (crouched) {
//		posYOffset -= 0.01;
//		if (posYOffset < 0.1) {
//			posYOffset = 0.1;
//		}
//	} else {
//		posYOffset += 0.01;
//		if (posYOffset > 0.2) {
//			posYOffset = 0.2;
//		}
//	}

}
