// simple.vert
// Responsibility:
// - Vertex shader for 2D scene rendering.
// - Receives a 2D position per vertex.
// - Outputs clip-space position for rasterization.

#version 330 core

layout(location = 0) in vec3 inPosition;
uniform float uAspect;

// Optional future uniforms (leave commented until needed):
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

void main()
{
    vec2 p = inPosition.xy;
    // Compensate for non-square framebuffers so circles stay circular.
    p.x /= uAspect;
    gl_Position = vec4(p, 0.0, 1.0);
}