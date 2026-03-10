// simple.vert
// Responsibility:
// - Vertex shader for 2D scene rendering.
// - Receives a 2D position per vertex.
// - Outputs clip-space position for rasterization.

#version 330 core

layout(location = 0) in vec2 inPosition;

// Optional future uniforms (leave commented until needed):
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

void main()
{
    // Minimal pass-through skeleton.
    // Replace with model/view/projection multiplication later if needed.
    gl_Position = vec4(inPosition, 0.0, 1.0);
}