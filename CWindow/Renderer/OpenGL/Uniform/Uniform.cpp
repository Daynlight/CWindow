// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "Uniform.h"



CW::Renderer::Uniform::Uniform() { };



CW::Renderer::Uniform::~Uniform() {
  destroy();
};



void CW::Renderer::Uniform::compile() {
  if(UBO) 
    destroy();

  glGenBuffers(1, &UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, UBO);

  unsigned int bufferSize = 0;
  unsigned int offset = 0;

  for (const auto& el : uniform_data.getData())
      bufferSize += sizeof(el.second.getType());

  glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);

  for (auto& el : uniform_data.getData()) {
      glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(el.second.getType()), &el.second.getValue());
      offset += sizeof(el.second.getType());
  }

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
};



void CW::Renderer::Uniform::destroy() {
  if (UBO) {
    glDeleteBuffers(1, &UBO);
    UBO = 0;
  };
};



void CW::Renderer::Uniform::bind(GLuint& shader) {
  if(!UBO) 
    compile();
  
  for (const auto& el : uniform_data.getData()) {
    GLuint loc = glGetUniformLocation(shader, el.first.c_str());

    // int
    if(el.second.getType() == &typeid(int)){
      int data = std::get<int>(el.second.getValue());
      glUniform1iv(loc, el.second.getSize(), &data); 
    }
    else if(el.second.getType() == &typeid(glm::ivec2)){
      glm::ivec2 data = std::get<glm::ivec2>(el.second.getValue());
      glUniform2iv(loc, el.second.getSize(), &data[0]); 
    }
    else if(el.second.getType() == &typeid(glm::ivec3)){
      glm::ivec3 data = std::get<glm::ivec3>(el.second.getValue());
      glUniform3iv(loc, el.second.getSize(), &data[0]); 
    }
    else if(el.second.getType() == &typeid(glm::ivec4)){
      glm::ivec4 data = std::get<glm::ivec4>(el.second.getValue());
      glUniform4iv(loc, el.second.getSize(), &data[0]); 
    } 
    
    // float
    else if(el.second.getType() == &typeid(float)){
      float data = std::get<float>(el.second.getValue());
      glUniform1fv(loc, el.second.getSize(), &data); 
    }
    else if (el.second.getType() == &typeid(glm::vec2)) {
      glm::vec2 data = std::get<glm::vec2>(el.second.getValue());
      glUniform2fv(loc, el.second.getSize(), &data[0]);
    }
    else if (el.second.getType() == &typeid(glm::vec3)) {
      glm::vec3 data = std::get<glm::vec3>(el.second.getValue());
      glUniform3fv(loc, el.second.getSize(), &data[0]);
    }
    else if (el.second.getType() == &typeid(glm::vec4)) {
      glm::vec4 data = std::get<glm::vec4>(el.second.getValue());
      glUniform4fv(loc, el.second.getSize(), &data[0]);
    } 
    
    // double
    else if(el.second.getType() == &typeid(double)){
      double data = std::get<double>(el.second.getValue());
      glUniform1dv(loc, el.second.getSize(), &data);
    }
    else if(el.second.getType() == &typeid(glm::dvec2)){
      glm::dvec2 data = std::get<glm::dvec2>(el.second.getValue());
      glUniform2dv(loc, el.second.getSize(), &data[0]);
    }
    else if(el.second.getType() == &typeid(glm::dvec3)){
      glm::dvec3 data = std::get<glm::dvec3>(el.second.getValue());
      glUniform3dv(loc, el.second.getSize(), &data[0]);
    }
    else if(el.second.getType() == &typeid(glm::dvec4)){
      glm::dvec4 data = std::get<glm::dvec4>(el.second.getValue());
      glUniform4dv(loc, el.second.getSize(), &data[0]);
    } 
    
    //mat
    else if(el.second.getType() == &typeid(glm::mat2)){
      glm::mat2 data = std::get<glm::mat2>(el.second.getValue());
      glUniformMatrix2fv(loc, el.second.getSize(), GL_FALSE, glm::value_ptr(data));
    }
    else if(el.second.getType() == &typeid(glm::mat3)){
      glm::mat3 data = std::get<glm::mat3>(el.second.getValue());
      glUniformMatrix3fv(loc, el.second.getSize(), GL_FALSE, glm::value_ptr(data));
    }
    else if(el.second.getType() == &typeid(glm::mat4)){
      glm::mat4 data = std::get<glm::mat4>(el.second.getValue());
      glUniformMatrix4fv(loc, el.second.getSize(), GL_FALSE, glm::value_ptr(data));
    }
  };
};



void CW::Renderer::Uniform::setUniformData(const CW::Renderer::Shared::UniformData& data) noexcept{
  uniform_data = data;
};



CW::Renderer::Shared::UniformData CW::Renderer::Uniform::getUniformData() noexcept{
  return uniform_data;
};