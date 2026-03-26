#include "Renderer.h"
#include "Gui.h"
#include "Shaders.h"
#include "Mesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"




int main(){
  CW::Renderer::Renderer window;
  window.setWindowTitle("Mesh Creation and Loading");

  CW::Renderer::DrawShader shader(Shader::vertex, Shader::fragment);
  CW::Renderer::Uniform uniform;

  shader.getUniforms().emplace_back(&uniform);

  CW::Renderer::Mesh square;
  square.addVertices(Mesh::vertices, 4);
  square.addIndicies(Mesh::indicies);
  square.addColors(Mesh::colors, 3);

  float time = 0.0f;

  while(!window.getWindowData()->should_close){
    window.beginFrame();

    glm::mat4 transformation = glm::mat4(1.0);
    transformation = glm::eulerAngleXYZ(10.0f, 20.0f, 0.0f);
    transformation = glm::scale(transformation, glm::vec3(0.2f));
    uniform["transformation"]->set<glm::mat4>(transformation);

    shader.bind();
    square.render();
    shader.unbind();

    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
}