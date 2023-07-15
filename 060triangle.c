#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"

/** Receives the coordinates of the vertices of a triangle
 * in counterclockwise direction and its color, and renders
 * it, in a solid color to the screen.
*/
void triRender(double a0, double a1, double b0, double b1, double c0, double c1, double r, double g, double b) {
    /*Checks if a is the leftmost vertex of the triangle.
      If it is not the case, the program rotates the triangle,
      by calling the function recursively, so a becmoes the
      leftmost vertex of the triangle. */
    if(b0 < c0 && b0 < a0){
        triRender(b0, b1, c0, c1, a0, a1, r, g, b);
    }
    else if(c0 < b0 && c0 < a0)
        triRender(c0, c1, a0, a1, b0, b1, r, g, b);
    else{
        int i, j;
        /* Checks whether c has lower x coordinates than b,
           if that is the case the triangle points up, and
           the program will draw it from the staight line
           joining a to b, reaching until the line AC, and
           them from the line joining a to b reaching until
           line CB.
        */
        if(c0 < b0){
            //Checks for a division by 0 and handles it if necessary
            if(!(a0 == b0 || a0 == c0)){
                //Draws the first half of the triangle
                for(i = ceil(a0); i <= floor(c0); i++){
                    for(j = ceil((a1 +(((b1 - a1)/(b0 - a0)) * (i - a0)))); j <= floor((a1+(((c1 - a1)/(c0 - a0)) * (i - a0)))); j ++){
                        pixSetRGB(i, j, r, g, b);
                    }
                }
            }
            //Draws the second half of the triangle
            for(i = floor(c0) + 1; i <= floor(b0); i++){
                for(j = ceil((a1 +(((b1 - a1)/(b0 - a0)) * (i - a0)))); j <= floor((c1 + (((b1 - c1)/(b0 - c0)) * (i - c0)))); j ++){
                    pixSetRGB(i, j, r, g, b);
                }
            }
        }
        /* The program enters this code block if b has lower
           coordinates than c, in which case the triangle points
           down. The program will draw the triangle from line
           segment AB until the straight line AC, and then from
           line segment BC until the straight line AB.
        */
        else{
            //Detects possible divisions by 0 and deals with them
            if(!(a0 == b0 || a0 == c0)){
                //Draws the first half of the triangle
                for(i = ceil(a0); i <= floor(b0); i++){
                    for(j = ceil((a1 + (((b1 - a1)/(b0 - a0)) * (i - a0)))); j <= floor((a1 + (((c1 - a1)/(c0 - a0)) * (i - a0)))); j++){
                        pixSetRGB(i, j, r, g, b);
                    }
                }
            }
            //Detects possible divisions by 0 and deals with them
            if(!(b0 == c0)){
                //Draws the second half of the triangle
                for(i = floor(b0) + 1; i <= floor(c0); i++){
                    for(j = ceil((b1 + (((c1 - b1)/(c0 - b0)) * (i - b0)))); j <= floor((a1 + (((c1 - a1)/(c0 - a0)) * (i - a0)))); j++){
                        pixSetRGB(i, j, r, g, b);
                    }
                }
            }
        }
    }
}






