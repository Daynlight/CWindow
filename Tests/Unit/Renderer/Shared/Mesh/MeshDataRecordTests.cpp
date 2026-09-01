// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <utility>

#define private public
#define protected public

#include "Renderer/Shared/Mesh/MeshDataRecord.h"

#undef private
#undef protected



// =============================
// ======= Constructors ========
// =============================
// core
TEST(MeshDataRecordDefaultConstructor, InitializesDefaultValues) {
  CW::Renderer::Shared::MeshDataRecord record;

  EXPECT_TRUE(record.data.empty());
  EXPECT_EQ(record.getSize(), 0u);
  EXPECT_EQ(record.getSizeOfElement(), 0u);
  EXPECT_EQ(record.getDimension(), 0u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Float);
};

TEST(MeshDataRecordConstructor, InitializesValues) {
  std::vector<char> data = {1, 2, 3, 4, 5, 6, 7, 8};

  CW::Renderer::Shared::MeshDataRecord record(data, 2, sizeof(float), 4, CW::Renderer::Shared::MeshDataType::Float);

  EXPECT_EQ(record.getSize(), 2u);
  EXPECT_EQ(record.getSizeOfElement(), sizeof(float));
  EXPECT_EQ(record.getDimension(), 4u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Float);
  EXPECT_EQ(record.data.size(), 8u);
  EXPECT_THAT(record.data, ::testing::ElementsAre(1, 2, 3, 4, 5, 6, 7, 8));
};

TEST(MeshDataRecordConstructor, TransfersDataOwnership) {
  std::vector<char> data = {1, 2, 3, 4};
  const char* ptr = data.data();

  CW::Renderer::Shared::MeshDataRecord record(data, 4, sizeof(char), 1, CW::Renderer::Shared::MeshDataType::Byte);

  EXPECT_EQ(record.data.data(), ptr);
  EXPECT_THAT(record.data, ::testing::ElementsAre(1, 2, 3, 4));
};

TEST(MeshDataRecordConstructor, HandlesEmptyData) {
  std::vector<char> data;

  CW::Renderer::Shared::MeshDataRecord record(data, 0, sizeof(float), 3, CW::Renderer::Shared::MeshDataType::Float);

  EXPECT_TRUE(record.data.empty());
  EXPECT_EQ(record.getSize(), 0u);
  EXPECT_EQ(record.getSizeOfElement(), sizeof(float));
  EXPECT_EQ(record.getDimension(), 3u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Float);
};

TEST(MeshDataRecordConstructor, HandlesLargeData) {
  std::vector<char> data(1024 * 1024, 42);
  const char* ptr = data.data();

  CW::Renderer::Shared::MeshDataRecord record(data, data.size(), sizeof(char), 1, CW::Renderer::Shared::MeshDataType::Byte);

  ASSERT_EQ(record.data.size(), 1024u * 1024u);
  EXPECT_EQ(record.data.data(), ptr);
  EXPECT_EQ(record.data.front(), 42);
  EXPECT_EQ(record.data.back(), 42);
};

// copy
TEST(MeshDataRecordCopyConstructor, CopiesValues) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord original(data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord copy(original);

  EXPECT_EQ(copy.data, original.data);
  EXPECT_EQ(copy.getSize(), original.getSize());
  EXPECT_EQ(copy.getSizeOfElement(), original.getSizeOfElement());
  EXPECT_EQ(copy.getDimension(), original.getDimension());
  EXPECT_EQ(copy.getType(), original.getType());
};

TEST(MeshDataRecordCopyConstructor, PerformsDeepCopy) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord original(data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord copy(original);

  ASSERT_FALSE(original.data.empty());
  ASSERT_FALSE(copy.data.empty());

  EXPECT_NE(original.data.data(), copy.data.data());
  EXPECT_EQ(original.data, copy.data);

  copy.data[0] = 99;
  copy.data[3] = 88;

  EXPECT_EQ(original.data[0], 1);
  EXPECT_EQ(original.data[3], 4);
  EXPECT_EQ(copy.data[0], 99);
  EXPECT_EQ(copy.data[3], 88);
};

TEST(MeshDataRecordCopyConstructor, CopiesEmptyRecord) {
  CW::Renderer::Shared::MeshDataRecord original;
  CW::Renderer::Shared::MeshDataRecord copy(original);

  EXPECT_TRUE(copy.data.empty());
  EXPECT_EQ(copy.getSize(), 0u);
  EXPECT_EQ(copy.getSizeOfElement(), 0u);
  EXPECT_EQ(copy.getDimension(), 0u);
  EXPECT_EQ(copy.getType(), CW::Renderer::Shared::MeshDataType::Float);
};

TEST(MeshDataRecordCopyConstructor, CopiesLargeRecordIndependently) {
  std::vector<char> data(100000, 12);

  CW::Renderer::Shared::MeshDataRecord original(data, 100000, 1, 1, CW::Renderer::Shared::MeshDataType::UByte);
  CW::Renderer::Shared::MeshDataRecord copy(original);

  ASSERT_EQ(copy.data.size(), original.data.size());

  EXPECT_NE(copy.data.data(), original.data.data());
  EXPECT_EQ(copy.data, original.data);

  copy.data[50000] = 100;

  EXPECT_EQ(original.data[50000], 12);
  EXPECT_EQ(copy.data[50000], 100);
};

TEST(MeshDataRecordCopyAssignment, CopiesValues) {
  std::vector<char> first_data = {1, 2, 3, 4};
  std::vector<char> second_data = {9, 8};

  CW::Renderer::Shared::MeshDataRecord source(first_data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord target(second_data, 2, 4, 1, CW::Renderer::Shared::MeshDataType::UInt);

  target = source;

  EXPECT_EQ(target.data, source.data);
  EXPECT_EQ(target.getSize(), source.getSize());
  EXPECT_EQ(target.getSizeOfElement(), source.getSizeOfElement());
  EXPECT_EQ(target.getDimension(), source.getDimension());
  EXPECT_EQ(target.getType(), source.getType());
};

TEST(MeshDataRecordCopyAssignment, PerformsDeepCopy) {
  std::vector<char> first_data = {1, 2, 3, 4};
  std::vector<char> second_data = {9, 8};

  CW::Renderer::Shared::MeshDataRecord source(first_data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord target(second_data, 2, 4, 1, CW::Renderer::Shared::MeshDataType::UInt);

  target = source;

  ASSERT_FALSE(source.data.empty());
  ASSERT_FALSE(target.data.empty());

  EXPECT_NE(target.data.data(), source.data.data());

  target.data[1] = 55;

  EXPECT_EQ(source.data[1], 2);
  EXPECT_EQ(target.data[1], 55);
};

TEST(MeshDataRecordCopyAssignment, ReplacesPreviousData) {
  std::vector<char> first_data = {1, 2, 3, 4, 5, 6};
  std::vector<char> second_data = {9};

  CW::Renderer::Shared::MeshDataRecord source(first_data, 6, 1, 3, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord target(second_data, 1, 4, 1, CW::Renderer::Shared::MeshDataType::UInt);

  target = source;

  EXPECT_EQ(target.data.size(), 6u);
  EXPECT_THAT(target.data, ::testing::ElementsAre(1, 2, 3, 4, 5, 6));
  EXPECT_EQ(target.getSize(), 6u);
  EXPECT_EQ(target.getSizeOfElement(), 1u);
  EXPECT_EQ(target.getDimension(), 3u);
  EXPECT_EQ(target.getType(), CW::Renderer::Shared::MeshDataType::Byte);
};

TEST(MeshDataRecordCopyAssignment, HandlesSelfAssignment) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord record(data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);

  const char* ptr = record.data.data();

  record = record;

  EXPECT_EQ(record.data.data(), ptr);
  EXPECT_THAT(record.data, ::testing::ElementsAre(1, 2, 3, 4));
  EXPECT_EQ(record.getSize(), 4u);
  EXPECT_EQ(record.getSizeOfElement(), 1u);
  EXPECT_EQ(record.getDimension(), 2u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Byte);
};

// move
TEST(MeshDataRecordMoveConstructor, TransfersValues) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord source(data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);

  const char* ptr = source.data.data();

  CW::Renderer::Shared::MeshDataRecord moved(std::move(source));

  EXPECT_EQ(moved.data.data(), ptr);
  EXPECT_THAT(moved.data, ::testing::ElementsAre(1, 2, 3, 4));
  EXPECT_EQ(moved.getSize(), 4u);
  EXPECT_EQ(moved.getSizeOfElement(), 1u);
  EXPECT_EQ(moved.getDimension(), 2u);
  EXPECT_EQ(moved.getType(), CW::Renderer::Shared::MeshDataType::Byte);
};

TEST(MeshDataRecordMoveConstructor, TransfersLargeBufferWithoutCopy) {
  std::vector<char> data(100000, 10);

  CW::Renderer::Shared::MeshDataRecord source(data, 100000, 1, 1, CW::Renderer::Shared::MeshDataType::UByte);

  const char* ptr = source.data.data();

  CW::Renderer::Shared::MeshDataRecord moved(std::move(source));

  ASSERT_EQ(moved.data.size(), 100000u);
  EXPECT_EQ(moved.data.data(), ptr);
  EXPECT_EQ(moved.data.front(), 10);
  EXPECT_EQ(moved.data.back(), 10);
};

TEST(MeshDataRecordMoveConstructor, HandlesEmptyRecord) {
  CW::Renderer::Shared::MeshDataRecord source;
  CW::Renderer::Shared::MeshDataRecord moved(std::move(source));

  EXPECT_TRUE(moved.data.empty());
  EXPECT_EQ(moved.getSize(), 0u);
  EXPECT_EQ(moved.getSizeOfElement(), 0u);
  EXPECT_EQ(moved.getDimension(), 0u);
  EXPECT_EQ(moved.getType(), CW::Renderer::Shared::MeshDataType::Float);
};

TEST(MeshDataRecordMoveAssignment, TransfersValues) {
  std::vector<char> source_data = {1, 2, 3, 4};
  std::vector<char> target_data = {9, 9};

  CW::Renderer::Shared::MeshDataRecord source(source_data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord target(target_data, 2, 4, 1, CW::Renderer::Shared::MeshDataType::UInt);

  const char* ptr = source.data.data();

  target = std::move(source);

  EXPECT_EQ(target.data.data(), ptr);
  EXPECT_THAT(target.data, ::testing::ElementsAre(1, 2, 3, 4));
  EXPECT_EQ(target.getSize(), 4u);
  EXPECT_EQ(target.getSizeOfElement(), 1u);
  EXPECT_EQ(target.getDimension(), 2u);
  EXPECT_EQ(target.getType(), CW::Renderer::Shared::MeshDataType::Byte);
};

TEST(MeshDataRecordMoveAssignment, ReplacesPreviousBuffer) {
  std::vector<char> source_data = {1, 2, 3, 4, 5};
  std::vector<char> target_data = {9, 8, 7};

  CW::Renderer::Shared::MeshDataRecord source(source_data, 5, 1, 5, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord target(target_data, 3, 4, 1, CW::Renderer::Shared::MeshDataType::UInt);

  const char* ptr = source.data.data();

  target = std::move(source);

  EXPECT_EQ(target.data.data(), ptr);
  EXPECT_EQ(target.data.size(), 5u);
  EXPECT_THAT(target.data, ::testing::ElementsAre(1, 2, 3, 4, 5));
  EXPECT_EQ(target.getSize(), 5u);
  EXPECT_EQ(target.getDimension(), 5u);
  EXPECT_EQ(target.getType(), CW::Renderer::Shared::MeshDataType::Byte);
};

TEST(MeshDataRecordMoveAssignment, HandlesSelfAssignment) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord record(data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);

  const char* ptr = record.data.data();

  record = std::move(record);

  EXPECT_EQ(record.data.data(), ptr);
  EXPECT_THAT(record.data, ::testing::ElementsAre(1, 2, 3, 4));
  EXPECT_EQ(record.getSize(), 4u);
  EXPECT_EQ(record.getSizeOfElement(), 1u);
  EXPECT_EQ(record.getDimension(), 2u);
  EXPECT_EQ(record.getType(), CW::Renderer::Shared::MeshDataType::Byte);
};



// =============================
// ======= Data Control ========
// =============================
TEST(MeshDataRecordGetSize, ReturnsStoredSize) {
  std::vector<char> data = {1, 2, 3, 4, 5, 6};

  CW::Renderer::Shared::MeshDataRecord record(data, 6, 1, 3, CW::Renderer::Shared::MeshDataType::Byte);

  EXPECT_EQ(record.getSize(), 6u);
};

TEST(MeshDataRecordGetSizeOfElement, ReturnsStoredSizeOfElement) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord record(data, 1, sizeof(float), 4, CW::Renderer::Shared::MeshDataType::Float);

  EXPECT_EQ(record.getSizeOfElement(), sizeof(float));
};

TEST(MeshDataRecordGetDimension, ReturnsStoredDimension) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord record(data, 1, sizeof(float), 4, CW::Renderer::Shared::MeshDataType::Float);

  EXPECT_EQ(record.getDimension(), 4u);
};

TEST(MeshDataRecordGetRawData, ReturnsInternalBuffer) {
  std::vector<char> data = {11, 22, 33, 44};

  CW::Renderer::Shared::MeshDataRecord record(data, 4, sizeof(char), 1, CW::Renderer::Shared::MeshDataType::Byte);

  const char* raw = record.getRawData();

  ASSERT_NE(raw, nullptr);

  EXPECT_EQ(raw, record.data.data());
  EXPECT_EQ(raw[0], 11);
  EXPECT_EQ(raw[1], 22);
  EXPECT_EQ(raw[2], 33);
  EXPECT_EQ(raw[3], 44);
};

TEST(MeshDataRecordGetRawData, ReflectsStoredBuffer) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord record(data, 4, 1, 1, CW::Renderer::Shared::MeshDataType::Byte);

  const char* raw = record.getRawData();

  record.data[0] = 50;
  record.data[3] = 100;

  EXPECT_EQ(raw[0], 50);
  EXPECT_EQ(raw[3], 100);
};

TEST(MeshDataRecordGetType, ReturnsAllSupportedTypes) {
  using Type = CW::Renderer::Shared::MeshDataType;

  const Type types[] = {
    Type::Float,
    Type::Int,
    Type::UInt,
    Type::Byte,
    Type::UByte
  };

  for(const Type type : types) {
    std::vector<char> data = {1};

    CW::Renderer::Shared::MeshDataRecord record(data, 1, 1, 1, type);

    EXPECT_EQ(record.getType(), type);
  };
};

TEST(MeshDataRecordCopyChain, KeepsBuffersIndependent) {
  std::vector<char> data = {1, 2, 3, 4};

  CW::Renderer::Shared::MeshDataRecord first(data, 4, 1, 2, CW::Renderer::Shared::MeshDataType::Byte);
  CW::Renderer::Shared::MeshDataRecord second(first);
  CW::Renderer::Shared::MeshDataRecord third(second);

  EXPECT_NE(first.data.data(), second.data.data());
  EXPECT_NE(first.data.data(), third.data.data());
  EXPECT_NE(second.data.data(), third.data.data());

  third.data[0] = 100;

  EXPECT_EQ(first.data[0], 1);
  EXPECT_EQ(second.data[0], 1);
  EXPECT_EQ(third.data[0], 100);
};

TEST(MeshDataRecordCopyMoveChain, PreservesValuesAndOwnership) {
  std::vector<char> data = {1, 2, 3, 4, 5, 6};

  CW::Renderer::Shared::MeshDataRecord original(data, 6, 2, 3, CW::Renderer::Shared::MeshDataType::UInt);
  CW::Renderer::Shared::MeshDataRecord copy(original);

  const char* copy_ptr = copy.data.data();

  CW::Renderer::Shared::MeshDataRecord moved(std::move(copy));

  EXPECT_EQ(moved.data.data(), copy_ptr);
  EXPECT_NE(moved.data.data(), original.data.data());
  EXPECT_EQ(moved.data, original.data);
  EXPECT_EQ(moved.getSize(), original.getSize());
  EXPECT_EQ(moved.getSizeOfElement(), original.getSizeOfElement());
  EXPECT_EQ(moved.getDimension(), original.getDimension());
  EXPECT_EQ(moved.getType(), original.getType());
};
