#include "Renderer.h"
#include <unistd.h>


void dataWindowShouldCloseTests(int *total, int *passed){
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

void dataWindowVsyncTests(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->vsync == 0) (*passed)++;
  else printf("Fail on: window.getWindowData()->vsync == 0\n");

  window.setVsync(1);
  sleep(1);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->vsync == 1) (*passed)++;
  else printf("Fail on: window.getWindowData()->vsync == 1\n");
};

void dataWindowModesTests(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->window_mode == CW::Renderer::WINDOW) (*passed)++;
  else printf("window.getWindowData()->window_mode == CW::Renderer::WINDOW\n");

  window.setWindowMode(CW::Renderer::BORDERLESS);
  sleep(1);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->window_mode == CW::Renderer::BORDERLESS) (*passed)++;
  else printf("window.getWindowData()->window_mode == CW::Renderer::BORDERLESS\n");

  window.setWindowMode(CW::Renderer::FULLSCREEN);
  sleep(1);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->window_mode == CW::Renderer::FULLSCREEN) (*passed)++;
  else printf("window.getWindowData()->window_mode == CW::Renderer::FULLSCREEN\n");
};

void dataWindowTitleTests(int *total, int *passed){
  std::string title = "Data Example 123 \b \n adas";
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.setWindowTitle(title);
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->title == title) (*passed)++;
  else printf("Fail on: window.getWindowData()->title == title\n");
};

void dataWindowMinimizedTests(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->is_minimize == 0) (*passed)++;
  else printf("Fail on: window.getWindowData()->is_minimize == 0\n");

  window.minimize(1);
  sleep(1);
  window.windowEvents();

  (*total)++;
  if(window.getWindowData()->is_minimize == 1) (*passed)++;
  else printf("Fail on: window.getWindowData()->is_minimize == 1\n");
};

void dataWindowMaximizedTests(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->is_maximize == 0) (*passed)++;
  else printf("Fail on: window.getWindowData()->is_maximize == 0\n");

  window.maximize(1);
  sleep(1);
  window.windowEvents();

  (*total)++;
  if(window.getWindowData()->is_maximize == 1) (*passed)++;
  else printf("Fail on: window.getWindowData()->is_maximize == 1\n");
};

void dataWindowFocusTests(int *total, int *passed){
  CW::Renderer::Renderer window = CW::Renderer::Renderer();
  window.windowEvents();
  
  (*total)++;
  if(window.getWindowData()->is_focused == 1) (*passed)++;
  else printf("Fail on: window.getWindowData()->is_focused == 1\n");

  window.minimize(1);
  sleep(1);
  window.windowEvents();  

  (*total)++;
  if(window.getWindowData()->is_focused == 0) (*passed)++;
  else printf("Fail on: window.getWindowData()->is_focused == 0\n");
};



void data_run_all(){
  int total = 0;
  int passed = 0;

  printf("/////// data tests: /////// \n");
  dataWindowTitleTests(&total, &passed);
  dataWindowShouldCloseTests(&total, &passed);
  dataWindowVsyncTests(&total, &passed);
  dataWindowModesTests(&total, &passed);
  dataWindowMinimizedTests(&total, &passed);
  dataWindowMaximizedTests(&total, &passed);
  dataWindowFocusTests(&total, &passed);

  printf("%d/%d passed\n", passed, total);
}