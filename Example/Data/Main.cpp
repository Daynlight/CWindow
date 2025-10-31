#include "Renderer.h"
#include "Gui.h"

inline std::function<void(CW::Renderer::iRenderer *window)> dataWindow(){
return [](CW::Renderer::iRenderer *window){

  ImGui::SeparatorText("Info");
  ImGui::Text("fps, %f", window->getWindowData()->delta_time != 0 ? floor(1 / window->getWindowData()->delta_time) : 0.0f);



  ImGui::SeparatorText("Get Data");

  ImGui::Text("should_close, %d", window->getWindowData()->should_close);
  ImGui::Text("is vsync, %d", window->getWindowData()->vsync);
  if(window->getWindowData()->window_mode == CW::Renderer::WINDOW) ImGui::Text("window_mode: Window");
  if(window->getWindowData()->window_mode == CW::Renderer::BORDERLESS) ImGui::Text("window_mode: Borderless");
  if(window->getWindowData()->window_mode == CW::Renderer::FULLSCREEN) ImGui::Text("window_mode: Fullscreen");
  ImGui::Text("window pos, [%d, %d]", window->getWindowData()->x, window->getWindowData()->y);
  ImGui::Text("window size, [%d, %d]", window->getWindowData()->width, window->getWindowData()->height);
  ImGui::Text("title, %s", window->getWindowData()->title.c_str());
  ImGui::Text("icon path, %s", window->getWindowData()->icon.c_str());
  ImGui::Text("is maximized, %d", window->getWindowData()->is_maximize);
  ImGui::Text("is minimized, %d", window->getWindowData()->is_minimize);
  ImGui::Text("is focused, %d", window->getWindowData()->is_focused);
  ImGui::Text("is cursor visible, %d", window->getWindowData()->is_cursor_visible);
  ImGui::Text("is cursor on, %d", window->getWindowData()->is_cursor_on);
  ImGui::Text("delta time, %f", window->getWindowData()->delta_time);



  ImGui::SeparatorText("SetData");

  if(ImGui::Button("Should close")) window->close();
  if(ImGui::Button("vsync")) window->setVsync(!window->getWindowData()->vsync);

  glm::ivec2 pos = {window->getWindowData()->x, window->getWindowData()->y};
  if(ImGui::InputInt2("window pos: ", &pos[0])) window->setPosition(pos.x, pos.y);
  
  glm::ivec2 size = {window->getWindowData()->width, window->getWindowData()->height};
  if(ImGui::SliderInt2("window size: ", &size[0], 10, 1920)) window->setSize(size.x, size.y);

  std::string title = window->getWindowData()->title;
  title.resize(255, '\0');
  if(ImGui::InputText("Title: ", &title[0], title.size())) window->setWindowTitle(title.c_str());

  std::string icon = window->getWindowData()->icon;
  icon.resize(255, '\0');
  if(ImGui::InputText("Icon: ", &icon[0], icon.size())) window->setIcon(icon);

  
  if(ImGui::Button("Window mode")) window->setWindowMode(CW::Renderer::WINDOW);
  if(ImGui::Button("Borderless mode")) window->setWindowMode(CW::Renderer::BORDERLESS);
  if(ImGui::Button("Fullscreen mode")) window->setWindowMode(CW::Renderer::FULLSCREEN);
  if(ImGui::Button("Maximize")) window->maximize(!window->getWindowData()->is_maximize);
  if(ImGui::Button("Minimize")) window->minimize(!window->getWindowData()->is_minimize);
  

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  if(ImGui::Button("Cursor Visibility")) window->setCursorVisibility(!window->getWindowData()->is_cursor_visible);
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  if(ImGui::Button("Cursor On")) window->setCursorOn(!window->getWindowData()->is_cursor_on);
};
};





int main(){
  CW::Renderer::Renderer window;
  window.setWindowTitle("Data Example");

  CW::Gui::Gui gui = CW::Gui::Gui(&window);
  gui.addWindow("Data Window", dataWindow());

  while(!window.getWindowData()->should_close){
    window.beginFrame();

    if(window.getInputData()->is_key_down('q')) window.setCursorVisibility(!window.getWindowData()->is_cursor_visible);
    if(window.getInputData()->is_key_down('e')) window.setCursorOn(!window.getWindowData()->is_cursor_on);

    gui.render();
    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
};