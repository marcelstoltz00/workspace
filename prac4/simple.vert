#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inShadeFactor;
layout(location = 2) in float inAlphaFactor;

uniform mat4 model;
uniform mat4 projection;

out vec3 fragPosition;
out vec3 localPosition;
out float fragShadeFactor;
out float fragAlphaFactor;

void main()
{
    vec4 worldPosition = model * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;
    localPosition = inPosition;
    fragShadeFactor = inShadeFactor;
    fragAlphaFactor = inAlphaFactor;
    gl_Position = projection * worldPosition;
}
