# Triangle Rasterization Software
This is the implementation of the triangle rasterization algorithm I wrote for CS 311 Computer Graphics, taught By Josh Davis.
This project also implements other important concepts in graphics such as interpolation, linear filtering, meshes, rendering of 2D and 3D objects, shaders, orthographic and perspective projection, Cameras, Backface culling, and Clipping.
To run just download the folder and run `clang 340mainLandscape.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit` on Mac or `cc 340mainLandscape.c 040pixel.o -lglfw -lGL -lm -ldl` on Linux

The files were originally written as homework exercises to be submitted before every class. The file naming convention of the class was quite strict, files which start with a larger number were written later in the term. Some files were the updated/upgraded version of homework assignments delivered earlier. For example, 080triangle is the same as 060triangle but with additional features (as it was done later). Files where the name start with main can be executed, and were used to test/debug the code throughout the term. Feel free to compile/run them. 
