#include "Renderer.h"
#include "Gui.h"


int main(){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  


  while(window.getWindowData()->should_close){




    window.windowEvents();
    window.swapBuffer();
  };


  return 0;
};