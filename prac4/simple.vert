#version 330 core

layout(location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 projection;

out vec3 fragPosition;

void main()
{
    vec4 worldPosition = model * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;
    gl_Position = projection * worldPosition;
}