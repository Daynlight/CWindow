#pragma once
#include "string"

namespace Texture {
std::string vertex = R"(
#version 430 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
  gl_Position = vec4(aPos, 0.0, 1.0);
  TexCoord = aTexCoord;
};
)";

std::string fragment = R"(
#version 430 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform mat3 matrix;
uniform ivec2 radius;

vec2 texelSize = 1.0f / radius;

void main() {
  vec4 pixel = vec4(0.0f);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      vec2 offset = vec2((i - 1) * texelSize.x, (j - 1) * texelSize.y);
      pixel += matrix[j][i] * texture(uTexture, TexCoord + offset);
    }
  }


  FragColor = pixel;
};
)";
};