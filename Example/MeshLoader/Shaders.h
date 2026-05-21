#pragma once
#include <string>

namespace Shader {

const std::string vertex = R"(#version 430 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

uniform mat4 model;
uniform mat4 transformation;

out vec3 fragNormal;
out vec3 fragPos;
out vec3 fragColor;

void main()
{
    fragPos = vec3(model * vertexPosition);

    fragNormal = mat3(transpose(inverse(model))) * vertexNormal;

    fragColor = vertexColor;

    gl_Position = transformation * vertexPosition;
})";

const std::string fragment = R"(#version 430 core

in vec3 fragNormal;
in vec3 fragPos;
in vec3 fragColor;

out vec4 out_color;

uniform vec3 lightPos;
uniform vec3 lightColor;

// MATERIAL (BASE COLOR SOURCE)
uniform vec3 albedo;
uniform float roughness;
uniform float metallic;
uniform float ao;
uniform float specular;

// TEXTURE
uniform sampler2D albedoMap;
uniform bool useTexture;

void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(
        max(dot(norm, halfwayDir), 0.0),
        mix(64.0, 8.0, roughness)
    );

    float ambient = 0.1 * ao;

    // =========================
    // BASE COLOR PIPELINE
    // =========================

    vec3 baseColor = albedo;

    // 1. texture overrides or modulates albedo
    if (useTexture)
        baseColor *= texture(albedoMap, fragPos.xy).rgb;

    // 2. vertex color acts as tint (NOT full override)
    baseColor *= fragColor;

    // =========================
    // LIGHTING
    // =========================

    vec3 diffuse = diff * baseColor * lightColor;
    vec3 specularCol = spec * specular * lightColor;

    vec3 color = ambient * baseColor + diffuse + specularCol;

    // metallic approximation
    color = mix(color, color * baseColor, metallic);

    out_color = vec4(color, 1.0);
})";

};