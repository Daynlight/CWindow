#include "Gui.h"

Gui::Gui::Gui(OpenGLRenderer::Renderer *renderer, std::vector<glm::vec2>* data)
: renderer(renderer), data(data) { 
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  
  ImGuiIO& io = ImGui::GetIO();  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = "window.ini";
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(renderer->getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 430");
};

Gui::Gui::~Gui(){
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Gui::Gui::render(){
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
  ImGui::Begin("Window DockSpace", nullptr, window_flags);
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();
  ImGuiID docspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(docspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
  
  
  renderSettings();

  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::Gui::renderSettings()
{
  ImGui::Begin("Settings", nullptr);

  if(ImGui::InputFloat2("Z_0", new_z_0, "%.3f")) update = true;
  if(ImGui::SliderFloat2("Z_0 Sidler", new_z_0, -3, 3, "%.3f")) update = true;
  if(ImGui::InputFloat3("colors", new_colors, "%.3f")) update = true;
  if(ImGui::ColorPicker3("colors", new_colors)){
    for(int i = 0; i < 3; i++){
      if(new_colors[i] >1) new_colors[i] /= 255;
      new_colors[i] *= 255;
    }
    
    update = true;
  }
  if(ImGui::InputInt("MaxIter", &new_max_iter)) update = true;

  if(update){
    (*data)[0] = static_cast<glm::vec2>(new_z_0[0], new_z_0[1]);
    (*data)[1] = static_cast<glm::vec2>(static_cast<float>(new_max_iter), new_colors[0]);
    (*data)[2] = static_cast<glm::vec2>(new_colors[1], new_colors[2]);

    renderer->runComputeShader(*data);
    update = false;
  }

  ImGui::End();
}
