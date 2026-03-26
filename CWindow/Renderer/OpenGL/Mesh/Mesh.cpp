#include "Mesh.h"

CW::Renderer::Mesh::Mesh(){};

CW::Renderer::Mesh::~Mesh() {
 destroy();
}

void CW::Renderer::Mesh::addVertices(std::vector<float> vertices, unsigned int dimension, unsigned int layout){
  this->vertices = vertices;
  this->vertices_dimension = dimension;
  this->vertices_id = layout;
  is_compiled = false;
};

void CW::Renderer::Mesh::addIndicies(std::vector<unsigned int> indices) {
  this->indices = indices;
  is_compiled = false;
};

void CW::Renderer::Mesh::addNormals(std::vector<GLfloat> normals, unsigned int dimension, unsigned int layout) {
  this->normals = normals;
  this->normals_dimension = dimension;
  this->normals_id = layout;
  is_compiled = false;
};

void CW::Renderer::Mesh::addColors(std::vector<GLfloat> colors, unsigned int dimension, unsigned int layout){
  this->colors = colors;
  this->color_dimension = dimension;
  this->color_id = layout;
  is_compiled = false;
};

void CW::Renderer::Mesh::addTextCords(std::vector<GLfloat> textCords, unsigned int dimension, unsigned int layout) {
  this->textCords = textCords;
  this->textCords_dimension = dimension;
  this->textCords_id = layout;
  is_compiled = false;
};

void CW::Renderer::Mesh::addTextID(std::vector<GLuint> textID, unsigned int dimension, unsigned int layout) {
  this->textID = textID;
  this->textID_dimension = dimension;
  this->textID_id = layout;
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
  if(vertices.size() == 0) return;
  if(indices.size() == 0) return;
  if(vertices_dimension == 0) return;
  // [TODO] check if id's are unique

  if (is_compiled) destroy();
  
  std::vector<GLfloat> bufferData;
  unsigned int line_size = vertices_dimension + color_dimension + textCords_dimension;

  bufferData.reserve(vertices.size() + colors.size() + textCords.size());
  
  for (size_t i = 0; i < vertices.size() / 4; ++i) {
    for(size_t j = 0; j < vertices_dimension; ++j)
      bufferData.push_back(vertices[i * vertices_dimension + j]);
    
    for(size_t j = 0; j < normals_dimension; ++j)
      bufferData.push_back(normals[i * normals_dimension + j]);

    for(size_t j = 0; j < color_dimension; ++j)
    bufferData.push_back(colors[i * color_dimension + j]);
    
    for(size_t j = 0; j < textCords_dimension; ++j)
      bufferData.push_back(textCords[i * textCords_dimension + j]);
    
    for(size_t j = 0; j < textID_dimension; ++j)
      bufferData.push_back(textID[i * textID_dimension + j]);
  };


  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(GLfloat), bufferData.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(vertices_id, vertices_dimension, GL_FLOAT, GL_FALSE, line_size * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(vertices_id);

  unsigned int offset = 3;

  // Normals
  if (normals_id) {
    glVertexAttribPointer(normals_id, normals_dimension, GL_FLOAT, GL_FALSE, line_size * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(normals_id);  
    offset += normals_dimension;
  };

  // Colors
  if (color_id) {
    glVertexAttribPointer(color_id, color_dimension, GL_FLOAT, GL_FALSE, line_size * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(color_id);  
    offset += color_dimension;
  };

  // Texture coordinates
  if (textCords_id) {
    glVertexAttribPointer(textCords_id, textCords_dimension, GL_FLOAT, GL_FALSE, line_size * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(textCords_id);  
    offset += textCords_dimension;
  };

  // Texture ID
  if (textID_id) {
    glVertexAttribPointer(textID_id, textID_dimension, GL_FLOAT, GL_FALSE, line_size * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(textID_id);  
    offset += textID_dimension;
  };

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  is_compiled = true;
};

void CW::Renderer::Mesh::destroy() {
  if (EBO) glDeleteBuffers(1, &EBO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteBuffers(1, &VAO);

  is_compiled = false;
};
