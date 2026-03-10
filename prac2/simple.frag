#version 330 core

uniform vec3 inColor;
out vec4 fragColor;

void main()
{
    // Minimal solid-color skeleton.
    fragColor = vec4(inColor, 1.0);
}