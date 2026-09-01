#include "CWindow/Renderer/OpenGL/Renderer.h"
#include "CWindow/Gui/OpenGL/Gui.h"
#include "Shaders.h"


int main(){
  // init window and renderer
  CW::Renderer::Renderer window(true);
  window.setVsync(0);
  window.setWindowTitle("Compute Shader");
  
  std::vector<float> data = {2, 1, 2, 3, 4, 5};
  
  CW::Renderer::ComputeShader compute(ComputeShader::compute);
  compute.run<float>(data, data.size()); // each thread one value
  
  data = compute.get<float>();
  

  bool init = true;
  printf("[");
  for(float el : data){
    if(init)
      printf("%.f", el);
    else
      printf(", %.f", el);
    init = false;
  }
  printf("]\n");

  
  return 0;
}