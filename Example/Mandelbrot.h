#pragma once
#include "string"

namespace Mandelbrot{
std::string vertex = R"(
#version 430 core

layout(location = 0) in vec2 aPos;
void main() {
  gl_Position = vec4(aPos, 0.0, 1.0); 
}
)";

std::string fragment = R"(
#version 430 core

out vec4 FragColor;

uniform vec3 colors;
uniform int maxIter;
uniform vec2 z;

void main() {
    vec2 a1 = (gl_FragCoord.xy / vec2(800.0, 600.0)) * 3.0 - vec2(2.0, 1.5);
    vec2 c = z;
    if (z.x == 0.0f && z.y == 0.0f){
        a1 = z;
        c = (gl_FragCoord.xy / vec2(800.0, 600.0)) * 3.0 - vec2(2.0, 1.5);
    }
    vec2 a2 = a1;
    
    int j = 0;
    for(int i = 0; i < maxIter; i++){
        a2.x = a1.x * a1.x - a1.y * a1.y + c.x;  // Real part of (a1 * a1 + c)
        a2.y = 2.0 * a1.x * a1.y + c.y;   
        
        if (length(a2) > 2.0f){
            break;
        };

        a1 = a2;
        j = j + 1;
    };

    float t = float(j) / float(maxIter); 
    vec3 color = vec3(
        0.5 + 0.5 * sin(t * colors.x + 0.0),
        0.5 + 0.5 * cos(t * colors.y + 1.0),
        0.5 + 0.5 * sin(t * colors.z + 2.0)
    );

    float smoothColor = smoothstep(0.0, 1.0, length(a2));
    
    FragColor = vec4(color * smoothColor, 1.0);
}
)";
};