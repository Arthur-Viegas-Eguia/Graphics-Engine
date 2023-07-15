


/* On macOS, compile with...
    clang 160mainAbstracted.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 160mainAbstracted.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "150texture.c"
#include "200shading.c"

/**These constansts represent what information each index
 * of the arrays sent from trirender store.
 */
#define ATTRX 0
#define ATTRY 1
#define ATTRS 2
#define ATTRT 3
#define VARYX 0
#define VARYY 1
#define VARYS 2
#define VARYT 3
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define UNIFROT 3
#define UNIFTRANSLX 4
#define UNIFTRANSLY 5
#define TEXR 0
#define TEXG 1
#define TEXB 2

/** This functions takes st coordinates sent
 * from tri-render to take a texture from tex,
 * after that it modulates the image by the
 * color in uniform vector. It saves the resulting color on
 * rgb
 */
void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int varyDim, const double vary[], double rgb[3]) {
		double texColor[3], texUnif[3];
    	texSample(tex[0], vary[VARYS], vary[VARYT], texColor);
		vecModulate(3, &unif[UNIFR], texColor, rgb);
}

/* Outputs vary, based on the other parameters, which are unaltered. Like 
shadeFragment, this function should not access any variables other than its 
parameters and any local variables that it declares. */
void shadeVertex(
        int unifDim, const double unif[], int attrDim, const double attr[], 
        int varyDim, double vary[]) {
    double rot[2][2];
	mat22Rotation(unif[UNIFROT], rot);
	mat221Multiply(rot, attr, vary);
	vary[VARYX] += unif[UNIFTRANSLX];
	vary[VARYY] += unif[UNIFTRANSLY];
	vary[VARYS] = attr[ATTRS];
    vary[VARYT] = attr[ATTRT];
}

/* We have to include triangle.c after defining shadeFragment, because triRender 
refers to shadeFragment. (Later in the course we handle this issue better.) */
#include "170triangle.c"
#include "200mesh.c"
#include "190mesh2D.c"

/* This struct is initialized in main() below. */
shaShading sha;
/* Here we make an array of one texTexture pointer, in such a way that the 
const qualifier can be enforced throughout the surrounding code. C is confusing 
for stuff like this. Don't worry about mastering C at this level. It doesn't 
come up much in our course. */
texTexture texture;
const texTexture *textures[1] = {&texture};
const texTexture **tex = textures;
meshMesh mesh;
double unif[6] = {0.5, 0.5, 0.5, 0, 10, 20};

/** Renders the triangle,  tests the filter
 *
 */
void render(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	unif[UNIFTRANSLX] += + 0.7;
	unif[UNIFTRANSLY] += 0.5;
	unif[UNIFROT] += 0.001;
	meshRender(&mesh, &sha, unif, tex);
}


/** Changes the type of filter from linear to
 * nearest neighbor or vice versa if enter is
 * pressed

 */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown, 
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		if (texture.filtering == texLINEAR)
			texSetFiltering(&texture, texNEAREST);
		else
			texSetFiltering(&texture, texLINEAR);
		render();
	}
}

//Prints the framerate of the program on the screen
void handleTimeStep(double oldTime, double newTime) {
	render();
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
}


//Runs the program, initializes the
//files
int main(void) {
	if (pixInitialize(512, 512, "Abstracted") != 0)
		return 1;
	if (texInitializeFile(&texture, "test.jpg") != 0) {
	    pixFinalize();
		return 2;
	}
	double r = 350, l = 50, t = 400, b = 200;
	mesh2DInitializeRectangle(&mesh, l, r, b, t);
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    sha.unifDim = 3 + 3;
	sha.attrDim = 4;
    sha.varyDim = 4;
    sha.texNum = 1;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
	meshFinalize(&mesh);
    texFinalize(&texture);
    pixFinalize();
    return 0;
}


