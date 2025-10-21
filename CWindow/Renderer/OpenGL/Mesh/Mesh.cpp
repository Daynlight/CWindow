#include "Mesh.h"

CW::Renderer::Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices) 
  :vertices(vertices), indices(indices), is_compiled(false) {};

CW::Renderer::Mesh::~Mesh() {
 destroy();
};

void CW::Renderer::Mesh::render() {
  if(!is_compiled)
    compile();

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
};

void CW::Renderer::Mesh::compile() {
  if(VAO) 
    destroy();

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  is_compiled = true;
};

void CW::Renderer::Mesh::destroy() {
  if (EBO) glDeleteBuffers(1, &EBO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteBuffers(1, &VAO);

  is_compiled = false;
};
