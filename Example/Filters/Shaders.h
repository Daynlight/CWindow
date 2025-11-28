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

void main() {
  FragColor = texture(uTexture, TexCoord);
};
)";
};