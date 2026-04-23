#version 330 core

uniform vec3 objectColor;
uniform int receivesLight;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float objectAlpha;

in vec3 fragPosition;
out vec4 fragColor;

void main()
{
    vec3 finalColor = objectColor;

    if (receivesLight == 1) {
        vec3 toLight = lightPosition - fragPosition;
        float distanceToLight = length(toLight);
        vec3 lightDir = normalize(toLight);

        // Floor-oriented normal for the current baseline scene.
        vec3 floorNormal = vec3(0.0, 1.0, 0.0);
        float diffuse = max(dot(floorNormal, lightDir), 0.0);

        float attenuation = 1.0 / (1.0 + 0.25 * distanceToLight + 0.12 * distanceToLight * distanceToLight);
        vec3 ambient = ambientStrength * objectColor;
        vec3 projectedLight = diffuse * attenuation * (objectColor * lightColor);

        finalColor = ambient + projectedLight;
    }

    fragColor = vec4(finalColor, objectAlpha);
}