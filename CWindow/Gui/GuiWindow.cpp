#include "GuiWindow.h"

CW::Gui::GuiWindow::GuiWindow() {};

CW::Gui::GuiWindow::GuiWindow(std::function<void(CW::Renderer::iRenderer *renderer)> onRender,
                              std::function<void()> onDestroy)
    : onRender(onRender),
      onDestroy(onDestroy) {};

CW::Gui::GuiWindow::~GuiWindow() {
  onDestroy();
};