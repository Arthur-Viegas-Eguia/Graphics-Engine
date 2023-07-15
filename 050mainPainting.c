#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"

/** These are global variables which determine the amout of red,
 * green and blue light of the stroke of paint respecatively
 */

double red = 1, green = 1, blue = 1;

/** This is a flag which has value 1, if the user is pressing
 * the mouse button (which indicates the program to keep
 * painting) or 0 if the user is not pressing the mouse button
 * (which tells the program to stop painting)
 */

int drawing = 0;

/** Checks if the user has pressed one of the following keys
 * r, g, b, c, m, y, k, w which sets the color of the current pixel to be
 * red, green, blue, cyan, magenta, yellow, balck or white
 * respectively
 */

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
        int altOptionIsDown, int superCommandIsDown) {
    if (key == GLFW_KEY_R){
        red = 1;
        blue = 0;
        green = 0;
    }
    else if (key == GLFW_KEY_G){
        red = 0;
        blue = 0;
        green = 1;
    }
    else if (key == GLFW_KEY_B){
        red = 0;
        blue = 1;
        green = 0;
    }
    else if (key == GLFW_KEY_C){
        red = 0;
        blue = 1;
        green = 1;
    }
    else if (key == GLFW_KEY_M){
        red = 1;
        blue = 1;
        green = 0;
    }
    else if (key == GLFW_KEY_Y){
        red = 1;
        blue = 0;
        green = 1;
    }
    else if (key == GLFW_KEY_K){
        red = 0;
        blue = 0;
        green = 0;
    }
    else if (key == GLFW_KEY_W){
        red = 1;
        blue = 1;
        green = 1;
    }
}

/** Initializes the stroke of paint. sets the RGB of
 * the current pixel to the desired color (or default color
 * if no color was chosen), sets a flag variable to indicate
 * that the user is holding the mouse button.
*/

void handleMouseDown(double x, double y, int button, int shiftIsDown,
        int controlIsDown, int altOptionIsDown, int superCommandIsDown) {
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        drawing = 1;
        pixSetRGB(x,y,red,green,blue);
    }
    
}

/** Receives the information that the user has stopped
 * pressing the mouse button, and sets the flag to a value
 * which determines the program to stop painting
*/

void handleMouseUp(double x, double y, int button, int shiftIsDown,
        int controlIsDown, int altOptionIsDown, int superCommandIsDown) {
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        drawing = 0;
    }
}

/** Tracks the position of the mouse. If the left button of the
 * mouse is being pressed, it sets the color of the pixel to
 * the desired RGB value or the default value if no color was
 * chosen.
 */

void handleMouseMove(double x, double y) {
    if(drawing){
        pixSetRGB(x, y, red, green, blue);
    }
}

/** Initializes and runs the program. Creates the canvas,
 * Sets the event listeners for mouse buttons being pressed,
 * and the mouse being moved
 */
int main(void) {
    //initializing the screen and setting it to 512X512 pixels
    if (pixInitialize(512, 512, "Arthur's paint :D") != 0)
        return 1;
    //Initializing the callbacks
    pixSetKeyDownHandler(handleKeyDown);
    pixSetMouseDownHandler(handleMouseDown);
    pixSetMouseUpHandler(handleMouseUp);
    pixSetMouseMoveHandler(handleMouseMove);
    pixClearRGB(0.7, 0.7, 0.7);
    //Running the event loop
    pixRun();
    //Cleaning up the resources
    pixFinalize();
    return 0;
}


