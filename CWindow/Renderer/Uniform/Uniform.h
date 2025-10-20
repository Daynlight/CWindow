#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <unordered_map>
#include <string>
#include <variant>
#include <typeinfo>

namespace CW::Renderer{
using DataVariants = std::variant<int, float, double, glm::vec2, glm::vec3, glm::dvec2, glm::dvec3>;
struct UniformData{
  DataVariants value = 0.0f;
  const std::type_info* type = &typeid(float);

  UniformData() {};

  template<typename T>
  void set(T value) {
    this->value = value;
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
  Uniform();
  ~Uniform();

  void compile();
  void destroy();

  void bind(GLuint& shader) const;
  UniformData* operator[](const std::string& name);
};
};
