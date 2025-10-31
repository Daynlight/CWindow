#include "Gui.h"

void CW::Gui::Gui::setDefaultDockingWorkspace() noexcept {
  workspace = [](std::function<void()> render_windows){
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

  render_windows();

  ImGui::End();
  };
};

CW::Gui::Gui::Gui(Renderer::iRenderer *renderer, std::function<void(ImGuiIO &io)> style)
  : renderer(renderer) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  
  ImGuiIO& io = ImGui::GetIO();  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = "window.ini";
  ImGui::StyleColorsDark();

  style(io);

  ImGui_ImplGlfw_InitForOpenGL(renderer->getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 430");

  setDefaultDockingWorkspace();
};

CW::Gui::Gui::~Gui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void CW::Gui::Gui::render() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  workspace([this](){
    for(auto& window : windows){
      ImGui::Begin(window.first.c_str(), nullptr);
      window.second.onRender(renderer);
      ImGui::End();
    };
  });
  
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CW::Gui::Gui::setWorkspace(std::function<void(std::function<void()> render_windows)> new_workspace) noexcept {
  workspace = new_workspace;
};

void CW::Gui::Gui::addWindow(const std::string& name, const CW::Gui::GuiWindow& window) noexcept {
  windows[name] = window;
};

void CW::Gui::Gui::deleteWindow(const std::string& name) noexcept {
  windows.erase(name);
};