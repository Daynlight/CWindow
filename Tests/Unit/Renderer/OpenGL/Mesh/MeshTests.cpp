// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <vector>
#include <cstring>

#define private public
#define protected public

#include "CWindow/Renderer/OpenGL/Renderer.h"
#include "CWindow/Renderer/OpenGL/Mesh/Mesh.h"

#undef private
#undef protected



CW::Renderer::Renderer renderer;

CW::Renderer::Shared::MeshData createMeshData() {
  CW::Renderer::Shared::MeshData data;

  std::vector<float> vertices = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f
  };

  std::vector<float> colors = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };

  std::vector<unsigned int> indices = {0, 1, 2};

  data.addVertices(vertices, 3, 0);
  data.setData<float>(colors, 3, 1);
  data.addIndices(indices);

  return data;
};



// =============================
// ======= Constructors ========
// =============================
TEST(MeshDefaultConstructor, InitializesAsUncompiled) {
  CW::Renderer::Mesh mesh;

  EXPECT_EQ(mesh.VAO, 0u);
  EXPECT_EQ(mesh.VBO, 0u);
  EXPECT_EQ(mesh.EBO, 0u);
  EXPECT_FALSE(mesh.is_compiled);
};

TEST(MeshConstructor, CopiesMeshData) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);

  EXPECT_EQ(mesh.getMeshData().getIndices(), data.getIndices());
  EXPECT_EQ(mesh.getMeshData().getDataRegister().size(), data.getDataRegister().size());
  EXPECT_FALSE(mesh.is_compiled);
};

TEST(MeshConstructor, PerformsDeepCopyOfMeshData) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);

  const auto& original = data.getDataRegister().at(0);
  const auto& copy = mesh.getMeshData().getDataRegister().at(0);

  EXPECT_NE(original.getRawData(), copy.getRawData());
  EXPECT_EQ(std::memcmp(original.getRawData(), copy.getRawData(), original.getSize()), 0);
};

// copy
TEST(MeshCopyConstructor, DoesNotShareGPUObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh source(data);
  source.compile();

  CW::Renderer::Mesh copy(source);

  EXPECT_FALSE(copy.is_compiled);
  EXPECT_EQ(copy.VAO, 0u);
  EXPECT_EQ(copy.VBO, 0u);
  EXPECT_EQ(copy.EBO, 0u);
};

TEST(MeshCopyConstructor, CreatesIndependentGPUObjectsAfterCompile) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh source(data);
  source.compile();

  CW::Renderer::Mesh copy(source);
  copy.compile();

  EXPECT_NE(copy.VAO, source.VAO);
  EXPECT_NE(copy.VBO, source.VBO);
  EXPECT_NE(copy.EBO, source.EBO);

  EXPECT_EQ(glIsVertexArray(source.VAO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(source.VBO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(source.EBO), GL_TRUE);

  EXPECT_EQ(glIsVertexArray(copy.VAO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(copy.VBO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(copy.EBO), GL_TRUE);
};

TEST(MeshCopyAssignment, DestroysPreviousGPUObjects) {
  CW::Renderer::Shared::MeshData first_data = createMeshData();
  CW::Renderer::Shared::MeshData second_data = createMeshData();

  CW::Renderer::Mesh source(first_data);
  CW::Renderer::Mesh target(second_data);

  source.compile();
  target.compile();

  const GLuint old_vao = target.VAO;
  const GLuint old_vbo = target.VBO;
  const GLuint old_ebo = target.EBO;

  target = source;

  EXPECT_EQ(glIsVertexArray(old_vao), GL_FALSE);
  EXPECT_EQ(glIsBuffer(old_vbo), GL_FALSE);
  EXPECT_EQ(glIsBuffer(old_ebo), GL_FALSE);

  EXPECT_FALSE(target.is_compiled);
  EXPECT_EQ(target.VAO, 0u);
  EXPECT_EQ(target.VBO, 0u);
  EXPECT_EQ(target.EBO, 0u);
};

TEST(MeshCopyAssignment, CopiesCPUDataIndependently) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh source(data);
  CW::Renderer::Mesh target;

  target = source;

  const auto& first = source.mesh_data.dataRegister.at(0);
  const auto& second = target.mesh_data.dataRegister.at(0);

  EXPECT_NE(first.getRawData(), second.getRawData());
  EXPECT_EQ(std::memcmp(first.getRawData(), second.getRawData(), first.getSize()), 0);
};

// move
TEST(MeshMoveConstructor, TransfersGPUObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh source(data);
  source.compile();

  const GLuint vao = source.VAO;
  const GLuint vbo = source.VBO;
  const GLuint ebo = source.EBO;

  CW::Renderer::Mesh moved(std::move(source));

  EXPECT_EQ(moved.VAO, vao);
  EXPECT_EQ(moved.VBO, vbo);
  EXPECT_EQ(moved.EBO, ebo);
  EXPECT_TRUE(moved.is_compiled);

  EXPECT_EQ(source.VAO, 0u);
  EXPECT_EQ(source.VBO, 0u);
  EXPECT_EQ(source.EBO, 0u);
  EXPECT_FALSE(source.is_compiled);

  EXPECT_EQ(glIsVertexArray(vao), GL_TRUE);
  EXPECT_EQ(glIsBuffer(vbo), GL_TRUE);
  EXPECT_EQ(glIsBuffer(ebo), GL_TRUE);
};

TEST(MeshMoveAssignment, DeletesTargetAndTransfersSourceObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh source(data);
  CW::Renderer::Mesh target(data);

  source.compile();
  target.compile();

  const GLuint source_vao = source.VAO;
  const GLuint source_vbo = source.VBO;
  const GLuint source_ebo = source.EBO;

  const GLuint target_vao = target.VAO;
  const GLuint target_vbo = target.VBO;
  const GLuint target_ebo = target.EBO;

  target = std::move(source);

  EXPECT_EQ(glIsVertexArray(target_vao), GL_FALSE);
  EXPECT_EQ(glIsBuffer(target_vbo), GL_FALSE);
  EXPECT_EQ(glIsBuffer(target_ebo), GL_FALSE);

  EXPECT_EQ(target.VAO, source_vao);
  EXPECT_EQ(target.VBO, source_vbo);
  EXPECT_EQ(target.EBO, source_ebo);

  EXPECT_EQ(source.VAO, 0u);
  EXPECT_EQ(source.VBO, 0u);
  EXPECT_EQ(source.EBO, 0u);
};

TEST(MeshDestructor, DeletesGPUObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  {
    CW::Renderer::Mesh mesh(data);
    mesh.compile();

    vao = mesh.VAO;
    vbo = mesh.VBO;
    ebo = mesh.EBO;

    ASSERT_EQ(glIsVertexArray(vao), GL_TRUE);
    ASSERT_EQ(glIsBuffer(vbo), GL_TRUE);
    ASSERT_EQ(glIsBuffer(ebo), GL_TRUE);
  };

  EXPECT_EQ(glIsVertexArray(vao), GL_FALSE);
  EXPECT_EQ(glIsBuffer(vbo), GL_FALSE);
  EXPECT_EQ(glIsBuffer(ebo), GL_FALSE);
};



// =============================
// ======= Data Control ========
// =============================
TEST(MeshSetMeshData, CopiesNewData) {
  CW::Renderer::Shared::MeshData first = createMeshData();
  CW::Renderer::Shared::MeshData second;

  std::vector<float> vertices = {
    10.0f, 20.0f,
    30.0f, 40.0f
  };

  std::vector<unsigned int> indices = {0, 1};

  second.addVertices(vertices, 2, 0);
  second.addIndices(indices);

  CW::Renderer::Mesh mesh(first);

  mesh.setMeshData(second);

  EXPECT_EQ(mesh.getMeshData().getIndices(), second.getIndices());
  EXPECT_EQ(mesh.getMeshData().getDataRegister().at(0).getDimension(), 2u);
  EXPECT_FALSE(mesh.is_compiled);
};

TEST(MeshSetMeshData, DoesNotShareCPUStorage) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh;
  mesh.setMeshData(data);

  EXPECT_NE(mesh.mesh_data.dataRegister.at(0).getRawData(), data.getDataRegister().at(0).getRawData());
};

TEST(MeshSetMeshData, RecompilesWithNewMeshData) {
  CW::Renderer::Shared::MeshData first = createMeshData();

  CW::Renderer::Shared::MeshData second;

  std::vector<float> vertices = {
    -10.0f, -10.0f,
     10.0f, -10.0f,
      0.0f,  10.0f
  };

  std::vector<unsigned int> indices = {0, 1, 2};

  second.addVertices(vertices, 2, 0);
  second.addIndices(indices);

  CW::Renderer::Mesh mesh(first);
  mesh.compile();

  mesh.setMeshData(second);
  mesh.compile();

  EXPECT_TRUE(mesh.is_compiled);
  EXPECT_EQ(mesh.mesh_data_version, mesh.mesh_data.getVersion());

  EXPECT_EQ(glIsVertexArray(mesh.VAO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.VBO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.EBO), GL_TRUE);

  GLint vbo_size = 0;

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  EXPECT_EQ(vbo_size, static_cast<GLint>(mesh.mesh_data.getDataBuffer().size()));
};

TEST(MeshSetMeshData, UploadsNewDataAfterRecompile) {
  CW::Renderer::Shared::MeshData first = createMeshData();

  CW::Renderer::Shared::MeshData second;

  std::vector<float> vertices = {
    100.0f, 200.0f,
    300.0f, 400.0f
  };

  std::vector<unsigned int> indices = {0, 1};

  second.addVertices(vertices, 2, 0);
  second.addIndices(indices);

  CW::Renderer::Mesh mesh(first);
  mesh.compile();

  mesh.setMeshData(second);
  mesh.compile();

  std::vector<char> gpu_data(mesh.mesh_data.getDataBuffer().size());

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, gpu_data.size(), gpu_data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  EXPECT_EQ(gpu_data, mesh.mesh_data.getDataBuffer());
};

TEST(MeshCompile, CreatesGPUObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  EXPECT_TRUE(mesh.is_compiled);
  EXPECT_NE(mesh.VAO, 0u);
  EXPECT_NE(mesh.VBO, 0u);
  EXPECT_NE(mesh.EBO, 0u);

  EXPECT_EQ(glIsVertexArray(mesh.VAO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.VBO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.EBO), GL_TRUE);
};

TEST(MeshCompile, StoresCurrentMeshDataVersion) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  EXPECT_EQ(mesh.mesh_data_version, mesh.mesh_data.getVersion());
};

TEST(MeshCompile, IsIdempotentWhenDataDoesNotChange) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  const GLuint vao = mesh.VAO;
  const GLuint vbo = mesh.VBO;
  const GLuint ebo = mesh.EBO;

  mesh.compile();

  EXPECT_EQ(mesh.VAO, vao);
  EXPECT_EQ(mesh.VBO, vbo);
  EXPECT_EQ(mesh.EBO, ebo);
};

TEST(MeshCompile, UploadsCorrectVBOSize) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  GLint size = 0;

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  EXPECT_EQ(size, static_cast<GLint>(mesh.mesh_data.getDataBuffer().size()));
};

TEST(MeshCompile, UploadsCorrectEBOSize) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  GLint size = 0;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  EXPECT_EQ(size, static_cast<GLint>(mesh.mesh_data.getIndices().size() * sizeof(GLuint)));
};

TEST(MeshCompile, UploadsExactVertexBuffer) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  std::vector<char> gpu_data(mesh.mesh_data.getDataBuffer().size());

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, gpu_data.size(), gpu_data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  EXPECT_EQ(gpu_data, mesh.mesh_data.getDataBuffer());
};

TEST(MeshCompile, UploadsExactIndexBuffer) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  std::vector<unsigned int> gpu_indices(mesh.mesh_data.getIndices().size());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, gpu_indices.size() * sizeof(unsigned int), gpu_indices.data());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  EXPECT_EQ(gpu_indices, mesh.mesh_data.getIndices());
};

TEST(MeshCompile, HandlesMissingIndices) {
  CW::Renderer::Shared::MeshData data;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f};

  data.addVertices(vertices, 3, 0);

  CW::Renderer::Mesh mesh(data);

  EXPECT_NO_THROW(mesh.compile());
  EXPECT_FALSE(mesh.is_compiled);
};

TEST(MeshDestroy, DeletesAllGPUObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  const GLuint vao = mesh.VAO;
  const GLuint vbo = mesh.VBO;
  const GLuint ebo = mesh.EBO;

  mesh.destroy();

  EXPECT_EQ(glIsVertexArray(vao), GL_FALSE);
  EXPECT_EQ(glIsBuffer(vbo), GL_FALSE);
  EXPECT_EQ(glIsBuffer(ebo), GL_FALSE);

  EXPECT_EQ(mesh.VAO, 0u);
  EXPECT_EQ(mesh.VBO, 0u);
  EXPECT_EQ(mesh.EBO, 0u);
  EXPECT_FALSE(mesh.is_compiled);
};

TEST(MeshDestroy, CanBeCalledMultipleTimes) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  mesh.destroy();
  mesh.destroy();
  mesh.destroy();

  EXPECT_EQ(mesh.VAO, 0u);
  EXPECT_EQ(mesh.VBO, 0u);
  EXPECT_EQ(mesh.EBO, 0u);
  EXPECT_FALSE(mesh.is_compiled);
};

TEST(MeshRender, CompilesAutomatically) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);

  ASSERT_FALSE(mesh.is_compiled);

  mesh.render();

  EXPECT_TRUE(mesh.is_compiled);
  EXPECT_EQ(glIsVertexArray(mesh.VAO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.VBO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.EBO), GL_TRUE);
};

TEST(MeshRender, RecompilesWhenMeshDataChanges) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  const unsigned int compiled_version = mesh.mesh_data_version;

  std::vector<float> colors = {
    0.5f, 0.5f, 0.5f,
    0.2f, 0.2f, 0.2f,
    0.9f, 0.9f, 0.9f
  };

  mesh.mesh_data.setData<float>(colors, 3, 1);

  ASSERT_NE(mesh.mesh_data.getVersion(), compiled_version);

  mesh.render();

  EXPECT_EQ(mesh.mesh_data_version, mesh.mesh_data.getVersion());
};

TEST(MeshRender, LeavesVertexArrayUnbound) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.render();

  GLint vao = -1;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);

  EXPECT_EQ(vao, 0);
};



// =============================
// ========== Helpers ==========
// =============================
TEST(MeshGenBuffers, AllocatesCorrectObjectTypes) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.mesh_data.generateBuffer();

  mesh.genBuffers(mesh.mesh_data.getDataBuffer());

  EXPECT_EQ(glIsVertexArray(mesh.VAO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.VBO), GL_TRUE);
  EXPECT_EQ(glIsBuffer(mesh.EBO), GL_TRUE);

  mesh.closeBuffers();
};

TEST(MeshCloseBuffers, UnbindsObjects) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.mesh_data.generateBuffer();
  mesh.genBuffers(mesh.mesh_data.getDataBuffer());

  mesh.closeBuffers();

  GLint vao = -1;
  GLint vbo = -1;

  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);

  EXPECT_EQ(vao, 0);
  EXPECT_EQ(vbo, 0);
};

TEST(MeshSetDataPositions, ConfiguresAttributeZero) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  glBindVertexArray(mesh.VAO);

  GLint enabled = 0;
  GLint size = 0;
  GLint type = 0;
  GLint stride = 0;

  glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
  glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
  glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
  glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);

  glBindVertexArray(0);

  EXPECT_EQ(enabled, GL_TRUE);
  EXPECT_EQ(size, 3);
  EXPECT_EQ(type, GL_FLOAT);
  EXPECT_EQ(stride, static_cast<GLint>(mesh.mesh_data.getLineSize()));
};

TEST(MeshSetDataPositions, ConfiguresAllAttributes) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  glBindVertexArray(mesh.VAO);

  GLint first_enabled = 0;
  GLint second_enabled = 0;

  glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &first_enabled);
  glGetVertexAttribiv(1, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &second_enabled);

  glBindVertexArray(0);

  EXPECT_EQ(first_enabled, GL_TRUE);
  EXPECT_EQ(second_enabled, GL_TRUE);
};

TEST(MeshSetDataPositions, ConfiguresCorrectOffsets) {
  CW::Renderer::Shared::MeshData data = createMeshData();

  CW::Renderer::Mesh mesh(data);
  mesh.compile();

  glBindVertexArray(mesh.VAO);

  void* first = nullptr;
  void* second = nullptr;

  glGetVertexAttribPointerv(0, GL_VERTEX_ATTRIB_ARRAY_POINTER, &first);
  glGetVertexAttribPointerv(1, GL_VERTEX_ATTRIB_ARRAY_POINTER, &second);

  glBindVertexArray(0);

  EXPECT_EQ(reinterpret_cast<uintptr_t>(first), 0u);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(second), 3u * sizeof(float));
};
