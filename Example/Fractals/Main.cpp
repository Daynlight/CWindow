#include "Renderer.h"
#include "Gui.h"
#include "Shaders.h"



const float scroll_sensitivity = 0.02f; 
const float sensitivity = 20.0f;
const float keyboard_sensitivity = 4.0f;
const float keyboard_scroll_sensitivity = 0.005f;

const float zoom_speed = 0.005f;
const glm::vec2 z_speed = {0.004f, 0.002f};
const float max_z_dot = 1;

glm::vec2 last_world_pos;
glm::vec2 last_mouse_pos;
float current_zoom_speed = zoom_speed;
glm::vec2 current_z_speed = z_speed;

bool zoom_animation = false;
bool z_animation = false;




inline std::function<void(CW::Renderer::iRenderer *window)> renderSettingsWindow(CW::Renderer::Uniform* uniform) {
  return [uniform](CW::Renderer::iRenderer *window){
  glm::vec2 z = (*uniform)["z_0"]->get<glm::vec2>(); 
  int maxIter = (*uniform)["maxIter"]->get<int>();
  glm::vec3 colors = (*uniform)["colors"]->get<glm::vec3>();
  colors /= 255;
  int mode = (*uniform)["mode"]->get<int>();

  ImGui::Begin("Settings", nullptr);

  if(window->getWindowData()->delta_time >= 0.0f) 
  ImGui::Text("FPS: %.f", 1.0f / window->getWindowData()->delta_time);
  



  ImGui::Separator();
  ImGui::Text("Settings");

  ImGui::InputFloat2("Z_0", &z[0], "%.3f");
  ImGui::SliderFloat2("Z_0 Sidler", &z[0], -3, 3, "%.3f");

  ImGui::InputFloat3("colors", &colors[0], "%.3f");
  ImGui::ColorPicker3("colors", &colors[0]);

  ImGui::InputInt("MaxIter", &maxIter);
  



  ImGui::Separator();
  ImGui::Text("Animations");
  
  if(ImGui::Button("Zoom Animation")) 
    zoom_animation = !zoom_animation;

  if(ImGui::Button("Z_0 Animation"))
    z_animation = !z_animation;




  ImGui::Separator();
  ImGui::Text("Presets");
      
  if(ImGui::Button("Malgenbrot"))
    mode = 0;

  if(ImGui::Button("Julia"))
    mode = 1;

  if(ImGui::Button("Julia: 0.35 + 0.35i")){
    mode = 1;
    (*uniform)["zoom"]->set<float>(3.0f);
    (*uniform)["world_post"]->set<glm::vec2>({0.0f, 0.0f});
    z = {0.35f, 0.35f};
  }

  if(ImGui::Button("Julia: 0.4 + 0.4i")){
    mode = 1;
    (*uniform)["zoom"]->set<float>(3.0f);
    (*uniform)["world_post"]->set<glm::vec2>({0.0f, 0.0f});
    z = {0.4f, 0.4f};
  }
  
  if(ImGui::Button("Julia: -0.7269 + 0.1886i")){
    mode = 1;
    (*uniform)["zoom"]->set<float>(3.0f);
    (*uniform)["world_post"]->set<glm::vec2>({0.0f, 0.0f});
    z = {-0.7269f, 0.1889f};
  }

  if(ImGui::Button("Julia: -0.8 + 0.156i")){
    mode = 1;
    (*uniform)["zoom"]->set<float>(3.0f);
    (*uniform)["world_post"]->set<glm::vec2>({0.0f, 0.0f});
    z = {-0.8f, 0.156f};
  }

  if(ImGui::Button("Julia: -0.8i")){
    mode = 1;
    (*uniform)["zoom"]->set<float>(3.0f);
    (*uniform)["world_post"]->set<glm::vec2>({0.0f, 0.0f});
    z = {0.0f, -0.8f};
  }

  ImGui::End();




  if(zoom_animation){
    if((*uniform)["zoom"]->get<float>() < 0.002) 
      current_zoom_speed = -1 * (zoom_speed);

    if((*uniform)["zoom"]->get<float>() > 3)
      current_zoom_speed = (zoom_speed);
     
    (*uniform)["zoom"]->set<float>((*uniform)["zoom"]->get<float>() - (*uniform)["zoom"]->get<float>() * current_zoom_speed);
  }

  if(z_animation){
    if(glm::abs(z.x) * glm::abs(z.y) > max_z_dot) 
      current_z_speed = -1.0f * current_z_speed;
    z += current_z_speed;
  }



  (*uniform)["z_0"]->set<glm::vec2>(z);
  (*uniform)["maxIter"]->set<int>(maxIter);
  (*uniform)["colors"]->set<glm::vec3>(colors * 255.0f);
  (*uniform)["mode"]->set<int>(mode);
};
};




inline void windowMovement(CW::Renderer::iRenderer *window, CW::Renderer::Uniform* uniform){

    (*uniform)["window_ratio"]->set<glm::vec2>({
      window->getWindowData()->width,
      window->getWindowData()->height
    });

    if(window->getInputData()->right_mouse_button_is_down){
      (*uniform)["z_0"]->set<glm::vec2>({
        3 * (window->getWindowData()->width / 2 - window->getInputData()->mouse_x) / window->getWindowData()->width, 
        3 * (window->getWindowData()->height / 2 - window->getInputData()->mouse_y) / window->getWindowData()->height
      });
    }

    if(window->getInputData()->scroll_is_down){
      (*uniform)["world_pos"]->set<glm::vec2>({
        last_world_pos.x - (window->getInputData()->mouse_x - last_mouse_pos.x) * (*uniform)["zoom"]->get<float>(),
        last_world_pos.y + (window->getInputData()->mouse_y - last_mouse_pos.y) * (*uniform)["zoom"]->get<float>()
      });
    }
    else{
      last_world_pos = (*uniform)["world_pos"]->get<glm::vec2>();
      last_mouse_pos = {window->getInputData()->mouse_x, window->getInputData()->mouse_y};
    };

    float zoom = (*uniform)["zoom"]->get<float>();
    zoom += window->getInputData()->scroll_y * scroll_sensitivity * zoom;
    zoom = glm::clamp(zoom, 0.000001f, 10.0f);
    (*uniform)["zoom"]->set<float>(zoom);

    


    (*uniform)["world_pos"]->set<glm::vec2>({
      (*uniform)["world_pos"]->get<glm::vec2>().x + keyboard_sensitivity * (*uniform)["zoom"]->get<float>() * window->getInputData()->is_bind_down("Move Right")
                                                  - keyboard_sensitivity * (*uniform)["zoom"]->get<float>() * window->getInputData()->is_bind_down("Move Left"),
      (*uniform)["world_pos"]->get<glm::vec2>().y + keyboard_sensitivity * (*uniform)["zoom"]->get<float>() * window->getInputData()->is_bind_down("Move Up")
                                                  - keyboard_sensitivity * (*uniform)["zoom"]->get<float>() * window->getInputData()->is_bind_down("Move Down")
    });
  
    (*uniform)["zoom"]->set<float>(
      (*uniform)["zoom"]->get<float>() + keyboard_scroll_sensitivity * window->getInputData()->is_bind_down("Increase Zoom") * (*uniform)["zoom"]->get<float>() 
                                       - keyboard_scroll_sensitivity * window->getInputData()->is_bind_down("Decrease Zoom") * (*uniform)["zoom"]->get<float>()
    );
};







int main(){
  // init window and renderer
  CW::Renderer::Renderer window;
  window.setVsync(0);
  window.setWindowTitle("Malgenbrota and Julia");
  
  // create uniform and malgenbrota shader
  CW::Renderer::Uniform uniform;
  CW::Renderer::DrawShader malgenbrot(Fractal::vertex, Fractal::fragment);
  malgenbrot.getUniforms().emplace_back(&uniform);
  
  // uniform default values
  uniform["z_0"]->set<glm::vec2>({0.394f, 0.355f});
  uniform["mode"]->set<int>(0);
  uniform["maxIter"]->set<int>(300);
  uniform["colors"]->set<glm::vec3>({20.0f, 100.0f, 5.0f});
  uniform["world_pos"]->set<glm::vec2>({-100.0f, 0.0f});
  uniform["zoom"]->set<float>(3.0f);
  uniform["window_ratio"]->set<glm::vec2>({
    window.getWindowData()->width,
    window.getWindowData()->height
  });

  // set binds
  window.setBind("Move Up", 'W');
  window.setBind("Move Down", 'S');
  window.setBind("Move Left", 'A');
  window.setBind("Move Right", 'D');
  window.setBind("Increase Zoom", '-');
  window.setBind("Decrease Zoom", '=');

  // init gui and add Settings Window
  CW::Gui::Gui gui(&window);
  gui.addWindow("Settings", renderSettingsWindow(&uniform));

  // create viewport mesh
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

  
  

  // main loop
  while(window.getWindowData()->should_close){
    window.beginFrame();

    malgenbrot.bind();
    viewport.render();
    malgenbrot.unbind();

    windowMovement(&window, &uniform);

    gui.render();
    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
}