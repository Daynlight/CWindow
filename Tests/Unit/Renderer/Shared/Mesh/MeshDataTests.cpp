// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <array>
#include <vector>
#include <cstring>
#include <limits>
#include <utility>

#define private public
#define protected public

#include "Renderer/Shared/Mesh/MeshData.h"

#undef private
#undef protected



template<typename T>
T readBufferValue(const std::vector<char>& buffer, const size_t offset) {
  T value;
  std::memcpy(&value, buffer.data() + offset, sizeof(T));
  return value;
};



// =============================
// ======= Constructors ========
// =============================
// core
TEST(MeshDataDefaultConstructor, InitializesDefaultValues) {
  CW::Renderer::Shared::MeshData mesh;

  EXPECT_TRUE(mesh.dataRegister.empty());
  EXPECT_TRUE(mesh.indices.empty());
  EXPECT_TRUE(mesh.bufferData.empty());
  EXPECT_EQ(mesh.line_size, 0u);
  EXPECT_TRUE(mesh.keys.empty());
  EXPECT_FALSE(mesh.culling_box_exists);
  EXPECT_FALSE(mesh.buffer_is_ready);
  EXPECT_FALSE(mesh.keys_are_ready);
  EXPECT_EQ(mesh.version, std::numeric_limits<unsigned int>::max());
};

TEST(MeshDataDefaultConstructor, InitializesCullingBoxEmpty) {
  CW::Renderer::Shared::MeshData mesh;

  EXPECT_TRUE(mesh.culling_box[0].empty());
  EXPECT_TRUE(mesh.culling_box[1].empty());
};

// copy
TEST(MeshDataCopyConstructor, CopiesCompleteState) {
  CW::Renderer::Shared::MeshData source;

  std::vector<float> vertices = {-2.0f, 4.0f, 1.0f, 3.0f, -5.0f, 8.0f};
  std::vector<unsigned int> indices = {0, 1};

  source.addVertices(vertices, 3, 0);
  source.addIndices(indices);
  source.generateBuffer();

  CW::Renderer::Shared::MeshData copy(source);

  EXPECT_EQ(copy.dataRegister.size(), source.dataRegister.size());
  EXPECT_EQ(copy.indices, source.indices);
  EXPECT_EQ(copy.bufferData, source.bufferData);
  EXPECT_EQ(copy.line_size, source.line_size);
  EXPECT_EQ(copy.culling_box, source.culling_box);
  EXPECT_EQ(copy.keys, source.keys);
  EXPECT_EQ(copy.culling_box_exists, source.culling_box_exists);
  EXPECT_EQ(copy.buffer_is_ready, source.buffer_is_ready);
  EXPECT_EQ(copy.keys_are_ready, source.keys_are_ready);
  EXPECT_EQ(copy.version, source.version);
};

TEST(MeshDataCopyConstructor, PerformsDeepCopyOfIndices) {
  CW::Renderer::Shared::MeshData source;

  std::vector<unsigned int> indices = {0, 1, 2, 3};
  source.addIndices(indices);

  CW::Renderer::Shared::MeshData copy(source);

  ASSERT_FALSE(source.indices.empty());
  ASSERT_FALSE(copy.indices.empty());

  EXPECT_NE(source.indices.data(), copy.indices.data());

  copy.indices[0] = 100;

  EXPECT_EQ(source.indices[0], 0u);
  EXPECT_EQ(copy.indices[0], 100u);
};

TEST(MeshDataCopyConstructor, PerformsDeepCopyOfCullingBox) {
  CW::Renderer::Shared::MeshData source;

  std::vector<float> vertices = {-4.0f, -2.0f, 8.0f, 6.0f, 7.0f, -9.0f};
  source.addVertices(vertices, 3, 0);

  CW::Renderer::Shared::MeshData copy(source);

  ASSERT_FALSE(source.culling_box[0].empty());
  ASSERT_FALSE(copy.culling_box[0].empty());

  EXPECT_NE(source.culling_box[0].data(), copy.culling_box[0].data());
  EXPECT_NE(source.culling_box[1].data(), copy.culling_box[1].data());

  copy.culling_box[0][0] = 100.0f;
  copy.culling_box[1][2] = 200.0f;

  EXPECT_FLOAT_EQ(source.culling_box[0][0], -4.0f);
  EXPECT_FLOAT_EQ(source.culling_box[1][2], 8.0f);
};

TEST(MeshDataCopyConstructor, PerformsDeepCopyOfDataRegister) {
  CW::Renderer::Shared::MeshData source;

  std::vector<float> data = {1.0f, 2.0f, 3.0f, 4.0f};
  source.setData<float>(data, 2, 3, CW::Renderer::Shared::MeshDataType::Float);

  CW::Renderer::Shared::MeshData copy(source);

  const auto& source_record = source.dataRegister.at(3);
  const auto& copy_record = copy.dataRegister.at(3);

  ASSERT_NE(source_record.getRawData(), nullptr);
  ASSERT_NE(copy_record.getRawData(), nullptr);

  EXPECT_NE(source_record.getRawData(), copy_record.getRawData());
  EXPECT_EQ(std::memcmp(source_record.getRawData(), copy_record.getRawData(), source_record.getSize()), 0);
};

TEST(MeshDataCopyConstructor, PerformsDeepCopyOfGeneratedBuffer) {
  CW::Renderer::Shared::MeshData source;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  source.addVertices(vertices, 2, 0);
  source.addIndices(indices);
  source.generateBuffer();

  CW::Renderer::Shared::MeshData copy(source);

  ASSERT_FALSE(source.bufferData.empty());
  ASSERT_FALSE(copy.bufferData.empty());

  EXPECT_NE(source.bufferData.data(), copy.bufferData.data());

  copy.bufferData[0] ^= 0x01;

  EXPECT_NE(source.bufferData[0], copy.bufferData[0]);
};

TEST(MeshDataCopyAssignment, CopiesCompleteState) {
  CW::Renderer::Shared::MeshData source;
  CW::Renderer::Shared::MeshData target;

  std::vector<float> vertices = {-1.0f, 2.0f, 3.0f, 9.0f};
  std::vector<unsigned int> indices = {0, 1};
  std::vector<int> old_data = {10, 20, 30};

  source.addVertices(vertices, 2, 2);
  source.addIndices(indices);
  source.generateBuffer();

  target.setData<int>(old_data, 1, 9, CW::Renderer::Shared::MeshDataType::Int);

  target = source;

  EXPECT_EQ(target.dataRegister.size(), source.dataRegister.size());
  EXPECT_EQ(target.indices, source.indices);
  EXPECT_EQ(target.bufferData, source.bufferData);
  EXPECT_EQ(target.line_size, source.line_size);
  EXPECT_EQ(target.culling_box, source.culling_box);
  EXPECT_EQ(target.keys, source.keys);
  EXPECT_EQ(target.culling_box_exists, source.culling_box_exists);
  EXPECT_EQ(target.buffer_is_ready, source.buffer_is_ready);
  EXPECT_EQ(target.keys_are_ready, source.keys_are_ready);
  EXPECT_EQ(target.version, source.version);
};

TEST(MeshDataCopyAssignment, ReplacesOldState) {
  CW::Renderer::Shared::MeshData source;
  CW::Renderer::Shared::MeshData target;

  std::vector<float> source_data = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<int> target_data = {100, 200, 300};

  source.setData<float>(source_data, 2, 3, CW::Renderer::Shared::MeshDataType::Float);
  target.setData<int>(target_data, 1, 8, CW::Renderer::Shared::MeshDataType::Int);

  target = source;

  EXPECT_TRUE(target.dataRegister.contains(3));
  EXPECT_FALSE(target.dataRegister.contains(8));
};

TEST(MeshDataCopyAssignment, PerformsDeepCopy) {
  CW::Renderer::Shared::MeshData source;
  CW::Renderer::Shared::MeshData target;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  source.addVertices(vertices, 2, 0);
  source.addIndices(indices);

  target = source;

  ASSERT_FALSE(source.indices.empty());
  ASSERT_FALSE(target.indices.empty());

  EXPECT_NE(source.indices.data(), target.indices.data());
  EXPECT_NE(source.dataRegister.at(0).getRawData(), target.dataRegister.at(0).getRawData());

  target.indices[0] = 100;

  EXPECT_EQ(source.indices[0], 0u);
};

TEST(MeshDataCopyAssignment, HandlesSelfAssignment) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(vertices, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  const char* buffer_ptr = mesh.bufferData.data();
  const unsigned int version = mesh.version;

  mesh = mesh;

  EXPECT_EQ(mesh.bufferData.data(), buffer_ptr);
  EXPECT_EQ(mesh.version, version);
  EXPECT_THAT(mesh.indices, ::testing::ElementsAre(0, 1));
};

// move
TEST(MeshDataMoveConstructor, TransfersCompleteState) {
  CW::Renderer::Shared::MeshData source;

  std::vector<float> vertices = {-2.0f, 5.0f, 7.0f, -4.0f};
  std::vector<unsigned int> indices = {0, 1};

  source.addVertices(vertices, 2, 0);
  source.addIndices(indices);
  source.generateBuffer();

  const char* buffer_ptr = source.bufferData.data();
  const unsigned int* indices_ptr = source.indices.data();
  const unsigned int version = source.version;

  CW::Renderer::Shared::MeshData moved(std::move(source));

  EXPECT_EQ(moved.bufferData.data(), buffer_ptr);
  EXPECT_EQ(moved.indices.data(), indices_ptr);
  EXPECT_EQ(moved.version, version);
  EXPECT_EQ(source.version, std::numeric_limits<unsigned int>::max());
};

TEST(MeshDataMoveConstructor, TransfersDataRegisterStorage) {
  CW::Renderer::Shared::MeshData source;

  std::vector<float> values = {1.0f, 2.0f, 3.0f, 4.0f};
  source.setData<float>(values, 2, 4, CW::Renderer::Shared::MeshDataType::Float);

  const char* ptr = source.dataRegister.at(4).getRawData();

  CW::Renderer::Shared::MeshData moved(std::move(source));

  ASSERT_TRUE(moved.dataRegister.contains(4));
  EXPECT_EQ(moved.dataRegister.at(4).getRawData(), ptr);
};

TEST(MeshDataMoveAssignment, TransfersCompleteState) {
  CW::Renderer::Shared::MeshData source;
  CW::Renderer::Shared::MeshData target;

  std::vector<float> vertices = {-3.0f, 5.0f, 8.0f, 2.0f};
  std::vector<unsigned int> indices = {0, 1};
  std::vector<int> old_data = {1, 2, 3};

  source.addVertices(vertices, 2, 0);
  source.addIndices(indices);
  source.generateBuffer();

  target.setData<int>(old_data, 1, 8, CW::Renderer::Shared::MeshDataType::Int);

  const char* buffer_ptr = source.bufferData.data();
  const unsigned int* indices_ptr = source.indices.data();
  const unsigned int version = source.version;

  target = std::move(source);

  EXPECT_EQ(target.bufferData.data(), buffer_ptr);
  EXPECT_EQ(target.indices.data(), indices_ptr);
  EXPECT_EQ(target.version, version);
  EXPECT_EQ(source.version, std::numeric_limits<unsigned int>::max());
  EXPECT_FALSE(target.dataRegister.contains(8));
};

TEST(MeshDataMoveAssignment, HandlesSelfAssignment) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(vertices, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  const char* buffer_ptr = mesh.bufferData.data();
  const unsigned int version = mesh.version;

  mesh = std::move(mesh);

  EXPECT_EQ(mesh.bufferData.data(), buffer_ptr);
  EXPECT_EQ(mesh.version, version);
  EXPECT_THAT(mesh.indices, ::testing::ElementsAre(0, 1));
};



// =============================
// ======= Data Control ========
// =============================
TEST(MeshDataAddVertices, CreatesFloatDataRecord) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  mesh.addVertices(vertices, 3, 5);

  ASSERT_TRUE(mesh.dataRegister.contains(5));

  const auto& record = mesh.dataRegister.at(5);

  EXPECT_EQ(record.getSize(), vertices.size() * sizeof(float));
  EXPECT_EQ(record.getSizeOfElement(), sizeof(float));
  EXPECT_EQ(record.getDimension(), 3u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Float);
};

TEST(MeshDataAddVertices, StoresExactVertexBytes) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.25f, -2.5f, 100.75f, -0.125f};

  mesh.addVertices(vertices, 2, 0);

  const auto& record = mesh.dataRegister.at(0);

  ASSERT_EQ(record.getSize(), vertices.size() * sizeof(float));
  EXPECT_EQ(std::memcmp(record.getRawData(), vertices.data(), record.getSize()), 0);
};

TEST(MeshDataAddVertices, GeneratesCorrectCullingBox3D) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {
    -5.0f,  2.0f, 10.0f,
     4.0f, -3.0f,  8.0f,
     0.0f,  7.0f, -2.0f,
    -1.0f,  1.0f, 20.0f
  };

  mesh.addVertices(vertices, 3, 0);

  ASSERT_TRUE(mesh.getCullingBoxExists());
  ASSERT_EQ(mesh.getCullingBox()[0].size(), 3u);
  ASSERT_EQ(mesh.getCullingBox()[1].size(), 3u);

  EXPECT_THAT(mesh.getCullingBox()[0], ::testing::ElementsAre(-5.0f, -3.0f, -2.0f));
  EXPECT_THAT(mesh.getCullingBox()[1], ::testing::ElementsAre(4.0f, 7.0f, 20.0f));
};

TEST(MeshDataAddVertices, GeneratesCorrectCullingBox2D) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {-10.0f, 5.0f, 20.0f, -30.0f, 0.0f, 100.0f};

  mesh.addVertices(vertices, 2, 0);

  EXPECT_THAT(mesh.getCullingBox()[0], ::testing::ElementsAre(-10.0f, -30.0f));
  EXPECT_THAT(mesh.getCullingBox()[1], ::testing::ElementsAre(20.0f, 100.0f));
};

TEST(MeshDataAddVertices, GeneratesCorrectCullingBoxForSingleVertex) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {-7.0f, 15.0f, 3.5f};

  mesh.addVertices(vertices, 3, 0);

  EXPECT_THAT(mesh.getCullingBox()[0], ::testing::ElementsAre(-7.0f, 15.0f, 3.5f));
  EXPECT_THAT(mesh.getCullingBox()[1], ::testing::ElementsAre(-7.0f, 15.0f, 3.5f));
};

TEST(MeshDataAddVertices, ReplacesExistingLayout) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> first = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<float> second = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f};

  mesh.addVertices(first, 2, 0);
  mesh.addVertices(second, 3, 0);

  ASSERT_EQ(mesh.dataRegister.size(), 1u);

  const auto& record = mesh.dataRegister.at(0);

  EXPECT_EQ(record.getDimension(), 3u);
  EXPECT_EQ(record.getSize(), second.size() * sizeof(float));
  EXPECT_EQ(std::memcmp(record.getRawData(), second.data(), record.getSize()), 0);
};

TEST(MeshDataAddVertices, InvalidatesGeneratedState) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> first = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(first, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_TRUE(mesh.buffer_is_ready);
  ASSERT_TRUE(mesh.keys_are_ready);

  std::vector<float> second = {5.0f, 6.0f, 7.0f, 8.0f};
  mesh.addVertices(second, 2, 0);

  EXPECT_FALSE(mesh.buffer_is_ready);
  EXPECT_FALSE(mesh.keys_are_ready);
};

TEST(MeshDataAddIndices, CopiesIndices) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<unsigned int> indices = {5, 4, 3, 2, 1, 0};

  mesh.addIndices(indices);

  EXPECT_EQ(mesh.getIndices(), indices);
  EXPECT_NE(mesh.getIndices().data(), indices.data());

  indices[0] = 100;

  EXPECT_EQ(mesh.getIndices()[0], 5u);
};

TEST(MeshDataAddIndices, ReplacesExistingIndices) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<unsigned int> first = {0, 1, 2};
  std::vector<unsigned int> second = {6, 7, 8, 9};

  mesh.addIndices(first);
  mesh.addIndices(second);

  EXPECT_THAT(mesh.getIndices(), ::testing::ElementsAre(6, 7, 8, 9));
};

TEST(MeshDataGetIndices, ReturnsInternalVector) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<unsigned int> indices = {0, 2, 4, 6};

  mesh.addIndices(indices);

  EXPECT_EQ(&mesh.getIndices(), &mesh.indices);
};

TEST(MeshDataSetData, StoresExactFloatBytes) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.25f, -5.5f, 100.125f, 0.0f};

  mesh.setData<float>(data, 2, 3, CW::Renderer::Shared::MeshDataType::Float);

  const auto& record = mesh.dataRegister.at(3);

  EXPECT_EQ(record.getSize(), data.size() * sizeof(float));
  EXPECT_EQ(record.getSizeOfElement(), sizeof(float));
  EXPECT_EQ(record.getDimension(), 2u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Float);
  EXPECT_EQ(std::memcmp(record.getRawData(), data.data(), record.getSize()), 0);
};

TEST(MeshDataSetData, StoresExactIntegerBytes) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<int> data = {-100, 0, 42, 1000000};

  mesh.setData<int>(data, 1, 7, CW::Renderer::Shared::MeshDataType::Int);

  const auto& record = mesh.dataRegister.at(7);

  EXPECT_EQ(record.getSize(), data.size() * sizeof(int));
  EXPECT_EQ(record.getSizeOfElement(), sizeof(int));
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Int);
  EXPECT_EQ(std::memcmp(record.getRawData(), data.data(), record.getSize()), 0);
};

TEST(MeshDataSetData, StoresExactUnsignedIntegerBytes) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<unsigned int> data = {0u, 1u, 4000000000u};

  mesh.setData<unsigned int>(data, 1, 9, CW::Renderer::Shared::MeshDataType::UInt);

  const auto& record = mesh.dataRegister.at(9);

  EXPECT_EQ(record.getSizeOfElement(), sizeof(unsigned int));
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::UInt);
  EXPECT_EQ(std::memcmp(record.getRawData(), data.data(), record.getSize()), 0);
};

TEST(MeshDataSetData, StoresByteData) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<char> data = {-128, -1, 0, 1, 127};

  mesh.setData<char>(data, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);

  const auto& record = mesh.dataRegister.at(2);

  EXPECT_EQ(record.getSize(), data.size());
  EXPECT_EQ(record.getSizeOfElement(), 1u);
  EXPECT_EQ(record.getDimension(), 1u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Byte);
  EXPECT_EQ(std::memcmp(record.getRawData(), data.data(), record.getSize()), 0);
};

TEST(MeshDataSetData, IgnoresZeroDimension) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f};
  const unsigned int version = mesh.version;

  mesh.setData<float>(data, 0, 4, CW::Renderer::Shared::MeshDataType::Float);

  EXPECT_TRUE(mesh.dataRegister.empty());
  EXPECT_EQ(mesh.version, version);
};

TEST(MeshDataSetData, ReplacesSameLayout) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> first = {1.0f, 2.0f};
  std::vector<int> second = {10, 20, 30, 40};

  mesh.setData<float>(first, 1, 5, CW::Renderer::Shared::MeshDataType::Float);
  mesh.setData<int>(second, 2, 5, CW::Renderer::Shared::MeshDataType::Int);

  ASSERT_EQ(mesh.dataRegister.size(), 1u);

  const auto& record = mesh.dataRegister.at(5);

  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Int);
  EXPECT_EQ(record.getDimension(), 2u);
  EXPECT_EQ(record.getSizeOfElement(), sizeof(int));
  EXPECT_EQ(record.getSize(), second.size() * sizeof(int));
};

TEST(MeshDataSetData, InvalidatesCachedKeysAndBuffer) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(vertices, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_TRUE(mesh.buffer_is_ready);
  ASSERT_TRUE(mesh.keys_are_ready);

  std::vector<float> second = {5.0f, 6.0f};

  mesh.setData<float>(second, 1, 1, CW::Renderer::Shared::MeshDataType::Float);

  EXPECT_FALSE(mesh.buffer_is_ready);
  EXPECT_FALSE(mesh.keys_are_ready);
};

TEST(MeshDataRemoveData, RemovesOnlyRequestedLayout) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> first = {1.0f, 2.0f};
  std::vector<float> second = {3.0f, 4.0f};
  std::vector<float> third = {5.0f, 6.0f};

  mesh.setData<float>(first, 1, 1);
  mesh.setData<float>(second, 1, 2);
  mesh.setData<float>(third, 1, 3);

  mesh.removeData(2);

  EXPECT_TRUE(mesh.dataRegister.contains(1));
  EXPECT_FALSE(mesh.dataRegister.contains(2));
  EXPECT_TRUE(mesh.dataRegister.contains(3));
  EXPECT_EQ(mesh.dataRegister.size(), 2u);
};

TEST(MeshDataRemoveData, InvalidatesCaches) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(vertices, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_TRUE(mesh.buffer_is_ready);
  ASSERT_TRUE(mesh.keys_are_ready);

  mesh.removeData(0);

  EXPECT_FALSE(mesh.buffer_is_ready);
  EXPECT_FALSE(mesh.keys_are_ready);
};

TEST(MeshDataClearData, RemovesAllDataRecords) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f};

  mesh.setData<float>(data, 1, 0);
  mesh.setData<float>(data, 1, 1);
  mesh.setData<float>(data, 1, 2);

  ASSERT_EQ(mesh.dataRegister.size(), 3u);

  mesh.clearData();

  EXPECT_TRUE(mesh.dataRegister.empty());
};

TEST(MeshDataClearData, InvalidatesCaches) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(vertices, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_TRUE(mesh.buffer_is_ready);
  ASSERT_TRUE(mesh.keys_are_ready);

  mesh.clearData();

  EXPECT_FALSE(mesh.buffer_is_ready);
  EXPECT_FALSE(mesh.keys_are_ready);
};

TEST(MeshDataGetCullingBoxExists, ReflectsGeneratedCullingBox) {
  CW::Renderer::Shared::MeshData mesh;

  EXPECT_FALSE(mesh.getCullingBoxExists());

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f};
  mesh.addVertices(vertices, 3, 0);

  EXPECT_TRUE(mesh.getCullingBoxExists());
};

TEST(MeshDataGetCullingBox, ReturnsInternalCullingBox) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {-1.0f, -2.0f, 5.0f, 6.0f};
  mesh.addVertices(vertices, 2, 0);

  EXPECT_EQ(&mesh.getCullingBox(), &mesh.culling_box);
};

TEST(MeshDataGetDataRegister, ReturnsInternalRegister) {
  CW::Renderer::Shared::MeshData mesh;

  EXPECT_EQ(&mesh.getDataRegister(), &mesh.dataRegister);
};

TEST(MeshDataGetDataRegisterLayouts, ReturnsSortedLayouts) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f};

  mesh.setData<float>(data, 1, 10);
  mesh.setData<float>(data, 1, 2);
  mesh.setData<float>(data, 1, 17);
  mesh.setData<float>(data, 1, 0);
  mesh.setData<float>(data, 1, 5);

  const auto& layouts = mesh.getDataRegisterLayouts();

  EXPECT_THAT(layouts, ::testing::ElementsAre(0, 2, 5, 10, 17));
};

TEST(MeshDataGetDataRegisterLayouts, UsesCachedResult) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f};

  mesh.setData<float>(data, 1, 4);
  mesh.setData<float>(data, 1, 1);

  const auto& first = mesh.getDataRegisterLayouts();
  const unsigned int version = mesh.version;
  const unsigned int* ptr = first.data();

  const auto& second = mesh.getDataRegisterLayouts();

  EXPECT_EQ(second.data(), ptr);
  EXPECT_EQ(mesh.version, version);
  EXPECT_TRUE(mesh.keys_are_ready);
};

TEST(MeshDataGetDataRegisterLayouts, RebuildsAfterDataChange) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f};

  mesh.setData<float>(data, 1, 5);
  mesh.setData<float>(data, 1, 1);

  EXPECT_THAT(mesh.getDataRegisterLayouts(), ::testing::ElementsAre(1, 5));

  mesh.setData<float>(data, 1, 3);

  EXPECT_FALSE(mesh.keys_are_ready);
  EXPECT_THAT(mesh.getDataRegisterLayouts(), ::testing::ElementsAre(1, 3, 5));
};

TEST(MeshDataGenerateBuffer, ThrowsWithoutIndices) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  mesh.addVertices(vertices, 2, 0);

  EXPECT_THROW(mesh.generateBuffer(), std::runtime_error);
};

TEST(MeshDataGenerateBuffer, ThrowsWithoutVertexData) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<unsigned int> indices = {0, 1};
  mesh.addIndices(indices);

  EXPECT_THROW(mesh.generateBuffer(), std::runtime_error);
};

TEST(MeshDataGenerateBuffer, GeneratesSingleLayoutBufferExactly) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.5f, 2.5f, 3.5f, 4.5f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.setData<float>(vertices, 2, 0, CW::Renderer::Shared::MeshDataType::Float);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_EQ(mesh.getDataBuffer().size(), vertices.size() * sizeof(float));

  EXPECT_EQ(std::memcmp(mesh.getDataBuffer().data(), vertices.data(), mesh.getDataBuffer().size()), 0);
  EXPECT_EQ(mesh.getLineSize(), 2u * sizeof(float));
};

TEST(MeshDataGenerateBuffer, InterleavesTwoFloatLayouts) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> positions = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<float> colors = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.setData<float>(positions, 2, 0, CW::Renderer::Shared::MeshDataType::Float);
  mesh.setData<float>(colors, 3, 1, CW::Renderer::Shared::MeshDataType::Float);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  const auto& buffer = mesh.getDataBuffer();

  ASSERT_EQ(buffer.size(), 10u * sizeof(float));
  EXPECT_EQ(mesh.getLineSize(), 5u * sizeof(float));

  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 0), 1.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 4), 2.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 8), 10.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 12), 20.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 16), 30.0f);

  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 20), 3.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 24), 4.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 28), 40.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 32), 50.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 36), 60.0f);
};

TEST(MeshDataGenerateBuffer, InterleavesLayoutsBySortedLayoutId) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> late = {100.0f, 200.0f};
  std::vector<float> early = {1.0f, 2.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.setData<float>(late, 1, 10, CW::Renderer::Shared::MeshDataType::Float);
  mesh.setData<float>(early, 1, 2, CW::Renderer::Shared::MeshDataType::Float);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  const auto& buffer = mesh.getDataBuffer();

  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 0), 1.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 4), 100.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 8), 2.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 12), 200.0f);
};

TEST(MeshDataGenerateBuffer, InterleavesMixedDataTypes) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> positions = {1.5f, 2.5f, 3.5f, 4.5f};
  std::vector<unsigned int> ids = {100u, 200u};
  std::vector<char> flags = {7, 9};
  std::vector<unsigned int> indices = {0, 1};

  mesh.setData<float>(positions, 2, 0, CW::Renderer::Shared::MeshDataType::Float);
  mesh.setData<unsigned int>(ids, 1, 1, CW::Renderer::Shared::MeshDataType::UInt);
  mesh.setData<char>(flags, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  const auto& buffer = mesh.getDataBuffer();
  const size_t line_size = sizeof(float) * 2 + sizeof(unsigned int) + sizeof(char);

  ASSERT_EQ(mesh.getLineSize(), line_size);
  ASSERT_EQ(buffer.size(), line_size * 2);

  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 0), 1.5f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, 4), 2.5f);
  EXPECT_EQ(readBufferValue<unsigned int>(buffer, 8), 100u);
  EXPECT_EQ(readBufferValue<char>(buffer, 12), 7);

  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, line_size), 3.5f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(buffer, line_size + 4), 4.5f);
  EXPECT_EQ(readBufferValue<unsigned int>(buffer, line_size + 8), 200u);
  EXPECT_EQ(readBufferValue<char>(buffer, line_size + 12), 9);
};

TEST(MeshDataGenerateBuffer, HandlesLargeInterleavedBuffer) {
  CW::Renderer::Shared::MeshData mesh;

  constexpr unsigned int count = 10000;

  std::vector<float> first(count * 3);
  std::vector<float> second(count * 2);
  std::vector<unsigned int> indices(count);

  for(unsigned int i = 0; i < count; i++) {
    first[i * 3] = static_cast<float>(i);
    first[i * 3 + 1] = static_cast<float>(i + 1);
    first[i * 3 + 2] = static_cast<float>(i + 2);

    second[i * 2] = static_cast<float>(i * 10);
    second[i * 2 + 1] = static_cast<float>(i * 20);

    indices[i] = i;
  };

  mesh.setData<float>(first, 3, 0);
  mesh.setData<float>(second, 2, 1);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_EQ(mesh.getLineSize(), 5u * sizeof(float));
  ASSERT_EQ(mesh.getDataBuffer().size(), count * 5u * sizeof(float));

  const size_t last = (count - 1) * mesh.getLineSize();

  EXPECT_FLOAT_EQ(readBufferValue<float>(mesh.getDataBuffer(), last), static_cast<float>(count - 1));
  EXPECT_FLOAT_EQ(readBufferValue<float>(mesh.getDataBuffer(), last + 12), static_cast<float>((count - 1) * 10));
  EXPECT_FLOAT_EQ(readBufferValue<float>(mesh.getDataBuffer(), last + 16), static_cast<float>((count - 1) * 20));
};

TEST(MeshDataGenerateBuffer, UsesCachedBufferWhenReady) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> vertices = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.addVertices(vertices, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  ASSERT_TRUE(mesh.buffer_is_ready);

  const char* ptr = mesh.bufferData.data();
  const unsigned int version = mesh.version;
  const auto buffer = mesh.bufferData;

  mesh.generateBuffer();

  EXPECT_EQ(mesh.bufferData.data(), ptr);
  EXPECT_EQ(mesh.bufferData, buffer);
  EXPECT_EQ(mesh.version, version);
};

TEST(MeshDataGenerateBuffer, RegeneratesAfterDataReplacement) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> first = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<float> second = {10.0f, 20.0f, 30.0f, 40.0f};
  std::vector<unsigned int> indices = {0, 1};

  mesh.setData<float>(first, 2, 0);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  EXPECT_FLOAT_EQ(readBufferValue<float>(mesh.bufferData, 0), 1.0f);

  mesh.setData<float>(second, 2, 0);
  mesh.generateBuffer();

  EXPECT_FLOAT_EQ(readBufferValue<float>(mesh.bufferData, 0), 10.0f);
  EXPECT_FLOAT_EQ(readBufferValue<float>(mesh.bufferData, 12), 40.0f);
};

TEST(MeshDataGetLineSize, ReturnsCombinedVertexSize) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> positions = {1.0f, 2.0f, 3.0f, 4.0f};
  std::vector<unsigned int> ids = {10, 20};
  std::vector<char> flags = {1, 0};
  std::vector<unsigned int> indices = {0, 1};

  mesh.setData<float>(positions, 2, 0);
  mesh.setData<unsigned int>(ids, 1, 1, CW::Renderer::Shared::MeshDataType::UInt);
  mesh.setData<char>(flags, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  mesh.addIndices(indices);
  mesh.generateBuffer();

  EXPECT_EQ(mesh.getLineSize(), sizeof(float) * 2 + sizeof(unsigned int) + sizeof(char));
};

TEST(MeshDataGetDataBuffer, ReturnsInternalBuffer) {
  CW::Renderer::Shared::MeshData mesh;

  EXPECT_EQ(&mesh.getDataBuffer(), &mesh.bufferData);
};

TEST(MeshDataGetVersion, ReturnsInternalVersion) {
  CW::Renderer::Shared::MeshData mesh;

  EXPECT_EQ(mesh.getVersion(), mesh.version);
};

TEST(MeshDataVersion, SetDataChangesVersion) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f};

  EXPECT_EQ(mesh.getVersion(), std::numeric_limits<unsigned int>::max());

  mesh.setData<float>(data, 1, 0);

  EXPECT_EQ(mesh.getVersion(), 0u);

  mesh.setData<float>(data, 1, 1);

  EXPECT_EQ(mesh.getVersion(), 1u);
};

TEST(MeshDataVersion, AddIndicesChangesVersion) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<unsigned int> indices = {0, 1};

  mesh.addIndices(indices);

  EXPECT_EQ(mesh.getVersion(), 0u);

  mesh.addIndices(indices);

  EXPECT_EQ(mesh.getVersion(), 1u);
};



// =============================
// ========== Helpers ==========
// =============================
TEST(MeshDataGenerateCullingBox, HandlesAllNegativeValues) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {
    -10.0f, -20.0f, -30.0f,
     -1.0f,  -2.0f,  -3.0f,
     -5.0f, -50.0f,  -7.0f
  };

  mesh.generateCullingBox(data, 3);

  EXPECT_TRUE(mesh.culling_box_exists);
  EXPECT_THAT(mesh.culling_box[0], ::testing::ElementsAre(-10.0f, -50.0f, -30.0f));
  EXPECT_THAT(mesh.culling_box[1], ::testing::ElementsAre(-1.0f, -2.0f, -3.0f));
};

TEST(MeshDataGenerateCullingBox, HandlesAllPositiveValues) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {
    10.0f, 20.0f, 30.0f,
     1.0f,  2.0f,  3.0f,
     5.0f, 50.0f,  7.0f
  };

  mesh.generateCullingBox(data, 3);

  EXPECT_THAT(mesh.culling_box[0], ::testing::ElementsAre(1.0f, 2.0f, 3.0f));
  EXPECT_THAT(mesh.culling_box[1], ::testing::ElementsAre(10.0f, 50.0f, 30.0f));
};

TEST(MeshDataGenerateCullingBox, HandlesFourDimensions) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {
    1.0f,  2.0f,  3.0f,  4.0f,
   -1.0f, 10.0f, -3.0f, 40.0f,
    5.0f, -2.0f, 30.0f,  0.0f
  };

  mesh.generateCullingBox(data, 4);

  EXPECT_THAT(mesh.culling_box[0], ::testing::ElementsAre(-1.0f, -2.0f, -3.0f, 0.0f));
  EXPECT_THAT(mesh.culling_box[1], ::testing::ElementsAre(5.0f, 10.0f, 30.0f, 40.0f));
};

TEST(MeshDataGenerateCullingBox, ZeroDimensionClearsExistingBox) {
  CW::Renderer::Shared::MeshData mesh;

  std::vector<float> data = {1.0f, 2.0f, 3.0f};

  mesh.generateCullingBox(data, 3);

  ASSERT_TRUE(mesh.culling_box_exists);

  mesh.generateCullingBox(data, 0);

  EXPECT_FALSE(mesh.culling_box_exists);
  EXPECT_THAT(mesh.culling_box[0], ::testing::ElementsAre(0.0f));
  EXPECT_THAT(mesh.culling_box[1], ::testing::ElementsAre(0.0f));
};
