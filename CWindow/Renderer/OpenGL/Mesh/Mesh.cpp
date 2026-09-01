// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "Mesh.h"



// =============================
// ======= Constructors ========
// =============================
// core
CW::Renderer::Mesh::Mesh() noexcept {};



CW::Renderer::Mesh::Mesh(const CW::Renderer::Shared::MeshData& mesh_data) noexcept 
  : mesh_data(mesh_data) {};



CW::Renderer::Mesh::~Mesh() noexcept {
 destroy();
};



// copy
CW::Renderer::Mesh::Mesh(const Mesh& second) noexcept
  : mesh_data(second.mesh_data) {};



CW::Renderer::Mesh& CW::Renderer::Mesh::operator=(const Mesh& second) noexcept {
  if (this == &second) return *this; 
  
  this->destroy(); 

  this->VAO = 0;
  this->VBO = 0;
  this->EBO = 0;
  this->is_compiled = false;
  mesh_data = second.mesh_data;

  return *this;
};



// move
CW::Renderer::Mesh::Mesh(Mesh&& second) noexcept 
  : VAO(std::move(second.VAO)), VBO(std::move(second.VBO)), 
    EBO(std::move(second.EBO)), is_compiled(std::move(second.is_compiled)),
    mesh_data(std::move(second.mesh_data)), mesh_data_version(std::move(second.mesh_data_version)) {
  second.VAO = 0;
  second.VBO = 0;
  second.EBO = 0;
  second.is_compiled = false;
  second.mesh_data_version = -1;
};



CW::Renderer::Mesh& CW::Renderer::Mesh::operator=(Mesh&& second) noexcept {
  if (this == &second) return *this;
    
  this->destroy(); 

  this->VAO = std::move(second.VAO);
  this->VBO = std::move(second.VBO);
  this->EBO = std::move(second.EBO);
  this->is_compiled = std::move(second.is_compiled);
  this->mesh_data = std::move(second.mesh_data);
  this->mesh_data_version = std::move(second.mesh_data_version);

  second.VAO = 0;
  second.VBO = 0;
  second.EBO = 0;
  second.is_compiled = false;
  second.mesh_data_version = -1;

  return *this;
};



// =============================
// ======= Data Control ========
// =============================
void CW::Renderer::Mesh::setMeshData(const CW::Renderer::Shared::MeshData& mesh_data) noexcept {
  this->mesh_data = mesh_data;
  is_compiled = false;
};



const CW::Renderer::Shared::MeshData& CW::Renderer::Mesh::getMeshData() const noexcept{
  return mesh_data;
};



void CW::Renderer::Mesh::compile() noexcept {
  if(is_compiled && mesh_data.getVersion() == mesh_data_version) return;
  if(VAO || VBO || EBO) destroy();

  try{
    mesh_data.generateBuffer();
  }
  catch(const std::runtime_error& e){
    printf("CW::Renderer::Mesh::compile() noexcept -> %s\n", e.what());
    return;
  };

  const std::vector<unsigned int>& keys = mesh_data.getDataRegisterLayouts();
  size_t line_size = mesh_data.getLineSize();

  genBuffers(mesh_data.getDataBuffer());
  setDataPositions(keys, line_size);
  closeBuffers();

  mesh_data_version = mesh_data.getVersion();
  is_compiled = true;
};



void CW::Renderer::Mesh::destroy() noexcept {
  if (EBO) glDeleteBuffers(1, &EBO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteVertexArrays(1, &VAO);
  VAO = 0;
  VBO = 0;
  EBO = 0;

  is_compiled = false;
  mesh_data_version = -1;
};



void CW::Renderer::Mesh::render(CW::Renderer::Shared::RenderType type) noexcept {
  if(!is_compiled || mesh_data.getVersion() != mesh_data_version) 
    compile();

  if(VAO != 0){
    glBindVertexArray(VAO);
    glDrawElements(RenderTypeToOpenGL(type), mesh_data.getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  };
};



// =============================
// ========== Helpers ==========
// =============================
void CW::Renderer::Mesh::genBuffers(const std::vector<char>& bufferData) noexcept {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(char), bufferData.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_data.getIndices().size() * sizeof(GLuint), mesh_data.getIndices().data(), GL_STATIC_DRAW);
};



void CW::Renderer::Mesh::closeBuffers() const noexcept {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};



void CW::Renderer::Mesh::setDataPositions(const std::vector<unsigned int>& keys, const unsigned int line_size) const noexcept {
  unsigned int offset = 0;

  const std::unordered_map<unsigned int, CW::Renderer::Shared::MeshDataRecord>& reg = mesh_data.getDataRegister();
  for(unsigned int k = 0; k < keys.size(); k++){
    const CW::Renderer::Shared::MeshDataRecord& data = reg.at(keys[k]);
    glVertexAttribPointer(keys[k], data.getDimension(), CW::Renderer::TypeToOpenGL(data.getType()), GL_FALSE, line_size, (GLvoid*)(offset * sizeof(char)));
    glEnableVertexAttribArray(keys[k]);
    offset += data.getDimension() * data.getSizeOfElement();
  };
};
