#ifndef CFLOOR_H
#define CFLOOR_H

#include <windows.h>
#include <gl/glut.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

#define PLANE_SIZE 24.0f
#define RES_PATH    "res\\"
#define MODELS_PATH ""

class CFloor
{
    public:
        // Constructor / Destructor
        CFloor();
        virtual ~CFloor();

        // Methods
        void InitFloor();
        void RenderFloor();

        void InitTexture(std::string textureName);
        void SetTextureToOpengl(void);
    protected:
    private:
        int ResolutionX;
        int ResolutionY;

        GLubyte     rgb[3];
        GLubyte	    *rgba;           /* RGBA pixel buffer */
        GLubyte	    *rgbaptr;        /* Pointer into RGBA buffer */
        GLubyte     *ptr;            /* Pointer into bit buffer */
        GLubyte	    *bits;           /* Bitmap RGB pixels */

        BITMAPINFO	*info;           /* Bitmap information */
        GLuint      texture;         /* Texture object */

        GLenum      type;            /* Texture type */

};

#endif // CFLOOR_H
