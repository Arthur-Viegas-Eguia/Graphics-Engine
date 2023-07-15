


/* On macOS, compile with...
    clang 250main3D.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 250main3D.c 040pixel.o -lglfw -lGL -lm -ldl
*/

/*List of the dependencies of the code*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "250vector.c"
#include "280matrix.c"
#include "150texture.c"
#include "260depth.c"
#include "260shading.c"
#include "270triangle.c"
#include "280mesh.c"
#include "190mesh2D.c"
#include "250mesh3D.c"
#include "280camera.c"




/*Constants which defines in which position we can find each item in
our attr, unif, vary, and tex arrays*/
#define ATTRX 0
#define ATTRY 1
#define ATTRZ 2
#define ATTRS 3
#define ATTRT 4
#define ATTRN 5
#define ATTRO 6
#define ATTRP 7
#define VARYX 0
#define VARYY 1
#define VARYZ 2
#define VARYW 3
#define VARYS 4
#define VARYT 5
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define UNIFMODELING 3
#define UNIFMPROJ 19
#define TEXR 0
#define TEXG 1
#define TEXB 2


/**
 * Defines the height and width of the window
*/
#define WINDOWH 512
#define WINDOWW 512


/*Determines the vary vector for each vertex of the triangles, the vector will contain the x, y, z, w, s, t
coordinates of the vertex. The first four entries of the array are x, y, z, w*/
void shadeVertex(
        int unifDim, const double unif[], int attrDim, const double attr[], 
        int varyDim, double vary[]) {
	double attrHomog[4] = {attr[0], attr[1], attr[2], 1.0}, varyHomog[4];
	mat441Multiply((double(*)[4])(&unif[UNIFMODELING]), attrHomog, varyHomog);
	//Multiplies by our projection matrix
	mat441Multiply((double(*)[4])(&unif[UNIFMPROJ]), varyHomog, vary);
	vary[VARYS] = attr[ATTRS];
	vary[VARYT] = attr[ATTRT];

}

/*Picks the color of each pixel in the triangles based on a texture and an uniform rgb color*/
void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int varyDim, const double vary[], double rgbd[4]) {
	double sample[tex[0]->texelDim];
	texSample(tex[0], vary[VARYS], vary[VARYT], sample);
	rgbd[0] = sample[TEXR];
	rgbd[1] = sample[TEXG];
	rgbd[2] = sample[TEXB];
	//The projection takes care of making Z negative
	rgbd[3] =  vary[VARYZ];
}

/*Declaration of our shader, texture, unif, meshes, buffer, viewport, camera, and rotation/translation vectors*/
shaShading sha;
texTexture texture;
const texTexture *textures[1] = {&texture};
const texTexture **tex = textures;
meshMesh mesh, mesh2;
depthBuffer buf;
double unif[3 + 16 + 16] = {
    1.0, 1.0, 1.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0,

	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0}, 
	unif2[3 + 16 + 16] = {
    1.0, 1.0, 1.0, 
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0,
	
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0},
	viewport[4][4];
double rotationAngle = 0.25, rotationAngle2 = 0.99999;
double translationVector[3] = {-5.0, -2.75, -35}, translationVector2[3] = {-1.0, -0.2, -35};
camCamera camera;
/*Renders the 3D meshes on the screen, restarts the buffer value*/
void render(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	//Sets the buffer to a high value, so anything can be in front of it
	depthClearDepths(&buf, 10000000000);
	meshRender(&mesh, &buf, viewport, &sha, unif, tex);
	meshRender(&mesh2, &buf, viewport, &sha, unif2, tex);
}

/*Changes the type of filtering when enter is pressed*/
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
	if(key == GLFW_KEY_P){
		if(camera.projectionType == camORTHOGRAPHIC)
			camSetProjectionType(&camera, camPERSPECTIVE);
		else
			camSetProjectionType(&camera, camORTHOGRAPHIC);
		camSetFrustum(&camera, M_PI/6.0, 10.0, 10.0, WINDOWW, WINDOWH);
		render();
	}
}

/*Prints the framerate on the screen, determines the new rotation/translation of the meshes */
void handleTimeStep(double oldTime, double newTime) {
	//if (floor(newTime) - floor(oldTime) >= 1.0)
	//	printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
	unif[UNIFR] = sin(newTime);
	unif[UNIFG] = cos(oldTime);
	double isom[4][4], isom2[4][4], proj[4][4];
	double rotation[3][3];
	double axis[3] = {1.0 / 2.0, 1.0 / 2.0, 1.0 / 2.0};
	vecUnit(3, axis, axis);
	if(camera.projectionType == camORTHOGRAPHIC)
		camGetOrthographic(&camera, proj);
	else
		camGetPerspective(&camera, proj);
	mat33AngleAxisRotation(rotationAngle, axis, rotation);
	mat44Isometry(rotation, translationVector, isom);
	mat44Isometry(rotation, translationVector2, isom2);
	vecCopy(16, (double *)isom, &unif[UNIFMODELING]);
	vecCopy(16, (double *)isom2, &unif2[UNIFMODELING]);
	vecCopy(16, (double *)proj, &unif[UNIFMPROJ]);
	vecCopy(16, (double *)proj, &unif2[UNIFMPROJ]);
	render();
}

/*Initializes the textures, screen, shaders, camera, our box. Runs the code. Tests the program */
int main(void) {
	if (pixInitialize(WINDOWW, WINDOWH, "Three Dimensions") != 0)
		return 1;
	if (texInitializeFile(&texture, "meme.jpg") != 0) {
	    pixFinalize();
		return 2;
	}
	if ((mesh3DInitializeBox(&mesh, 0.0, 7.0, 0.0, 2.1, 0.0, 5.0) != 0) || mesh3DInitializeBox(&mesh2, 0.0, 1.6, 0.0, 0.7, 0, 4.0) != 0) {
	    texFinalize(&texture);
	    pixFinalize();
		return 3;
	}
	if (depthInitialize(&buf, WINDOWW, WINDOWH) != 0) {
		meshFinalize(&mesh2);
		meshFinalize(&mesh);
	    texFinalize(&texture);
	    pixFinalize();
		return 3;
	}
	mat44Viewport(WINDOWW, WINDOWH, viewport);
	camSetProjectionType(&camera,camORTHOGRAPHIC);
	camSetFrustum(&camera, M_PI/6.0, 10.0, 10.0, WINDOWW, WINDOWH);
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    sha.unifDim = 3 + 16 + 16;
    sha.attrDim = 3 + 2 + 3;
    sha.varyDim = 4 + 2;
    sha.shadeVertex = shadeVertex;
    sha.shadeFragment = shadeFragment;
    sha.texNum = 1;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
	depthFinalize(&buf);
	meshFinalize(&mesh2);
    meshFinalize(&mesh);
    texFinalize(&texture);
    pixFinalize();
    return 0;
}


