#version 330 core

uniform vec3 objectColor;
uniform int receivesLight;
uniform int objectKind;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 secondLightPosition;
uniform vec3 secondLightColor;
uniform vec3 ballTintColor;
uniform float ambientStrength;
uniform float objectAlpha;
uniform float ballAlpha;

in vec3 fragPosition;
in vec3 localPosition;
out vec4 fragColor;

vec3 accumulatePointLight(vec3 activeLightPosition, vec3 activeLightColor, vec3 normalDir, vec3 filterColor)
{
    vec3 toLight = activeLightPosition - fragPosition;
    float distanceToLight = length(toLight);
    vec3 lightDir = normalize(toLight);
    float diffuse = max(dot(normalDir, lightDir), 0.0);
    float attenuation = 1.0 / (1.0 + 0.25 * distanceToLight + 0.12 * distanceToLight * distanceToLight);
    return diffuse * attenuation * (objectColor * activeLightColor * filterColor);
}

void main()
{
    vec3 finalColor = objectColor;

    if (receivesLight == 1) {
        vec3 normalDir = vec3(0.0, 1.0, 0.0);
        if (objectKind == 2) {
            normalDir = normalize(localPosition);
        }

        vec3 ambient = ambientStrength * objectColor;
        vec3 ballFilter = mix(vec3(1.0), ballTintColor, clamp(ballAlpha, 0.0, 1.0) * 0.75);
        vec3 projectedLight = accumulatePointLight(lightPosition, lightColor, normalDir, ballFilter);
        vec3 secondaryLight = accumulatePointLight(secondLightPosition, secondLightColor, normalDir, ballFilter);

        finalColor = ambient + projectedLight + secondaryLight;
    }

    fragColor = vec4(finalColor, objectAlpha);
}
