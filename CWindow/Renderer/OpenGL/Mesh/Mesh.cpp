#include "Mesh.h"







CW::Renderer::Mesh::Mesh(){};



CW::Renderer::Mesh::~Mesh() {
 destroy();
};



void CW::Renderer::Mesh::addVertices(std::vector<GLfloat> vertices, unsigned int dimension, unsigned int layout){
  culling_box_exists = false;
  setData<GLfloat>(vertices, dimension, layout, GL_FLOAT);
  generateCullingBox(vertices, dimension);
  is_compiled = false;
};



void CW::Renderer::Mesh::addIndicies(std::vector<unsigned int> indices) {
  this->indices = indices;
  is_compiled = false;
};



std::array<std::vector<GLfloat>, 2> CW::Renderer::Mesh::getCullingBox() const {
  return culling_box;
};



void CW::Renderer::Mesh::render(){
  if(!is_compiled) compile();

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
};



void CW::Renderer::Mesh::generateCullingBox(std::vector<GLfloat> data, unsigned int dimension){
  std::vector<GLfloat> vertex_max;
  vertex_max.reserve(dimension);
  std::vector<GLfloat> vertex_min;
  vertex_min.reserve(dimension);
  
  for(int i = 0; i < dimension; i++){
    vertex_max.emplace_back(data[i]);
    vertex_min.emplace_back(data[i]);
  };

  for(int i = 1; i < data.size() / dimension; i++){
    for(int j = 0; j < dimension; j++){
      if(vertex_max[j] < data[dimension * i + j])
        vertex_max[j] = data[dimension * i + j];
      if(vertex_min[j] > data[dimension * i + j])
        vertex_min[j] = data[dimension * i + j];
    };
  };

  culling_box[0] = vertex_min;
  culling_box[1] = vertex_max;
  culling_box_exists = true;
};



std::vector<char> CW::Renderer::Mesh::arangeData(const std::vector<unsigned int>* keys, unsigned int total_size, unsigned int total_points){
  std::vector<char> bufferData;
  
  bufferData.reserve(total_size);

  for(size_t i = 0; i < total_points; ++i)
    for(int k = 0; k < keys->size(); ++k)
      for(size_t j = 0; j < data[(*keys)[k]].getDimension() * data[(*keys)[k]].getSizeOfElement(); ++j)
        bufferData.push_back(data[(*keys)[k]][i * data[(*keys)[k]].getDimension() * data[(*keys)[k]].getSizeOfElement() + j]);
  
  return bufferData;
};



std::vector<unsigned int> CW::Renderer::Mesh::getDataKeys(){
  std::vector<unsigned int> keys;

  keys.reserve(data.size());
  for (const std::pair<const unsigned int, MeshData> &pair : data)
      keys.push_back(pair.first);

  std::sort(keys.begin(), keys.end());

  return keys;
};



void CW::Renderer::Mesh::compile(){
  if (is_compiled) destroy();
  if(indices.size() == 0) return;

  unsigned int line_size = 0;
  unsigned int total_size = 0;  
  unsigned int offset = 0;
  
  for(std::pair<const unsigned int, MeshData> &el : data){
    line_size += el.second.getDimension() * el.second.getSizeOfElement();
    total_size += el.second.getSize();
  };

  unsigned int total_points = total_size / line_size;
  
      
  std::vector<unsigned int> keys = getDataKeys();
  std::vector<char> bufferData = arangeData(&keys, total_size, total_points);

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
