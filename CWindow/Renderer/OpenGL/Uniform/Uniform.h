#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <unordered_map>
#include <string>
#include <variant>
#include <typeinfo>

namespace CW::Renderer{
using DataVariants = std::variant<int, glm::ivec2, glm::ivec3, glm::ivec4,
                                  float, glm::vec2, glm::vec3, glm::vec4, 
                                  double, glm::dvec2, glm::dvec3, glm::dvec4, 
                                  glm::mat2, glm::mat3, glm::mat4>;

struct UniformData{
  DataVariants value = 0.0f;
  unsigned int size = 1;
  const std::type_info* type = &typeid(float);

  UniformData() {};

  template<typename T>
  void set(T value) {
    this->value = value;
    this->type = &typeid(T);
  };

  template<typename T>
  void set(T value, unsigned int size) {
    this->value = value;
    this->size = size;
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
  void clear();
  void remove(const std::string& name);
  template<typename T>
  void set(const std::string& name, T value);
};

template <typename T>
inline void Uniform::set(const std::string &name, T value){
  this->operator[](name)->set<T>(value);
};
};
