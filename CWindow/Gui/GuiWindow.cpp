#include "GuiWindow.h"

CW::Gui::GuiWindow::GuiWindow() noexcept {};

CW::Gui::GuiWindow::GuiWindow(std::function<void(CW::Renderer::iRenderer *renderer)> onRender,
                              std::function<void()> onDestroy) noexcept
    : onRender(onRender),
      onDestroy(onDestroy) {};

CW::Gui::GuiWindow::~GuiWindow() noexcept {
  onDestroy();
};