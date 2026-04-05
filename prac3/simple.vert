#version 330 core

layout(location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(inPosition, 1.0);
}