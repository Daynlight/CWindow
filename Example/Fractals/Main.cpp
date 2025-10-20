#include "Renderer.h"
#include "Gui.h"
#include "Shaders.h"


const float scroll_sensitivity = 0.02f; 
const float sensitivity = 20.0f;
const float zoom_speed = 0.005;

glm::vec2 last_world_pos;
glm::vec2 last_mouse_pos;
bool animation = false;
float current_zoom_speed = 0.005;



inline std::function<void(CW::Renderer::iRenderer *window)> renderSettingsWindow(CW::Renderer::Uniform* uniform) {
  return [uniform](CW::Renderer::iRenderer *window){
  glm::vec2 z = (*uniform)["z"]->get<glm::vec2>(); 
  int maxIter = (*uniform)["maxIter"]->get<int>();
  glm::vec3 colors = (*uniform)["colors"]->get<glm::vec3>();
  colors /= 255;

  ImGui::Begin("Settings", nullptr);

  if(window->getWindowData()->delta_time >= 0.0f) 
    ImGui::Text("FPS: %.f", 1.0f / window->getWindowData()->delta_time);

  ImGui::InputFloat2("Z_0", &z[0], "%.3f");
  ImGui::SliderFloat2("Z_0 Sidler", &z[0], -3, 3, "%.3f");

  ImGui::InputFloat3("colors", &colors[0], "%.3f");
  ImGui::ColorPicker3("colors", &colors[0]);

  ImGui::InputInt("MaxIter", &maxIter);
  
  if(ImGui::Button("Animation")) 
    animation = !animation;

  ImGui::End();

  if(animation){
    if((*uniform)["zoom"]->get<float>() < 0.002) 
      current_zoom_speed = -1 * (zoom_speed);

    if((*uniform)["zoom"]->get<float>() > 3)
      current_zoom_speed = (zoom_speed);
     
    (*uniform)["zoom"]->set<float>((*uniform)["zoom"]->get<float>() - (*uniform)["zoom"]->get<float>() * current_zoom_speed);
  }

  (*uniform)["z"]->set<glm::vec2>(z);
  (*uniform)["maxIter"]->set<int>(maxIter);
  (*uniform)["colors"]->set<glm::vec3>(colors * 255.0f);
};
};





int main(){
  // init window and renderer
  CW::Renderer::Renderer window;
  window.setVsync(0);
  window.setWindowTitle("Malgenbrota and Julia");
  
  // create uniform and malgenbrota shader
  CW::Renderer::Uniform uniform;
  CW::Renderer::DrawShader malgenbrot(Mandelbrot::vertex, Mandelbrot::fragment);
  malgenbrot.getUniforms().emplace_back(&uniform);
  
  // uniform default values
  uniform["z"]->set<glm::vec2>({0.394f, 0.355f});
  uniform["maxIter"]->set<int>(500);
  uniform["colors"]->set<glm::vec3>({20.0f, 100.0f, 5.0f});
  uniform["world_pos"]->set<glm::vec2>({20.0f, 0.0f});
  uniform["zoom"]->set<float>(3.0f);
  uniform["window_ratio"]->set<glm::vec2>({
    window.getWindowData()->width,
    window.getWindowData()->height
  });

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

    uniform["window_ratio"]->set<glm::vec2>({
      window.getWindowData()->width,
      window.getWindowData()->height
    });


    if(window.getInputData()->right_mouse_button_is_down){
      uniform["z"]->set<glm::vec2>({
        3 * (window.getWindowData()->width / 2 - window.getInputData()->mouse_x) / window.getWindowData()->width, 
        3 * (window.getWindowData()->height / 2 - window.getInputData()->mouse_y) / window.getWindowData()->height
      });
    }

    if(window.getInputData()->scroll_is_down){
      uniform["world_pos"]->set<glm::vec2>({
        last_world_pos.x - (window.getInputData()->mouse_x - last_mouse_pos.x) * uniform["zoom"]->get<float>(),
        last_world_pos.y + (window.getInputData()->mouse_y - last_mouse_pos.y) * uniform["zoom"]->get<float>()
      });
    }
    else{
      last_world_pos = uniform["world_pos"]->get<glm::vec2>();
      last_mouse_pos = {window.getInputData()->mouse_x, window.getInputData()->mouse_y};
    };


    float zoom = uniform["zoom"]->get<float>();
    zoom += window.getInputData()->scroll_y * scroll_sensitivity * zoom;
    zoom = glm::clamp(zoom, 0.000001f, 10.0f);
    uniform["zoom"]->set<float>(zoom);

    gui.render();
    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
}