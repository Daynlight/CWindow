// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <utility>

#define private public
#define protected public

#include "CWindow/Renderer/Shared/Uniform/UniformDataRecord.h"

#undef private
#undef protected



// =============================
// ======= Constructors ========
// =============================
// core
TEST(UniformDataRecordDefaultConstructor, InitializesDefaultValues) {
  CW::Renderer::Shared::UniformDataRecord record;

  EXPECT_EQ(record.get<float>(), 0.0f);
  EXPECT_EQ(record.getSize(), 1u);
  EXPECT_EQ(record.getType(), &typeid(float));
  EXPECT_TRUE(std::holds_alternative<float>(record.getValue()));
};


// copy
TEST(UniformDataRecordCopyConstructor, CopiesIntValue) {
  CW::Renderer::Shared::UniformDataRecord original;

  original.set<int>(42, 5);

  CW::Renderer::Shared::UniformDataRecord copy(original);

  EXPECT_EQ(copy.get<int>(), 42);
  EXPECT_EQ(copy.getSize(), 5u);
  EXPECT_EQ(copy.getType(), &typeid(int));
};

TEST(UniformDataRecordCopyConstructor, CopiesVectorValue) {
  CW::Renderer::Shared::UniformDataRecord original;

  original.set<glm::vec4>({1.0f, 2.0f, 3.0f, 4.0f}, 3);

  CW::Renderer::Shared::UniformDataRecord copy(original);

  EXPECT_EQ(copy.get<glm::vec4>(), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(copy.getSize(), 3u);
  EXPECT_EQ(copy.getType(), &typeid(glm::vec4));
};

TEST(UniformDataRecordCopyConstructor, CopiesMatrixValue) {
  glm::mat4 value = {
    1.0f, 2.0f, 3.0f, 4.0f,
    5.0f, 6.0f, 7.0f, 8.0f,
    9.0f, 10.0f, 11.0f, 12.0f,
    13.0f, 14.0f, 15.0f, 16.0f
  };

  CW::Renderer::Shared::UniformDataRecord original;

  original.set<glm::mat4>(value);

  CW::Renderer::Shared::UniformDataRecord copy(original);

  EXPECT_EQ(copy.get<glm::mat4>(), value);
  EXPECT_EQ(copy.getSize(), 1u);
  EXPECT_EQ(copy.getType(), &typeid(glm::mat4));
};

TEST(UniformDataRecordCopyConstructor, KeepsValuesIndependent) {
  CW::Renderer::Shared::UniformDataRecord original;

  original.set<float>(10.0f);

  CW::Renderer::Shared::UniformDataRecord copy(original);

  original.set<float>(99.0f);

  EXPECT_EQ(original.get<float>(), 99.0f);
  EXPECT_EQ(copy.get<float>(), 10.0f);
};

TEST(UniformDataRecordCopyAssignment, CopiesValues) {
  CW::Renderer::Shared::UniformDataRecord source;
  CW::Renderer::Shared::UniformDataRecord target;

  source.set<double>(123.456, 4);
  target.set<int>(10);

  target = source;

  EXPECT_EQ(target.get<double>(), 123.456);
  EXPECT_EQ(target.getSize(), 4u);
  EXPECT_EQ(target.getType(), &typeid(double));
};

TEST(UniformDataRecordCopyAssignment, ReplacesPreviousValue) {
  CW::Renderer::Shared::UniformDataRecord source;
  CW::Renderer::Shared::UniformDataRecord target;

  source.set<glm::vec3>({1.0f, 2.0f, 3.0f}, 3);
  target.set<int>(10, 50);

  target = source;

  EXPECT_TRUE(std::holds_alternative<glm::vec3>(target.getValue()));
  EXPECT_EQ(target.get<glm::vec3>(), glm::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(target.getSize(), 3u);
  EXPECT_EQ(target.getType(), &typeid(glm::vec3));
};

TEST(UniformDataRecordCopyAssignment, KeepsValuesIndependent) {
  CW::Renderer::Shared::UniformDataRecord source;
  CW::Renderer::Shared::UniformDataRecord target;

  source.set<int>(20);

  target = source;

  source.set<int>(50);

  EXPECT_EQ(source.get<int>(), 50);
  EXPECT_EQ(target.get<int>(), 20);
};

TEST(UniformDataRecordCopyAssignment, HandlesSelfAssignment) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<double>(55.5, 7);

  record = record;

  EXPECT_EQ(record.get<double>(), 55.5);
  EXPECT_EQ(record.getSize(), 7u);
  EXPECT_EQ(record.getType(), &typeid(double));
};


// move
TEST(UniformDataRecordMoveConstructor, TransfersValues) {
  CW::Renderer::Shared::UniformDataRecord source;

  source.set<int>(42, 3);

  CW::Renderer::Shared::UniformDataRecord moved(std::move(source));

  EXPECT_EQ(moved.get<int>(), 42);
  EXPECT_EQ(moved.getSize(), 3u);
  EXPECT_EQ(moved.getType(), &typeid(int));
};

TEST(UniformDataRecordMoveConstructor, ResetsSource) {
  CW::Renderer::Shared::UniformDataRecord source;

  source.set<double>(123.0, 8);

  CW::Renderer::Shared::UniformDataRecord moved(std::move(source));

  EXPECT_EQ(source.get<float>(), 0.0f);
  EXPECT_EQ(source.getSize(), 1u);
  EXPECT_EQ(source.getType(), &typeid(float));
};

TEST(UniformDataRecordMoveConstructor, TransfersVectorValue) {
  glm::dvec4 value = {1.0, 2.0, 3.0, 4.0};

  CW::Renderer::Shared::UniformDataRecord source;

  source.set<glm::dvec4>(value, 2);

  CW::Renderer::Shared::UniformDataRecord moved(std::move(source));

  EXPECT_EQ(moved.get<glm::dvec4>(), value);
  EXPECT_EQ(moved.getSize(), 2u);
  EXPECT_EQ(moved.getType(), &typeid(glm::dvec4));

  EXPECT_EQ(source.get<float>(), 0.0f);
  EXPECT_EQ(source.getSize(), 1u);
  EXPECT_EQ(source.getType(), &typeid(float));
};

TEST(UniformDataRecordMoveConstructor, TransfersMatrixValue) {
  glm::mat3 value = {
    1.0f, 2.0f, 3.0f,
    4.0f, 5.0f, 6.0f,
    7.0f, 8.0f, 9.0f
  };

  CW::Renderer::Shared::UniformDataRecord source;

  source.set<glm::mat3>(value);

  CW::Renderer::Shared::UniformDataRecord moved(std::move(source));

  EXPECT_EQ(moved.get<glm::mat3>(), value);
  EXPECT_EQ(moved.getType(), &typeid(glm::mat3));
};

TEST(UniformDataRecordMoveAssignment, TransfersValues) {
  CW::Renderer::Shared::UniformDataRecord source;
  CW::Renderer::Shared::UniformDataRecord target;

  source.set<double>(15.5, 6);
  target.set<int>(123);

  target = std::move(source);

  EXPECT_EQ(target.get<double>(), 15.5);
  EXPECT_EQ(target.getSize(), 6u);
  EXPECT_EQ(target.getType(), &typeid(double));
};

TEST(UniformDataRecordMoveAssignment, ReplacesPreviousValue) {
  CW::Renderer::Shared::UniformDataRecord source;
  CW::Renderer::Shared::UniformDataRecord target;

  source.set<glm::ivec3>({1, 2, 3}, 7);
  target.set<double>(123.456, 50);

  target = std::move(source);

  EXPECT_TRUE(std::holds_alternative<glm::ivec3>(target.getValue()));
  EXPECT_EQ(target.get<glm::ivec3>(), glm::ivec3(1, 2, 3));
  EXPECT_EQ(target.getSize(), 7u);
  EXPECT_EQ(target.getType(), &typeid(glm::ivec3));
};

TEST(UniformDataRecordMoveAssignment, ResetsSource) {
  CW::Renderer::Shared::UniformDataRecord source;
  CW::Renderer::Shared::UniformDataRecord target;

  source.set<int>(99, 4);

  target = std::move(source);

  EXPECT_EQ(source.get<float>(), 0.0f);
  EXPECT_EQ(source.getSize(), 1u);
  EXPECT_EQ(source.getType(), &typeid(float));
};

TEST(UniformDataRecordMoveAssignment, HandlesSelfAssignment) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<float>(10.0f, 4);

  record = std::move(record);

  EXPECT_EQ(record.get<float>(), 10.0f);
  EXPECT_EQ(record.getSize(), 4u);
  EXPECT_EQ(record.getType(), &typeid(float));
};



// =============================
// ======= Data Control ========
// =============================
TEST(UniformDataRecordSet, StoresInt) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<int>(42);

  EXPECT_EQ(record.get<int>(), 42);
  EXPECT_EQ(record.getType(), &typeid(int));
  EXPECT_EQ(record.getSize(), 1u);
};

TEST(UniformDataRecordSet, StoresIVec2) {
  glm::ivec2 value = {1, 2};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::ivec2>(value);

  EXPECT_EQ(record.get<glm::ivec2>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::ivec2));
};

TEST(UniformDataRecordSet, StoresIVec3) {
  glm::ivec3 value = {1, 2, 3};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::ivec3>(value);

  EXPECT_EQ(record.get<glm::ivec3>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::ivec3));
};

TEST(UniformDataRecordSet, StoresIVec4) {
  glm::ivec4 value = {1, 2, 3, 4};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::ivec4>(value);

  EXPECT_EQ(record.get<glm::ivec4>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::ivec4));
};

TEST(UniformDataRecordSet, StoresFloat) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<float>(42.5f);

  EXPECT_EQ(record.get<float>(), 42.5f);
  EXPECT_EQ(record.getType(), &typeid(float));
  EXPECT_EQ(record.getSize(), 1u);
};

TEST(UniformDataRecordSet, StoresVec2) {
  glm::vec2 value = {1.5f, 2.5f};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::vec2>(value);

  EXPECT_EQ(record.get<glm::vec2>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::vec2));
};

TEST(UniformDataRecordSet, StoresVec3) {
  glm::vec3 value = {1.5f, 2.5f, 3.5f};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::vec3>(value);

  EXPECT_EQ(record.get<glm::vec3>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::vec3));
};

TEST(UniformDataRecordSet, StoresVec4) {
  glm::vec4 value = {1.5f, 2.5f, 3.5f, 4.5f};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::vec4>(value);

  EXPECT_EQ(record.get<glm::vec4>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::vec4));
};

TEST(UniformDataRecordSet, StoresDouble) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<double>(123.456);

  EXPECT_EQ(record.get<double>(), 123.456);
  EXPECT_EQ(record.getType(), &typeid(double));
  EXPECT_EQ(record.getSize(), 1u);
};

TEST(UniformDataRecordSet, StoresDVec2) {
  glm::dvec2 value = {1.5, 2.5};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::dvec2>(value);

  EXPECT_EQ(record.get<glm::dvec2>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::dvec2));
};

TEST(UniformDataRecordSet, StoresDVec3) {
  glm::dvec3 value = {1.5, 2.5, 3.5};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::dvec3>(value);

  EXPECT_EQ(record.get<glm::dvec3>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::dvec3));
};

TEST(UniformDataRecordSet, StoresDVec4) {
  glm::dvec4 value = {1.5, 2.5, 3.5, 4.5};

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::dvec4>(value);

  EXPECT_EQ(record.get<glm::dvec4>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::dvec4));
};

TEST(UniformDataRecordSet, StoresMat2) {
  glm::mat2 value = {
    1.0f, 2.0f,
    3.0f, 4.0f
  };

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::mat2>(value);

  EXPECT_EQ(record.get<glm::mat2>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::mat2));
};

TEST(UniformDataRecordSet, StoresMat3) {
  glm::mat3 value = {
    1.0f, 2.0f, 3.0f,
    4.0f, 5.0f, 6.0f,
    7.0f, 8.0f, 9.0f
  };

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::mat3>(value);

  EXPECT_EQ(record.get<glm::mat3>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::mat3));
};

TEST(UniformDataRecordSet, StoresMat4) {
  glm::mat4 value = {
    1.0f, 2.0f, 3.0f, 4.0f,
    5.0f, 6.0f, 7.0f, 8.0f,
    9.0f, 10.0f, 11.0f, 12.0f,
    13.0f, 14.0f, 15.0f, 16.0f
  };

  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::mat4>(value);

  EXPECT_EQ(record.get<glm::mat4>(), value);
  EXPECT_EQ(record.getType(), &typeid(glm::mat4));
};

TEST(UniformDataRecordSet, StoresSize) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<float>(5.5f, 128);

  EXPECT_EQ(record.get<float>(), 5.5f);
  EXPECT_EQ(record.getSize(), 128u);
};

TEST(UniformDataRecordSet, ReplacesExistingType) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<int>(10);

  EXPECT_EQ(record.get<int>(), 10);
  EXPECT_EQ(record.getType(), &typeid(int));

  record.set<glm::vec4>({1.0f, 2.0f, 3.0f, 4.0f});

  EXPECT_EQ(record.get<glm::vec4>(), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(record.getType(), &typeid(glm::vec4));
  EXPECT_TRUE(std::holds_alternative<glm::vec4>(record.getValue()));
};

TEST(UniformDataRecordSet, ReplacesExistingSize) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<float>(1.0f, 10);

  EXPECT_EQ(record.getSize(), 10u);

  record.set<float>(2.0f, 500);

  EXPECT_EQ(record.getSize(), 500u);
};

TEST(UniformDataRecordSet, SupportsZeroSize) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<int>(10, 0);

  EXPECT_EQ(record.get<int>(), 10);
  EXPECT_EQ(record.getSize(), 0u);
  EXPECT_EQ(record.getType(), &typeid(int));
};

TEST(UniformDataRecordSet, SupportsLargeSize) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<double>(100.0, 1000000);

  EXPECT_EQ(record.get<double>(), 100.0);
  EXPECT_EQ(record.getSize(), 1000000u);
};

TEST(UniformDataRecordGetValue, ReturnsStoredVariant) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<glm::ivec4>({1, 2, 3, 4});

  const auto& value = record.getValue();

  EXPECT_TRUE(std::holds_alternative<glm::ivec4>(value));
  EXPECT_EQ(std::get<glm::ivec4>(value), glm::ivec4(1, 2, 3, 4));
};

TEST(UniformDataRecordGetValue, ReturnsInternalVariantReference) {
  CW::Renderer::Shared::UniformDataRecord record;

  record.set<float>(25.0f);

  const auto& first = record.getValue();
  const auto& second = record.getValue();

  EXPECT_EQ(&first, &second);
};

TEST(UniformDataRecordGetType, ChangesWithStoredValue) {
  CW::Renderer::Shared::UniformDataRecord record;

  EXPECT_EQ(record.getType(), &typeid(float));

  record.set<int>(1);

  EXPECT_EQ(record.getType(), &typeid(int));

  record.set<glm::dvec3>({1.0, 2.0, 3.0});

  EXPECT_EQ(record.getType(), &typeid(glm::dvec3));

  record.set<glm::mat4>(glm::mat4(1.0f));

  EXPECT_EQ(record.getType(), &typeid(glm::mat4));
};

TEST(UniformDataRecordCopyChain, KeepsValuesIndependent) {
  CW::Renderer::Shared::UniformDataRecord first;

  first.set<glm::vec3>({1.0f, 2.0f, 3.0f}, 4);

  CW::Renderer::Shared::UniformDataRecord second(first);
  CW::Renderer::Shared::UniformDataRecord third(second);

  first.set<glm::vec3>({4.0f, 5.0f, 6.0f});
  second.set<glm::vec3>({7.0f, 8.0f, 9.0f});

  EXPECT_EQ(first.get<glm::vec3>(), glm::vec3(4.0f, 5.0f, 6.0f));
  EXPECT_EQ(second.get<glm::vec3>(), glm::vec3(7.0f, 8.0f, 9.0f));
  EXPECT_EQ(third.get<glm::vec3>(), glm::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(third.getSize(), 4u);
};

TEST(UniformDataRecordCopyMoveChain, PreservesValues) {
  glm::mat4 value = {
    1.0f, 2.0f, 3.0f, 4.0f,
    5.0f, 6.0f, 7.0f, 8.0f,
    9.0f, 10.0f, 11.0f, 12.0f,
    13.0f, 14.0f, 15.0f, 16.0f
  };

  CW::Renderer::Shared::UniformDataRecord original;

  original.set<glm::mat4>(value, 8);

  CW::Renderer::Shared::UniformDataRecord copy(original);
  CW::Renderer::Shared::UniformDataRecord moved(std::move(copy));

  EXPECT_EQ(original.get<glm::mat4>(), value);
  EXPECT_EQ(moved.get<glm::mat4>(), value);
  EXPECT_EQ(original.getSize(), 8u);
  EXPECT_EQ(moved.getSize(), 8u);

  EXPECT_EQ(copy.get<float>(), 0.0f);
  EXPECT_EQ(copy.getSize(), 1u);
  EXPECT_EQ(copy.getType(), &typeid(float));
};