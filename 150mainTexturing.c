


/* On macOS, compile with...
    clang 100mainTesting.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 100mainTesting.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <math.h>
#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "150texture.c"
#include "140triangle.c"


/*Variables corresponding to the coordinates of a triangle,
color, and alpha, beta, and gamma index in every vertex */

double a[2] = {20.0, 200};
double b[2] = {400, 10};
double c[2] = {250, 412};
double rgb[3] = {1.0, 1.0, 0.0};
double angle = 0.0;
double alpha[3] = {0, 0};
double beta[3] = {0, 1};
double gamma[3] = {1, 1};

/*These variables determine the type of filter in the image
and what happens if the image is smaller than the triangle*/
int filter = texNEAREST;
int topB = texREPEAT;
int leftR = texREPEAT; 


/** Listens for when the user presses keys on the keyboard.
 And if the user preses enter it changes the filtering mode from
 linear to nearest pixel (or vice versa), if the user presses L the
 program changes the right/left borders of the image from repeat to
 clip or vice versa. If the user presses b changes the top/bottom
 border changes the image from repeat to clip or vice versa.
 */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
        int altOptionIsDown, int superCommandIsDown) {
    if (key == GLFW_KEY_ENTER){
        if(filter == texNEAREST){
            filter = texLINEAR;
        }
        else{
            filter = texNEAREST;
        }
    }
    else if (key == GLFW_KEY_L){
        if(leftR == texREPEAT){
            leftR = texCLIP;
        }
        else{
            leftR = texREPEAT;
        }
    }
    else if (key == GLFW_KEY_B){
        if(topB == texREPEAT){
            topB = texCLIP;
        }
        else{
            topB = texREPEAT;
        }
    }
}




/** Updates the triangle based on the image edge/filter
 * chosen by the user. Draws the triangle
 */
void handleTimeStep(double oldTime, double newTime) {
    texTexture img;
    texInitializeFile(&img, "perry.jpeg");
    img.filtering = filter;
    img.topBottom = topB;
    img.leftRight = leftR;
    triRender(a, b, c, rgb, &img, alpha, beta, gamma);
    texFinalize(&img);
}



/**
 * Runs the program, testing all functions
 */
int main(void) {
    if (pixInitialize(512, 512, "Testing") != 0)
        return 1;
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    pixFinalize();
    return 0;
}


