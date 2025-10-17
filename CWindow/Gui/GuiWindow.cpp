#include "GuiWindow.h"

CW::Gui::GuiWindow::GuiWindow(std::string name, 
                              std::function<void(CW::Renderer::iRenderer *renderer)> onRender, 
                              std::function<void()> onDestroy)
  :name(name),
   onRender(onRender),
   onDestroy(onDestroy) {};

CW::Gui::GuiWindow::~GuiWindow() {
  onDestroy();
};

bool CW::Gui::GuiWindow::operator==(const CW::Gui::GuiWindow &second) {
  return this->name == second.name;
};