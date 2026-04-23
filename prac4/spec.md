
7

Automatic Zoom
Department of Computer Science
Faculty of Engineering, Built Environment & IT
University of Pretoria
COS344 - Computer GraphicsCopyright ©2026 by FJ Redelinghuys. All rights reserved.
Practical 4 Specification: Translucency, Textures
and Light
Release Date: 16-03-2026 at 06:00
Start By Date: 14-04-2026
Due Date: 11-05-2026 at 10:00
Total Marks: 80
1
∗ 5 other distinct colours of your choosing.
– Note that the light and glass colour should also affect the floor’s colour. In other words,
if the light is yellow, the glass is green, and the floor is purple, the resulting colour of the
floor will be a combination of the three colours (remember to take into account how much
light shines onto each specific vertex of the floor).
3.4 Light requirements
Your light should have the following requirements:
• The light should be a point light, initially positioned in the centre of the golf ball.
• You should at least make use of the pseudo code described in Chapter 4 of the prescribed
textbook to calculate the lighting for a point source.
• The light’s projection will only be displayed on the floor.
• The light’s projection should not affect the golf ball’s perception, but this can be added in as
bonus marks.
3.5 Transformation requirements
In this section, the transformation or animation requirements for your rendering are discussed. Keep
the animation rates small such that it allows you to press the key multiple times before going out of
camera view.
3.5.1 Rotations
• When the W key is pressed, the scene needs to rotate about the x-axis.
• When the S key is pressed, the scene needs to rotate about the x-axis in the opposite direction
to the W key.
• When the A key is pressed, the scene needs to rotate about the y-axis.
• When the D key is pressed, the scene needs to rotate about the y-axis in the opposite direction
to the S key.
• When the E key is pressed, the scene needs to rotate about the z-axis.
• When the Q key is pressed, the scene needs to rotate about the z-axis in the opposite direction
to the E key.
3.5.2 Translation
• When the up arrow key is pressed, the light’s local y-axis position needs to increase.
• When the down arrow key is pressed, the light’s local y-axis position needs decrease.
7
• When the left arrow key is pressed, the light’s local x-axis position needs decrease.
• When the right arrow key is pressed, the light’s local x-axis position needs increase.
• When the > key is pressed, the light’s local z-axis position needs to increase.
• When the < key is pressed, the light’s local z-axis position needs to decrease.
Note, through all the transformations of the light, the basic scene setup should remain still, apart
from the position of the light. Remember that the local coordinate system of the light can be different
from the global coordinate system.
3.5.3 Reset Command
In order to reset the render to its initial state, the Space Key should be used. This should reset the
number of vertices, positions and rotations of the entire scene to the initial state of the render.
3.6 Wireframe
You are also required to implement a wireframe for your render. The wireframe should maintain the
colour scheme of your render, such that the colours of the different shapes can be identified. The
wireframe should also conform to all the transformations described earlier.
Your program should toggle between the wireframe and the normal render when the Enter key
is pressed. Hint: you may need to implement a time delay between key presses for the wireframe
toggling, so that the expected behaviour is achieved.
Please note that you must use the GL_LINES to implement your wireframe. Using the glPolygonMode
function will result in the forfeiting of your wireframe marks.
3.7 Texture Maps
You will need to create three texture maps as described earlier. Note, you may not simply download
a texture map; you will need to create your own. For each texture map that you correctly created,
you will receive 4 marks. During the demo, you will need to explain how you created the texture
map. Remember to include the texture maps in your submission.
4 Marking rubric
The following rubric will be used to mark your submitted assignment. Note you will be demoing the
practical during the practical sessions on your own computer or a lab computer. Please see Table 1
for the rubric. Note: 1 mark will be subtracted for each transformation if the render is moved back
to the centre before a new transformation is applied.
5 Bonus marks
There are 10 bonus marks available. Things that can be done for bonus marks are:
8
• To sort your glass faces, use the Binary Search Tree Partitioning algorithm (5 marks).
• Add 5 different types of material finishes to the golf ball using another texture map (1 mark
for each material finish).
• Adding a second, which is not an ambient light (2 marks).
• Adding edges (with glLines) to the glass such that each face can be easily distinguished (2
marks).
• Allow the colour of the floor to change as the floor moves behind the coloured golf ball (2
marks).
• Allowing the light to also affect the glass causes a spectral lighting effect on the glass (5 marks).
• Adding stained glass regions to the glass. For you to obtain your marks, you will need to add
ray tracing so that the light that intersects the different regions affects the light’s projection
on the floor. For each new region you correctly add, you will get 2 marks.
• Allow the light to affect the golf ball’s perception (2 marks).
6 Implementation Details
• You need to use OpenGL version 3.3 for this practical.
• You may not use any of the build-in mathematical libraries within the glm package. This
included matrix arithmetic. Hint: You may use your practical 1 in this practical.
• You may not use any of the built-in OpenGL functions to generate the shapes for you. You
need to create each shape from first principles.
• You may not use any of the built-in OpenGL functions to perform the transformations of the
shapes. You need to transform each shape from first principles either explicitly or by using the
matrix arithmetic techniques discussed in class.
• You may only use the following C++ and OpenGL libraries:
– stdio.h
– stdlib.h
– iostream
– iomanip
– cmath
– sstream
– GL/glew.h
– GLFW/glfw3.h
9
