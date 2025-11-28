#include "Renderer.h"
#include "Gui.h"

#include "Shaders.h"


void buildFilters(CW::Renderer::Uniform *uniform){
  if(ImGui::Button("No Filter"))
    (*uniform)["matrix"]->set<glm::mat3>({0, 0, 0,
                                          0, 1, 0,
                                          0, 0, 0});
  if(ImGui::Button("Vertical High"))
    (*uniform)["matrix"]->set<glm::mat3>({1, 0, -1,
                                          1, 0, -1,
                                          1, 0, -1});
  if(ImGui::Button("Horizontal High"))
    (*uniform)["matrix"]->set<glm::mat3>({1, 1, 1,
                                          0, 0, 0,
                                          -1, -1, -1});
  if(ImGui::Button("Gaussian Blur"))
    (*uniform)["matrix"]->set<glm::mat3>({1.0f/16, 2.0f/16, 1.0f/16,
                                          2.0f/16, 4.0f/16, 2.0f/16,
                                          1.0f/16, 2.0f/16, 1.0f/16});
};

void filterMatrix(CW::Renderer::Uniform *uniform){
  glm::mat3 matrix = (*uniform)["matrix"]->get<glm::mat3>();
  float matrix_part_1[3] = {matrix[0][0], matrix[0][1], matrix[0][2]};
  float matrix_part_2[3] = {matrix[1][0], matrix[1][1], matrix[1][2]};
  float matrix_part_3[3] = {matrix[2][0], matrix[2][1], matrix[2][2]};

  ImGui::InputFloat3("1", matrix_part_1);
  ImGui::InputFloat3("2", matrix_part_2);
  ImGui::InputFloat3("3", matrix_part_3);

  glm::mat3 changed_matrix = {
    matrix_part_1[0], matrix_part_1[1], matrix_part_1[2],
    matrix_part_2[0], matrix_part_2[1], matrix_part_2[2],
    matrix_part_3[0], matrix_part_3[1], matrix_part_3[2],
  };

  if(ImGui::Button("Normalize")){
    float sum = 0.0f;

    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        sum += changed_matrix[i][j];
      };
    };

    if(sum != 0){
      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          changed_matrix[i][j] /= sum;
        };
      };
    };
  };

  (*uniform)["matrix"]->set<glm::mat3>(changed_matrix);
};


inline std::function<void(CW::Renderer::iRenderer *window)> settings(CW::Renderer::Uniform *uniform){
return [uniform](CW::Renderer::iRenderer *window){
  ImGui::Text("Filter Settings");

  glm::ivec2 radius = (*uniform)["radius"]->get<glm::ivec2>();
  int radius_int[2] = {radius[0], radius[1]};
  ImGui::InputInt2("radius", radius_int);
  radius = {radius_int[0], radius_int[1]};
  (*uniform)["radius"]->set<glm::ivec2>(radius);

  ImGui::Separator();
  filterMatrix(uniform);

  ImGui::Separator();
  buildFilters(uniform);
};
};


int main(){
  CW::Renderer::Renderer renderer;
  CW::Gui::Gui gui(&renderer);

  CW::Renderer::Texture texture;
  if(texture.load("../Assets/image2.png"))
    return -1;

  CW::Renderer::Mesh viewport = CW::Renderer::Mesh(
  {
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
  }, 
  {
    0, 1, 2,
    1, 3, 2
  });

  viewport.addTextCords(
  {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  });

  CW::Renderer::DrawShader texture_shader(Texture::vertex, Texture::fragment);
  CW::Renderer::Uniform uniform;
  texture_shader.getUniforms().emplace_back(&uniform);
  uniform["uTexture"]->set<int>(0);
  uniform["radius"]->set<glm::ivec2>({200, 200});
  uniform["matrix"]->set<glm::mat3>({0, 0, 0,
                                     0, 1, 0,
                                     0, 0, 0});



  gui.addWindow("Settings", settings(&uniform));


  while (!renderer.getWindowData()->should_close){
    renderer.beginFrame();
    
    texture.bind();
    texture_shader.bind();
    viewport.render();
    texture_shader.unbind();
    texture.unbind();
    
    gui.render();

    renderer.swapBuffer();
    renderer.windowEvents();
  };
  

  return 0;
};