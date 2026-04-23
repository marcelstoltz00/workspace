
3
2 Overview
For this practical, you will need to render a semi-translucent 3D glass golf ball resting on a flat plane.
Inside the golf ball, there is a light which shines through the glass and onto the plane below. The
number of vertices that the golf ball and the plane need to be adjustable, such that the effect of the
number of vertices on the overall quality of the render can be observed.
3 Your Task:
For this practical, you will need to render a scene similar to the sketch depicted in Figures 1 and 2:
Figure 1: Side depiction of the required render
Figure 2: Top-down depiction of the required render
In Figures 1 and 2, the white box represents the flat plane (floor) that the golf ball will rest on,
and the grey circle/sphere represents the golf ball. The yellow dot represents the initial position of
the light that is inside the golf ball, and the orange lines/circle represent the light that is shining
onto the floor. Remember, although Figures 1 and 2 are 2D, your render needs to be a single 3D
scene.
In the sections below, the different requirements are laid out.
3.1 Shape requirements
Your rendering should contain the following shapes:
4
• A sphere whose number of vertices can be increased and decreased during runtime.
– Assign two keys on your keyboard (of your choosing) to control the number of vertices
assigned to the sphere.
• A flat plane whose number of vertices can be increased and decreased during runtime.
– Assign two keys on your keyboard (of your choosing) to control the number of vertices
assigned to the floor.
– Hint: This may require you to create a series of smaller planes that are connected to form
a bigger flat plane
3.2 Texture requirements
To make the golf ball appear more realistic, texture mapping needs to be applied. You will be
required to create your own texture maps for this practical. The downloading or use of a pre-existing
texture map will result in forfeiting all marks for the texture requirement. The following two texture
maps are required:
• A colour texture map that will shade the golf ball such that it is the correct shaded regions
(dimples).
• A displacement texture map that will be able to morph the surface of the golf ball such that
the surface has accurate golf ball dimples.
• An alpha texture map that will make the dimples transparent while the normal surface is
non-transparent.
The different texture maps should be enabled and disabled using the following buttons:
• The B Key should be used to toggle the colour texture map on and off. When the colour texture
map is disabled, the sphere should be a uniform translucent colour. When the colour texture
map is enabled, the sphere should have dimples that is shaded a bit darker than the rest of the
golf ball.
• The N Key should be used to toggle the displacement texture map on and off. When the
displacement texture map is disabled, the surface of the golf ball should be smooth, but when
the displacement texture map is enabled, the surface should have visible dimples (in other
words, should not be smooth). Note that the actual surface should be changed. The use of
bump texture mapping is not allowed.
• The M Key should be used to toggle the alpha texture map on and off. When disabled, the entire
golf ball should be semi-transparent according to an alpha value, which is changed with the +
and - keys. When the alpha texture map is enabled, only the dimples should be transparent,
according to the same alpha value previously mentioned, and the rest of the golf ball should
be fully opaque.
The colour of the golf ball can change (discussed in the following section) and this should be
taken into account when applying the texture mappings as discussed above.
5
3.3 Colour requirements
You will need to comply with the following colour requirements:
• Floor
– Assign two keys on your keyboard (of your choosing) to control the colour of the floor.
– Using these keys, you will need to be able to cycle through the following colours:
∗ Red
∗ Green
∗ Blue
∗ White
∗ Black
∗ 5 other distinct colours of your choosing.
• Glass Golf Ball
– Assign two different keys on your keyboard (of your choosing) to control the colour of the
glass golf ball.
– Using these keys, you will need to be able to cycle through the following colours:
∗ Red
∗ Green
∗ Blue
∗ White
∗ Black
∗ 5 other distinct colours of your choosing.
– Assign two further keys (the + and - keys) to control the alpha value of the glass golf
ball.
– Hint: Make use of RGBA and assign the colour buffer to take in RGBA colours1 instead
of the standard RGB2 colours.
• Light
– Assign two different keys on your keyboard (of your choosing) to control the colour of the
light.
– Using these keys, you will need to be able to cycle through the following colours:
∗ Red
∗ Green
∗ Blue
∗ White
1A vec4 value.2A vec3 value.
6
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