#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Renderer/OpenGL/Renderer.h"



namespace CW::Renderer{
class FreeCamera{
private:
  glm::vec3 position = {0, 0, 0};
  glm::vec3 direction = {0, 0, 1};
  float yaw = 0.0f;
  float pitch = 0.0f;
  float sensitivity = 0.1;
  
  float lastMouseX = 0.0f;
  float lastMouseY = 0.0f;

public:
  FreeCamera(CW::Renderer::Renderer* renderer, glm::vec3 position = {0, 0, 0}, glm::vec3 direction = {0, 0, 1});
  glm::mat4 transformation(CW::Renderer::Renderer* renderer);
  
  void rotate(float xoffset, float yoffset);
  void updateDirection();

  void event(CW::Renderer::Renderer* renderer);

};
};
