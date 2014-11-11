#include "mapgame.h"

std::string  mapString(mapStruct* mapGame){

    std::string ret = "";

    for(int i=0;i<XMAP;i++){
        for(int j=0;j<YMAP;j++){
            ret.append(" ");
            switch(mapGame->matrixTypes[i][j]){

                case OBJ_PENGO:{
                    ret.append("P");
                    break;
                }
                case OBJ_MONSTER1:{
                    ret.append("M");
                    break;
                }
                case OBJ_CUBE1:{
                    ret.append("C");
                    break;
                }
                case OBJ_SOLID1:{
                    ret.append("S");
                    break;
                }
                default:
                    ret.append("-");

            }

            ret.append(" ");
        }
        ret += "\n";
    }

    return ret;
}
