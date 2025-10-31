#include "Renderer.h"

void dataWindowShouldClose(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->should_close == 0) (*passed)++;
  else printf("Fail on: window.getWindowData()->should_close == 0\n");
  window.close();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->should_close == 1) (*passed)++;
  else printf("Fail on: window.getWindowData()->should_close == 1\n");
};

void dataWindowVsync(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->vsync == 0) (*passed)++;
  else printf("Fail on: window.getWindowData()->vsync == 0\n");

  window.setVsync(1);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->vsync == 1) (*passed)++;
  else printf("Fail on: window.getWindowData()->vsync == 60\n");
};

void dataWindowModes(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->window_mode == CW::Renderer::WINDOW) (*passed)++;
  else printf("window.getWindowData()->window_mode == CW::Renderer::WINDOW\n");

  window.setWindowMode(CW::Renderer::BORDERLESS);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->window_mode == CW::Renderer::BORDERLESS) (*passed)++;
  else printf("window.getWindowData()->window_mode == CW::Renderer::BORDERLESS\n");

  window.setWindowMode(CW::Renderer::FULLSCREEN);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->window_mode == CW::Renderer::FULLSCREEN) (*passed)++;
  else printf("window.getWindowData()->window_mode == CW::Renderer::FULLSCREEN\n");
};

void dataWindowTitleTest(int *total, int *passed){
  std::string title = "Data Example 123 \b \n adas";
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.setWindowTitle(title);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->title == title) (*passed)++;
  else printf("Fail on: window.getWindowData()->title == title\n");
};







void data_run_all(){
  int total = 0;
  int passed = 0;

  printf("/////// data tests: /////// \n");
  dataWindowTitleTest(&total, &passed);
  dataWindowShouldClose(&total, &passed);
  dataWindowVsync(&total, &passed);
  dataWindowModes(&total, &passed);

  printf("%d/%d passed\n", passed, total);
}