#include "Floor.h"

CFloor::CFloor()
{
    //ctor
}

CFloor::~CFloor()
{
    //dtor
}

void CFloor::RenderFloor() {
	// set things up to render the floor with the texture
	glShadeModel(GL_SMOOTH);
	glEnable(type);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();

    glTranslatef(-(float)PLANE_SIZE/2.0f, 0.0f, -(float)PLANE_SIZE/2.0f);

	//float textureScaleX = 10.0;
	//float textureScaleY = 10.0;
    glColor4f(1.0f,1.0f,1.0f,1.0f);

    //int xQuads = 40;
    //int zQuads = 40;

    int xQuads = 1.6f * (float)PLANE_SIZE;
    int zQuads = 1.6f * (float)PLANE_SIZE;

    for (int i = 0; i < xQuads; i++) {
        for (int j = 0; j < zQuads; j++) {
            glBegin(GL_QUADS);
                glTexCoord2f(1.0f, 0.0f);   // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f(i * (float)PLANE_SIZE/xQuads, 0.0f, (j+1) * (float)PLANE_SIZE/zQuads);

                glTexCoord2f(0.0f, 0.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f((i+1) * (float)PLANE_SIZE/xQuads, 0.0f, (j+1) * (float)PLANE_SIZE/zQuads);

                glTexCoord2f(0.0f, 1.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f((i+1) * (float)PLANE_SIZE/xQuads, 0.0f, j * (float)PLANE_SIZE/zQuads);

                glTexCoord2f(1.0f, 1.0f);  // coords for the texture
                glNormal3f(0.0f,1.0f,0.0f);
                glVertex3f(i * (float)PLANE_SIZE/xQuads, 0.0f, j * (float)PLANE_SIZE/zQuads);

            glEnd();
        }
    }

	glDisable(type);


	glPopMatrix();
}


/**
Initialize the texture using the library bitmap
*/
void CFloor::InitTexture(std::string textureName)
{
    printf ("\nLoading texture..\n");

    int i;                       /* Looping var */

    // Load a texture object (256x256 true color)
    bits = LoadDIBitmap(std::string((std::string)RES_PATH + textureName + ".bmp").c_str(), &info);

    if (bits == (GLubyte *)0) {
		printf ("Error loading texture!\n\n");
		return;
	}
    // Figure out the type of texture
    if (info->bmiHeader.biHeight == 1)
      type = GL_TEXTURE_1D;
    else
      type = GL_TEXTURE_2D;

    // Create and bind a texture object
    glGenTextures(1, &texture);
	glBindTexture(type, texture);

    // Create an RGBA image
    rgba = (GLubyte *)malloc(info->bmiHeader.biWidth * info->bmiHeader.biHeight * 4);

    i = info->bmiHeader.biWidth * info->bmiHeader.biHeight;
    for( rgbaptr = rgba, ptr = bits;  i > 0; i--, rgbaptr += 4, ptr += 3)
    {
            rgbaptr[0] = ptr[2];     // windows BMP = BGR
            rgbaptr[1] = ptr[1];
            rgbaptr[2] = ptr[0];
            rgbaptr[3] = (ptr[0] + ptr[1] + ptr[2]) / 3;
    }



    printf("Texture %d\n", texture);
	printf("Textures Loaded.\n\n", texture);

}

/**
Recovers the texture already initialized in initTexture(), setting it to opengl
*/
void CFloor::SetTextureToOpengl(void)
{
    // Create and bind a texture object
    glGenTextures(1, &texture);
	glBindTexture(type, texture);

	// Set texture parameters
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(type, 0, 4, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
}
