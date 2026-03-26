#include "Mesh.h"

CW::Renderer::Mesh::Mesh(){};

CW::Renderer::Mesh::~Mesh() {
 destroy();
}

void CW::Renderer::Mesh::addVertices(std::vector<GLfloat> vertices, unsigned int dimension, unsigned int layout){
  addData<GLfloat>(vertices, dimension, layout, GL_FLOAT);
  is_compiled = false;
};

void CW::Renderer::Mesh::addIndicies(std::vector<unsigned int> indices) {
  this->indices = indices;
  is_compiled = false;
};

void CW::Renderer::Mesh::render() {
  // [TODO] culling
  if(!is_compiled)
    compile();

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
};

void CW::Renderer::Mesh::compile() {
  if(indices.size() == 0) return;
  if (is_compiled) destroy();

  std::vector<char> bufferData;
  unsigned int line_size = 0;
  unsigned int total_size = 0;  
  unsigned int offset = 0;
  
  for(std::pair<const unsigned int, MeshData> &el : data){
    line_size += el.second.getDimension() * el.second.getSizeOfElement();
    total_size += el.second.getSize();
  };

  unsigned int total_points = total_size / line_size;
  
  bufferData.reserve(total_size);

  std::vector<unsigned int> keys;
  keys.reserve(data.size());
  for (const std::pair<const unsigned int, MeshData> &pair : data)
      keys.push_back(pair.first);

  std::sort(keys.begin(), keys.end());

  for(size_t i = 0; i < total_points; ++i)
    for(int k = 0; k < keys.size(); ++k)
      for(size_t j = 0; j < data[keys[k]].getDimension() * data[keys[k]].getSizeOfElement(); ++j)
        bufferData.push_back(data[keys[k]][i * data[keys[k]].getDimension() * data[keys[k]].getSizeOfElement() + j]);
      

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(char), bufferData.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  for(int k = 0; k < keys.size(); k++){
    glVertexAttribPointer(keys[k], data[keys[k]].getDimension(), data[keys[k]].getType(), GL_FALSE, line_size, (GLvoid*)(offset * sizeof(char)));
    glEnableVertexAttribArray(keys[k]);
    offset += data[keys[k]].getDimension() * data[keys[k]].getSizeOfElement();
  };

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  is_compiled = true;
};

void CW::Renderer::Mesh::destroy() {
  if (EBO) glDeleteBuffers(1, &EBO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteVertexArrays(1, &VAO);
  VAO = 0;
  VBO = 0;
  EBO = 0;

  is_compiled = false;
};
