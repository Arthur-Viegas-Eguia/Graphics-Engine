#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"


/** Does the calculations which result in
 * the interpolated varyings of the triangle at a specific pixel.
 * Draws that point with the calculated color  by the fragment
 * shader on the screen.
*/
void renderPixel(int i, int j, const shaShading *sha, depthBuffer *buf, const double unif[], const texTexture *tex[], const double a[], const double m[2][2], const double betaMinusAlpha[], const double gammaMinusAlpha[]){
    const double x[2] = {i, j};
    double xMinusA[2], pAndQ[2], scaledP[sha->varyDim], scaledQ[sha->varyDim], scaledSum[sha->varyDim], chi[sha->varyDim], rgbd[4];
    vecSubtract(2, x, a, xMinusA);
    mat221Multiply(m, xMinusA, pAndQ);
    vecScale(sha->varyDim, pAndQ[0], betaMinusAlpha, scaledP);
    vecScale(sha->varyDim, pAndQ[1], gammaMinusAlpha, scaledQ);
    vecAdd(sha->varyDim, scaledP, scaledQ, scaledSum);
    vecAdd(sha->varyDim, scaledSum, a, chi);
    sha->shadeFragment(sha->unifDim, unif, sha->texNum, tex, sha->varyDim, chi, rgbd);
    if(rgbd[3] < depthGetDepth(buf, i, j)){
        depthSetDepth(buf, i, j, rgbd[3]);
        pixSetRGB(i, j, rgbd[0], rgbd[1], rgbd[2]);
    }
}




/** Does the basic calculations to interpolate the
 * varyings of a triangle. These calculations are used
 * for every single point in it.
 * Renders the triangle on the screen with the color calculated
 * by the fragment shader.
*/
void triRenderHelper(const shaShading *sha, depthBuffer *buf, const double unif[], const texTexture *tex[], 
        const double a[], const double b[], const double c[]){
        int i, j;

        /*Declares the basic variables to calculate
        the interpolated varyings of a triangle*/
        double column1[2], column2[2], mat[2][2], m[2][2], betaMinusAlpha[sha->varyDim], gammaMinusAlpha[sha->varyDim];

        /*Does the basic calculations that can be used in
        any given point to interpolate the varyings of a triangle*/
        vecSubtract(2,b,a,column1);
        vecSubtract(2, c, a, column2);
        mat22Columns(column1, column2, mat);
        //Prevents the program from drawing a clockwise triangle
        if(!(mat22Invert(mat, m) > 0)){
            return;
        }
        vecSubtract(sha->varyDim, b, a, betaMinusAlpha);
        vecSubtract(sha->varyDim, c, a, gammaMinusAlpha);


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
                        renderPixel(i, j, sha, buf, unif, tex, a, m, betaMinusAlpha, gammaMinusAlpha);
                    }
                }
            }
            //Draws the second half of the triangle
            for(i = floor(c[0]) + 1; i <= floor(b[0]); i++){
                for(j = ceil((a[1] +(((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((c[1] + (((b[1] - c[1])/(b[0] - c[0])) * (i - c[0])))); j ++){
                    renderPixel(i, j, sha, buf, unif, tex, a, m, betaMinusAlpha, gammaMinusAlpha);
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
            if(!(a[0] == b[0] || a[0] == c[0])){
                //Draws the first half of the triangle
                for(i = ceil(a[0]); i <= floor(b[0]); i++){
                    for(j = ceil((a[1] + (((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((a[1] + (((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j++){
                        renderPixel(i, j, sha, buf, unif, tex, a, m, betaMinusAlpha, gammaMinusAlpha);
                    }
                }
            }
            //Detects possible divisions by 0 and deals with them
            if(!(b[0] == c[0])){
                //Draws the second half of the triangle
                for(i = floor(b[0]) + 1; i <= floor(c[0]); i++){
                    for(j = ceil((b[1] + (((c[1] - b[1])/(c[0] - b[0])) * (i - b[0])))); j <= floor((a[1] + (((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j++){
                        renderPixel(i, j, sha, buf, unif, tex, a, m, betaMinusAlpha, gammaMinusAlpha);
                    }
                }
            }
        }
}



/** Receives the coordinates of the vertices of a triangle
 * in counterclockwise direction, and arrays with information
 * about vertices, textures and uniform data. Calls helper functions
 * to render it on the screen.
*/
void triRender(
        const shaShading *sha, depthBuffer *buf, const double unif[], 
        const texTexture *tex[], const double a[], const double b[], 
        const double c[]) {
    if(a[0] <= b[0] && a[0] <= c[0]){
        triRenderHelper(sha, buf, unif, tex, a, b, c);
    }
    else if(b[0] <= c[0] && b[0] <= a[0])
        triRenderHelper(sha, buf, unif, tex, b, c, a);
    else{
        triRenderHelper(sha, buf, unif, tex, c, a, b);
    }
}







