#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <unordered_map>
#include <string>
#include <variant>
#include <typeinfo>

namespace CW::Renderer{
using DataVariants = std::variant<int, float, glm::vec2, glm::vec3>;
struct UniformData{
  DataVariants value = 0.0f;
  const std::type_info* type = &typeid(float);
  unsigned int size = sizeof(float);

  UniformData() {};

  template<typename T>
  void set(T value) {
    this->value = value;
    this->size = sizeof(T); 
    this->type = &typeid(T);
  };

  template<typename T>
  T get(){
    return std::get<T>(value);
  };
};

class Uniform{
private:
  GLuint UBO;
  std::unordered_map<std::string, UniformData> data;

public:
  Uniform(std::unordered_map<std::string, UniformData>* data = nullptr);
  ~Uniform();
  void compile();
  void destroy();
  bool contains(const std::string& name);
  void bind(GLuint& shader) const;
  GLuint getUBO() const;
  UniformData* operator[](const std::string& name);
};
};
