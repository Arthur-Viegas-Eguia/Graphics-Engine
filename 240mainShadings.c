


/* On macOS, compile with...
    clang 220mainAbstracted.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
*/

/**
 * These are the dependencies of the program
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "080vector.c"
#include "230matrix.c"
#include "150texture.c"
#include "220shading.c"
#include "220triangle.c"
#include "220mesh.c"
#include "190mesh2D.c"



/**
 * These are the constants of the meshes being drawn by the
 * program.
*/
#define ATTRX 0
#define ATTRY 1
#define ATTRS 2
#define ATTRT 3
#define VARYX 0
#define VARYY 1
#define VARYS 2
#define VARYT 3
#define VARYR 4
#define VARYG 5
#define VARYB 6
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define UNIFMODELING 3
#define TEXR 0
#define TEXG 1
#define TEXB 2

/**
 * This determines the Vary vector of each vertex of the picture.
 * Determines their position in the screen
*/
void shadeVertex(
        int unifDim, const double unif[], int attrDim, const double attr[], 
        int varyDim, double vary[]) {
    double attrHomog[3] = {attr[ATTRX], attr[ATTRY], 1};
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomog, vary);
    vary[VARYS] = attr[ATTRS];
    vary[VARYT] = attr[ATTRT];
}

/**
 * This determines the Vary vector of each vertex of the background. Determines their position in the screen
*/
void shadeVertex2(
        int unifDim, const double unif[], int attrDim, const double attr[], 
        int varyDim, double vary[]) {
    double attrHomog[3] = {attr[ATTRX], attr[ATTRY], 1}, color;
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomog, vary);
    vary[VARYS] = attr[ATTRS] * unif[UNIFB];
    color = attr[ATTRT] + (unif[UNIFR] * unif[UNIFG]);
    if(color > 1){
        color = color / (2 * sqrt(3));
    }
    vary[VARYT] = color;
}

/**
 * This determines the Vary vector of each vertex of the tennis ball.
 * Inverts ATTRS to ATTRT
 * Determines their position in the screen
*/
void shadeVertex3(
        int unifDim, const double unif[], int attrDim, const double attr[], 
        int varyDim, double vary[]) {
    double attrHomog[3] = {attr[ATTRX], attr[ATTRY], 1};
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomog, vary);
    vary[VARYS] = attr[ATTRT];
    vary[VARYT] = attr[ATTRS];
}

/**
 * This determines the color of each pixel of the tennis ball, and modulate
 * it with the uniform vector
*/
void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[0]->texelDim];
    texSample(tex[0], vary[VARYS], vary[VARYT], sample);
    vecModulate(3, sample, &unif[UNIFR], rgb);
}

/**
 * This determines the color of each pixel of the background image
*/
void shadeFragment2(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[0]->texelDim];
    texSample(tex[1], vary[VARYS], vary[VARYT], rgb);
}

/**
 * This gets the texture of two images, meshes them together and modulates them
 * with the uniform vector and based on that determines the color of the pixel
*/
void shadeFragment3(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[3]->texelDim], sample2[tex[2]->texelDim];
    texSample(tex[3], vary[VARYS], vary[VARYT], sample);
    texSample(tex[2], vary[VARYS], vary[VARYT], sample2);
    vecModulate(3, sample, sample2, sample);
    vecModulate(3, sample, &unif[UNIFR], rgb);
}


/**
 * These are the variables which store our shader, textures, mesh and unif.
*/
shaShading sha, sha2, sha3;
texTexture texture, texture2, texture3, texture4;
const texTexture *textures[4] = {&texture, &texture2, &texture3, &texture4};
const texTexture **tex = textures;
meshMesh mesh, mesh2, mesh3;
double unif[3 + 9] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, rotationAngle = 0.0, translationVector[2] = {128.0, 128.0}, unif2[3 + 9] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, rotationAngle2 = 0.0, translationVector2[2] = {128.0, 128.0};
double unif3[3 + 9] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, rotationAngle3 = 1.0, translationVector3[2] = {50, 50};


/**
 * Renders the triangle on the screen
*/
void render(void) {
    pixClearRGB(0.0, 0.0, 0.0);
    meshRender(&mesh2, &sha2, unif2, tex);
    meshRender(&mesh, &sha, unif, tex);
    meshRender(&mesh3, &sha3, unif3, tex);
}

/**
 * Changes the type of filtering of the image when enter is pressed.
*/
void handleKeyUp(
        int key, int shiftIsDown, int controlIsDown, int altOptionIsDown, 
        int superCommandIsDown) {
    if (key == GLFW_KEY_ENTER) {
        if (texture.filtering == texLINEAR)
            texSetFiltering(&texture, texNEAREST);
        else
            texSetFiltering(&texture, texLINEAR);
        render();
    }
}

/**
 * Animates the image, sets the rotation and translation of the mesh.
 * Prints the framerate on the screen
*/
void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    unif[UNIFR] = sin(newTime);
    unif[UNIFG] = cos(oldTime);
    double isom[3][3], isom2[3][3], isom3[3][3];
    translationVector[0] = 256.0 + cos(newTime) * 128.0;
	translationVector[1] = 256.0 + sin(newTime) * 128.0;
    translationVector3[0] = 100.0 + cos(newTime) * 200.0;
	translationVector3[1] = 100.0 + sin(newTime) * 200.0;
	rotationAngle += (newTime - oldTime) * 3.0;
    rotationAngle2 += (newTime - oldTime) * 4.0;
    rotationAngle3 += (newTime - oldTime) * 6.0;
    mat33Isometry(rotationAngle, translationVector, isom);
    mat33Isometry(rotationAngle2, translationVector2, isom2);
    mat33Isometry(rotationAngle3, translationVector3, isom3);
    vecCopy(9, (double *)isom, &unif[UNIFMODELING]);
    vecCopy(9, (double *)isom2, &unif2[UNIFMODELING]);
    vecCopy(9, (double *)isom3, &unif3[UNIFMODELING]);
    render();
}


/**
 * Starts the program, runs the code. Initializes and finalizes textures, screen, meshes, shaders.
*/
int main(void) {
    if (pixInitialize(512, 512, "Shader Program") != 0)
        return 1;
    if ((texInitializeFile(&texture, "tennis.jpg") != 0) || (texInitializeFile(&texture2, "background.jpg") != 0) || (texInitializeFile(&texture3, "meme.jpg") != 0)|| (texInitializeFile(&texture4, "orange.jpg") != 0)) {
        pixFinalize();
        return 2;
    }
    if ((mesh2DInitializeEllipse(&mesh, 0.0, 0.0, 75, 75, 40) != 0) ||(mesh2DInitializeRectangle(&mesh2,-600, 600, -600, 600) != 0) || (mesh2DInitializeRectangle(&mesh3,0, 50, 0, 50) != 0)) {
        texFinalize(&texture);
        texFinalize(&texture2);
        texFinalize(&texture3);
        texFinalize(&texture4);
        pixFinalize();
        return 3;
    }
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    texSetFiltering(&texture2, texNEAREST);
    texSetLeftRight(&texture2, texREPEAT);
    texSetTopBottom(&texture2, texREPEAT);
    texSetFiltering(&texture3, texNEAREST);
    texSetLeftRight(&texture3, texREPEAT);
    texSetTopBottom(&texture3, texREPEAT);
    texSetFiltering(&texture4, texNEAREST);
    texSetLeftRight(&texture4, texREPEAT);
    texSetTopBottom(&texture4, texREPEAT);
    /* New! The shader program now records which shader functions to use. */
    sha.unifDim = 3 + 9;
    sha.attrDim = 2 + 2;
    sha.varyDim = 2 + 2;
    sha.texNum = 1;
    sha.shadeVertex = shadeVertex;
    sha.shadeFragment = shadeFragment;
    sha2.unifDim = 3 + 9;
    sha2.attrDim = 2 + 2;
    sha2.varyDim = 2 + 2;
    sha2.texNum = 1;
    sha2.shadeVertex = shadeVertex2;
    sha2.shadeFragment = shadeFragment2;
    sha3.unifDim = 3 + 9;
    sha3.attrDim = 2 + 2;
    sha3.varyDim = 2 + 2;
    sha3.texNum = 2;
    sha3.shadeVertex = shadeVertex3;
    sha3.shadeFragment = shadeFragment3;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    meshFinalize(&mesh);
    meshFinalize(&mesh2);
    meshFinalize(&mesh3);
    texFinalize(&texture);
    texFinalize(&texture2);
    texFinalize(&texture3);
    texFinalize(&texture4);
    pixFinalize();
    return 0;
}


