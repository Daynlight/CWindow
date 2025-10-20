#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Shaders.h"


int main(){
  CW::Renderer::iRenderer* window = new CW::Renderer::Renderer();
  
  // init window and renderer
  window->setVsync(0);
  window->setWindowTitle("Compute Shader");
  

  CW::Renderer::ComputeShader* compute = new CW::Renderer::ComputeShader(ComputeShader::compute);

  std::vector<float> data = {2, 1, 2, 3, 4, 5};
  compute->run<float>(data, data.size()); // each thread one value
  data = compute->get<float>();
  
  for(float el : data)
    printf("%.f, ", el);
  

  delete compute;
  delete window;
  
  return 0;
}