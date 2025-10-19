#include "Uniform.h"

CW::Renderer::Uniform::Uniform(std::unordered_map<std::string, UniformData>* data){
  if(data)
    this->data = *data; 
};

CW::Renderer::Uniform::~Uniform() {
  destroy();
};

void CW::Renderer::Uniform::compile() {
  if(UBO) destroy();

  glGenBuffers(1, &UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, UBO);

  unsigned int bufferSize = 0;
  unsigned int offset = 0;

  for (const auto& el : data) {
      bufferSize += el.second.size;
  }

  glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);

  for (auto& el : data) {
      glBufferSubData(GL_UNIFORM_BUFFER, offset, el.second.size, &el.second.value);
      offset += el.second.size;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CW::Renderer::Uniform::destroy() {
  if (UBO != 0) {
    glDeleteBuffers(1, &UBO);
    UBO = 0;
  }
}

bool CW::Renderer::Uniform::contains(const std::string &name){
  return data.contains(name);
}

void CW::Renderer::Uniform::bind(GLuint& shader) const{
  for (auto& el : data) {
    GLuint loc = glGetUniformLocation(shader, el.first.c_str());

    if (el.second.type == &typeid(glm::vec2)) {
      glm::vec2 data = std::get<glm::vec2>(el.second.value);
      glUniform2fv(loc, 1, &data[0]);
    }
    else if (el.second.type == &typeid(glm::vec3)) {
      glm::vec3 data = std::get<glm::vec3>(el.second.value);
      glUniform3fv(loc, 1, &data[0]);
    }
    else if(el.second.type == &typeid(float)){
      float data = std::get<float>(el.second.value);
      glUniform1fv(loc, 1, &data); 
    }
    else if(el.second.type == &typeid(int)){
      int data = std::get<int>(el.second.value);
      glUniform1iv(loc, 1, &data); 
    };
  };
};

GLuint CW::Renderer::Uniform::getUBO() const {
  return UBO;
}

CW::Renderer::UniformData *CW::Renderer::Uniform::operator[](const std::string &name){
  if(!contains(name))
    data[name] = CW::Renderer::UniformData();

  return &(data[name]);
}
