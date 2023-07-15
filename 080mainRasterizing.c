#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"
#include "080triangle.c"
#include "080vector.c"

/**
 * Runs the code and tests the program
*/
int main(void) {
    if(pixInitialize(512, 512, "Arthur's awesome sexy triangle") != 0){
        return 1;
    }
    double a[2] = {50, 10}, b[2] = {500, 10}, c[2] = {450, 50}, rgb[3] = {1, 1, 1};
    pixClearRGB(0.0, 0.0, 0.0);
    triRender(a,b,c,rgb);
    pixRun();
    pixFinalize();
    return 0;
    return 0;
}


