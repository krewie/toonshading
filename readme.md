
#Toon shading
![Alt Text](https://github.com/krewie/toonshading/blob/master/documentation/bunny.gif)
##Introduction
Toon shading or cel shading is a technique
that transforms the shading of a 3D model
so that it resembles classical animation
drawings. There are multiple techniques
for achieving a toon shaded 3D model with
varying results. The most simplest solution
is to draw the 3D model using two shaders
with different scales on the model. Using
this technique does not however color the
outline of inner structures within the 3D
model. Another solution is to implement
a two pass rendering whereas the outline
color and the actual color of the 3D model
is colored seperatly. The important parts
for achieveing a toon shaded model is to
emphasize the color of the outlines and to
restrict the number of colors that is used to
represent the 3D model.

##Implementation
As previously explained, there are multiple
ways of achieving a toon shaded 3D model.
The approach i used was using a variation
of the blinn phong shading model. In order
to color the outlines of the 3D model,
a simple edge detection had to be implemented.
This was done by using Lamberts
cosine law to recognize the angle at which
the fragments should be colored with a outline
color. The results are not perfect for
a toon shading since the outline color can
sometimes excessivly color the outlines of
the 3D model depending on the angle the
model is rotated, it does however color the
outline of inner structures of the 3D model.
In the openGL program, the parameters for
outline color and intensity can be configured
with the provided anttweakbar. The
ambient color is restricted to a maximum
number of colors which can also be configured
within the program, it exagerates the
contrasts between the different colors which
is typical for achieving toon shading.


To run the program, you first have to export ASSIGNMENT3_ROOT so that it points
to the assignment folder and then run ./build.sh

Commands:
export ASSIGNMENT3_ROOT [ROOT_DIR]
./build.sh
