#include "Renderer.h"
#include "Shaders.h"

#include "FreeCamera/FreeCamera3D.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"




int main(){
  CW::Renderer::Renderer window;
  window.setWindowTitle("Mesh Creation and Loading");
  CW::Renderer::FreeCamera3D camera(&window);
  window.setCursorVisibility(false);



  CW::Renderer::Shader shader(Shader::vertex, Shader::fragment);
  CW::Renderer::Uniform uniform;

  shader.getUniforms().emplace_back(&uniform);
  
  CW::MeshLoader data = CW::MeshLoader(); 
  data.LoadModel("../Example/MeshLoader/asset.stl");

  CW::Renderer::Mesh asset;
  asset.addVertices(data.vertices, 4, 0);
  asset.addIndices(data.indices);
  asset.setData<GLfloat>(data.normals, 3, 1, GL_FLOAT);
  // asset.setData<GLfloat>(data.colors, 3, 2, GL_FLOAT);

  data.bindMaterialToUniform(uniform);

  uniform["lightPos"]->set<glm::vec3>({50.0f, 100.0f, 20.0f});
  uniform["lightColor"]->set<glm::vec3>({1.0f, 1.0f, 1.0f});

  float time = 0.0f;
  float cursor_visible_lock = 0.0f;
  bool cursor_lock = true;

  while(!window.getWindowData()->should_close){
    window.beginFrame();

    // time += window.getWindowData()->delta_time;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
    model = model * glm::eulerAngleXYZ(-3.14f / 2.0f, 0.0f, 3.14f);
    model = glm::scale(model, glm::vec3(0.1f));

    glm::mat4 mvp = camera.transformation(&window) * model;

    uniform["transformation"]->set<glm::mat4>(mvp);
    uniform["model"]->set<glm::mat4>(model);


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
    asset.render();
    shader.unbind();

    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
}