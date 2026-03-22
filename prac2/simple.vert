#version 330 core

layout(location = 0) in vec3 inPosition;
uniform float uAspect;


void main()
{
    vec2 p = inPosition.xy;
    p.x /= uAspect;
    gl_Position = vec4(p, 0.0, 1.0);
}