#pragma once
#include <string.h>
#include "Material/PBRMaterial.h"




namespace Shader {
const std::string vertex = R"(#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 transformation;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;

void main()
{
  FragPos = vec3(model * aPos);
  Normal = mat3(transpose(inverse(model))) * aNormal;

  gl_Position = transformation * aPos;
})";


const std::string fragment =
R"(#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

)"
+ CW::PBRShaderBRDFUniforms +
R"(

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

)"
+ CW::PBRShaderBRDFInclude + 
R"(

// ---------------- MAIN ----------------

void main()
{
  vec3 color = BRDF(
    Normal,
    FragPos,
    cameraPos,
    lightPos,
    lightColor
  );

  FragColor = vec4(color, 1.0);
}
)";

};