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

uniform vec2 world_pos;
uniform vec2 window_ratio;

uniform vec2 z_0;
uniform vec3 colors;
uniform int maxIter;
uniform float zoom;

uniform int mode;


int julia(vec2 pixel){
    int i = 0;
    vec2 a1 = pixel;
    vec2 a2 = pixel;

    while(i < maxIter){
        a2.x = a1.x * a1.x - a1.y * a1.y + z_0.x;
        a2.y = 2.0 * a1.x * a1.y + z_0.y;   
        
        if (dot(a2, a2) > 4.0)
            break;

        a1 = a2;
        i++;
    };

    return i;
};

int malgenbrot(vec2 pixel){
    int i = 0;
    vec2 a1 = pixel;
    vec2 a2 = pixel;

    while(i < maxIter){
        a2.x = a1.x * a1.x - a1.y * a1.y + pixel.x;
        a2.y = 2.0 * a1.x * a1.y + pixel.y;   
        
        if (dot(a2, a2) > 4.0)
            break;

        a1 = a2;
        i++;
    };

    return i;
};

vec3 colorsByIter(int iter){
    float t = float(iter) / float(maxIter);

    if (iter == maxIter) {
        return vec3(0.0, 0.0, 0.0);
    }

    float r = 0.5 + 0.5 * sin(t * colors.x + 0.0);
    float g = 0.5 + 0.5 * cos(t * colors.y + 2.0);
    float b = 0.5 + 0.5 * sin(t * colors.z + 4.0);

    return vec3(r, g, b);
};


void main() {
    // seting window size
    float ratio = window_ratio.y / window_ratio.x;
    vec2 pixel = ((gl_FragCoord.xy - window_ratio / 2) / window_ratio) * zoom + world_pos / window_ratio;
    pixel.y = pixel.y * ratio;
    
    int i = 0; 
    if(mode == 0) i = malgenbrot(pixel);
    else i = julia(pixel);


    
    FragColor = vec4(colorsByIter(i), 1.0);
};
)";
};