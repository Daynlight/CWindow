#include "Renderer.h"
#include "Shaders.h"
#include "Mesh.h"
#include "FreeCamera/FreeCamera3D.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"



int main(){
  CW::Renderer::Renderer window;
  window.setWindowTitle("Mesh Creation and Loading");
  CW::Renderer::FreeCamera3D camera(&window);
  window.setCursorVisibility(false);



  CW::Renderer::DrawShader shader(Shader::vertex, Shader::fragment);
  CW::Renderer::Uniform uniform;

  shader.getUniforms().emplace_back(&uniform);

  CW::Renderer::Mesh square;
  square.addVertices(Mesh::vertices, 4, 0);
  square.addIndices(Mesh::indicies);
  square.setData<GLfloat>(Mesh::colors, 3, 1, GL_FLOAT);

  float time = 0.0f;
  float cursor_visible_lock = 0.0f;
  bool cursor_lock = true;

  while(!window.getWindowData()->should_close){
    window.beginFrame();

    time += window.getWindowData()->delta_time;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
    model = model * glm::eulerAngleXYZ(0.0f, time, 0.0f);
    model = glm::scale(model, glm::vec3(0.1f));
    glm::mat4 mvp = camera.transformation(&window) * model;
    uniform["transformation"]->set<glm::mat4>(mvp);


    if(cursor_lock) window.setCursorOn(true);
    else window.setCursorOn(false);

    if(window.getInputData()->is_key_down("ESC") && cursor_visible_lock <= 0.0f) {
      cursor_lock = !cursor_lock;
      cursor_visible_lock = 0.5f;
      camera.resetMouse();
    }
    else if(cursor_visible_lock > 0.0f) cursor_visible_lock -= window.getWindowData()->delta_time;

    if(!cursor_lock) camera.event(&window);


    shader.bind();
    square.render();
    shader.unbind();

    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
}