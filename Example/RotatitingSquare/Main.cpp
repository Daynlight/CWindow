#include "Renderer.h"
#include "Gui.h"
#include "Shaders.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"



int main(){
  CW::Renderer::Renderer window;
  window.setWindowTitle("Rotating Square");

  CW::Renderer::DrawShader shader(Shader::vertex, Shader::fragment);
  CW::Renderer::Uniform uniform;

  shader.getUniforms().emplace_back(&uniform);

  
  CW::Renderer::Mesh square;

  std::vector<GLfloat> vertices({
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f,
    1.0f,  1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, 1.0f,
    1.0f,  1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
  });
  square.addVertices(vertices);

  std::vector<GLuint> indicies({
    0, 1, 2,
    1, 3, 2,
    4, 5, 6,
    5, 7, 6,
    0, 1, 4,
    1, 5, 4,
    2, 3, 6,
    3, 7, 6,
    0, 2, 4,
    2, 6, 4,
    1, 3, 5,
    3, 7, 5
  });
  square.addIndices(indicies);

  std::vector<float> colors = 
  {
    0.1f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f,  1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    0.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
  };

  square.setData<GLfloat>(colors, 3, 1, GL_FLOAT);

  float time = 0.0f;

  while(!window.getWindowData()->should_close){
    window.beginFrame();

    time += window.getWindowData()->delta_time;
    glm::mat4 transformation;
    transformation = glm::eulerAngleXYZ(10.0f, time, 0.0f);
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