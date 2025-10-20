#pragma once
#include "string"

namespace ComputeShader{
std::string compute = R"(
#version 430 core

layout (local_size_x = 1) in;

// Declare the buffer properly
layout(std430, binding = 0) buffer Data {
    float data[];  // Buffer for data
};

void main() {
    uint id = gl_GlobalInvocationID.x;  // Get the global ID of the thread
    data[id] = data[id] * 2.0f;  // Multiply the value by 2
}
)";
};