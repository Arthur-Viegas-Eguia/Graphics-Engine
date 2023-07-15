#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"
#include "060triangle.c"

/**
 * Runs the code and tests the program
*/
int main(void) {
    if(pixInitialize(512, 512, "Arthur's awesome sexy triangle") != 0){
        return 1;
    }
    pixClearRGB(0.0, 0.0, 0.0);
    triRender(50.5, 10.3, 500.21, 10.0, 500.5, 150.5, 1.0, 1.0, 1.0);
    pixRun();
    pixFinalize();
    return 0;
    return 0;
}


