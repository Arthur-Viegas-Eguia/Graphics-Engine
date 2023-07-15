#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"

/** Receives the coordinates of the vertices of a triangle
 * in counterclockwise direction and its color, and renders
 * it, in a solid color to the screen.
*/
void triRender(const double a[2], const double b[2], const double c[2], const double rgb[3]) {
    /*Checks if a is the leftmost vertex of the triangle.
      If it is not the case, the program rotates the triangle,
      by calling the function recursively, so a becmoes the
      leftmost vertex of the triangle. */
    if(b[0] < c[0] && b[0] < a[0]){
        triRender(b, c, a, rgb);
    }
    else if(c[0] < b[0] && c[0] < a[0])
        triRender(c, a, b, rgb);
    else{
        int i, j;
         
        /* Checks whether c has lower x coordinates than b,
           if that is the case the triangle points up, and
           the program will draw it from the staight line
           joining a to b, reaching until the line AC, and
           them from the line joining a to b reaching until
           line CB.
        */
        if(c[0] < b[0]){
            //Checks for a division by 0 and handles it if necessary
            if(!(a[0] == b[0] || a[0] == c[0])){
                //Draws the first half of the triangle
                for(i = ceil(a[0]); i <= floor(c[0]); i++){
                    for(j = ceil((a[1] +(((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((a[1]+(((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j ++){
                        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                    }
                }
            }
            //Draws the second half of the triangle
            for(i = floor(c[0]) + 1; i <= floor(b[0]); i++){
                for(j = ceil((a[1] +(((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((c[1] + (((b[1] - c[1])/(b[0] - c[0])) * (i - c[0])))); j ++){
                    pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                }
            }
        }
        /* The program enters this code block if b has lower
           coordinates than c, in which case the triangle points
           down. The program will draw the triangle from line
           segment AB until the straight line AC, and then from
           line segment BC until the straight line AC.
        */
        else{
            //Detects possible divisions by 0 and deals with them
            if(!(a[0] == b[0] || a[0] == c[0])){
                //Draws the first half of the triangle
                for(i = ceil(a[0]); i <= floor(b[0]); i++){
                    for(j = ceil((a[1] + (((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((a[1] + (((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j++){
                        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                    }
                }
            }
            //Detects possible divisions by 0 and deals with them
            if(!(b[0] == c[0])){
                //Draws the second half of the triangle
                for(i = floor(b[0]) + 1; i <= floor(c[0]); i++){
                    for(j = ceil((b[1] + (((c[1] - b[1])/(c[0] - b[0])) * (i - b[0])))); j <= floor((a[1] + (((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j++){
                        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                    }
                }
            }
        }
    }
}







