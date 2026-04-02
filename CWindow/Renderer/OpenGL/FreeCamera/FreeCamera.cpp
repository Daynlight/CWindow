#include "FreeCamera.h"

CW::Renderer::FreeCamera::FreeCamera(CW::Renderer::Renderer* renderer, glm::vec3 position, glm::vec3 direction)
  :position(position), direction(direction){
    lastMouseX = renderer->getInputData()->mouse_x;
    lastMouseY = renderer->getInputData()->mouse_y;
  };

glm::mat4 CW::Renderer::FreeCamera::transformation(CW::Renderer::Renderer* renderer){
  glm::mat4 view = glm::lookAt(position, position + direction, glm::vec3(0,1,0));
  glm::mat4 projection = glm::perspective(glm::radians(80.0f), renderer->getWindowData()->width / (float)renderer->getWindowData()->height, 0.1f, 100.0f);
  glm::mat4 mvp = projection * view;
  
  return mvp;
};

void CW::Renderer::FreeCamera::event(CW::Renderer::Renderer* renderer) {
  float velocity = 1.1f * renderer->getWindowData()->delta_time;
  glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));

  if(renderer->getInputData()->is_key_down("W")) position += direction * velocity;
  if(renderer->getInputData()->is_key_down("S")) position -= direction * velocity;
  if(renderer->getInputData()->is_key_down("A")) position -= right * velocity;
  if(renderer->getInputData()->is_key_down("D")) position += right * velocity;

  float xoffset = renderer->getInputData()->mouse_x - lastMouseX;
  float yoffset = lastMouseY - renderer->getInputData()->mouse_y;

  lastMouseX = renderer->getInputData()->mouse_x;
  lastMouseY = renderer->getInputData()->mouse_y;

  rotate(xoffset, yoffset);
};

void CW::Renderer::FreeCamera::rotate(float xoffset, float yoffset) {
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f) pitch = 89.0f;
  if(pitch < -89.0f) pitch = -89.0f;

  updateDirection();
};


void CW::Renderer::FreeCamera::updateDirection() {
  glm::vec3 dir;
  dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  dir.y = sin(glm::radians(pitch));
  dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction = glm::normalize(dir);
};

void CW::Renderer::FreeCamera::resetMovement(CW::Renderer::Renderer* renderer){
  lastMouseX = renderer->getInputData()->mouse_x;
  lastMouseY = renderer->getInputData()->mouse_y;
};
