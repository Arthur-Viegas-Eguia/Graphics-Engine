


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
#include "140texture.c"
#include "140triangle.c"
/*These variables determine the vertexes of the triangle, 
its rgb color and st coordinates*/
double a[2] = {20.0, 200};
double b[2] = {400, 10};
double c[2] = {250, 412};
double rgb[3] = {1.0, 1.0, 0.0};
double angle = 0.0;
double alpha[3] = {0.0, 0.0};
double beta[3] = {1.0, 0.0};
double gamma[3] = {0.0, 1.0};

//Runs the program. Tests the functions
int main(void) {
    if (pixInitialize(512, 512, "Testing") != 0)
        return 1;
    texTexture img;
    texInitializeFile(&img, "meme.jpg");
    triRender(a, b, c, rgb, &img, alpha, beta, gamma);
    pixRun();
    texFinalize(&img);
    pixFinalize();
    return 0;
}


