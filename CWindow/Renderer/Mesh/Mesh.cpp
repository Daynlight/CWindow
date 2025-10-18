#include "Mesh.h"

CW::Renderer::Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices) {
  bind(vertices, indices);
}

CW::Renderer::Mesh::~Mesh() {
 unbind();
}

void CW::Renderer::Mesh::render() {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices_amount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void CW::Renderer::Mesh::bind(std::vector<GLfloat> vertices, std::vector<GLuint> indices) {
  if(VAO) unbind();
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  indices_amount = indices.size();
}

void CW::Renderer::Mesh::unbind() {
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteBuffers(1, &VAO);
  if (EBO) glDeleteBuffers(1, &EBO);
};
