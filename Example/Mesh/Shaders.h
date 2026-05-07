#pragma once
#include "string"

namespace Shader{
const std::string vertex = R"(#version 430 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexColor;

uniform mat4 transformation;
out vec3 color;

void main()
{
  color = vertexColor;
	gl_Position = transformation * vertexPosition;
})";

const std::string fragment = R"(#version 430 core

in vec3 color;
out vec4 out_color;
void main()
{
	out_color = vec4(color, 1.0);
})";
};