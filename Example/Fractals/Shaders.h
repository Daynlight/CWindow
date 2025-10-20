#pragma once
#include "string"

namespace Fractal{
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
uniform vec2 world_pos;
uniform float zoom;
uniform vec2 window_ratio;


void main() {
    float ratio = window_ratio.y / window_ratio.x;
    vec2 a1 = ((gl_FragCoord.xy - window_ratio / 2) / window_ratio) * zoom + world_pos / window_ratio;
    a1.y = a1.y * ratio;
    
    vec2 c = z;
    if (z.x == 0.0f && z.y == 0.0f){
        a1 = z;
        c = a1;
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