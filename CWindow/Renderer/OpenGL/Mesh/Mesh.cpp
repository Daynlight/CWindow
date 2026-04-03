#include "Mesh.h"







CW::Renderer::Mesh::Mesh(){};



CW::Renderer::Mesh::~Mesh() {
 destroy();
};



void CW::Renderer::Mesh::addVertices(const std::vector<GLfloat>& vertices, const unsigned int dimension, const unsigned int layout){
  culling_box_exists = false;

  setData<GLfloat>(vertices, dimension, layout, GL_FLOAT);
  generateCullingBox(vertices, dimension);
  
  is_compiled = false;
};



void CW::Renderer::Mesh::addIndices(const std::vector<unsigned int>& indices) {
  this->indices = indices;

  is_compiled = false;
};



void CW::Renderer::Mesh::removeData(const unsigned int layout){
  this->dataRegister.erase(layout);
};



void CW::Renderer::Mesh::clearData() {
  this->dataRegister.clear();
};



void CW::Renderer::Mesh::generateCullingBox(const std::vector<GLfloat>& data, const unsigned int dimension){
  if(dimension == 0) return;
  
  std::vector<GLfloat> vertex_max;
  vertex_max.reserve(dimension);
  std::vector<GLfloat> vertex_min;
  vertex_min.reserve(dimension);
  
  for(unsigned int i = 0; i < dimension; i++){
    vertex_max.emplace_back(data[i]);
    vertex_min.emplace_back(data[i]);
  };

  for(unsigned int i = 1; i < data.size() / dimension; i++){
    for(unsigned int j = 0; j < dimension; j++){
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



std::array<std::vector<GLfloat>, 2> CW::Renderer::Mesh::getCullingBox() const{
  return culling_box;
};



std::vector<unsigned int> CW::Renderer::Mesh::getDataRegisterLayouts() const {
  std::vector<unsigned int> keys;

  keys.reserve(dataRegister.size());
  for (const std::pair<const unsigned int, MeshData> &pair : dataRegister)
      keys.push_back(pair.first);

  std::sort(keys.begin(), keys.end());

  return keys;
};



std::vector<char> CW::Renderer::Mesh::generateDataBuffer(const std::vector<unsigned int>& keys, const unsigned int total_size, const unsigned int total_points) {
  std::vector<char> bufferData(total_size);

  unsigned int dstOffset = 0;

  for (unsigned int i = 0; i < total_points; ++i) {
    for (unsigned int k = 0; k < keys.size(); ++k) {
      const CW::Renderer::MeshData& md = dataRegister.at(keys[k]);
      const unsigned int elementSize = md.getDimension() * md.getSizeOfElement();
      const unsigned int srcOffset = i * elementSize;

      std::memcpy(bufferData.data() + dstOffset, md.getRawData() + srcOffset, elementSize);

      dstOffset += elementSize;
    };
  };

  return bufferData;
};



void CW::Renderer::Mesh::genBuffers(const std::vector<char>& bufferData) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(char), bufferData.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
};



void CW::Renderer::Mesh::closeBuffers() const {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};



void CW::Renderer::Mesh::setDataPositions(const std::vector<unsigned int>& keys, const unsigned int line_size) {
  unsigned int offset = 0;

  for(unsigned int k = 0; k < keys.size(); k++){
    glVertexAttribPointer(keys[k], dataRegister[keys[k]].getDimension(), dataRegister[keys[k]].getType(), GL_FALSE, line_size, (GLvoid*)(offset * sizeof(char)));
    glEnableVertexAttribArray(keys[k]);
    offset += dataRegister[keys[k]].getDimension() * dataRegister[keys[k]].getSizeOfElement();
  };
};



void CW::Renderer::Mesh::render(){
  if(!is_compiled) compile();

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
};



void CW::Renderer::Mesh::compile(){
  if (is_compiled) destroy();
  if(indices.size() == 0) return;

  unsigned int line_size = 0;
  unsigned int total_size = 0;  
  
  for(const std::pair<const unsigned int, MeshData>& el : dataRegister){
    line_size += el.second.getDimension() * el.second.getSizeOfElement();
    total_size += el.second.getSize();
  };

  if(line_size == 0) return;
  const unsigned int total_points = total_size / line_size;
      
  const std::vector<unsigned int> keys = getDataRegisterLayouts();
  const std::vector<char> bufferData = generateDataBuffer(keys, total_size, total_points);

  genBuffers(bufferData);
  setDataPositions(keys, line_size);
  closeBuffers();

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
