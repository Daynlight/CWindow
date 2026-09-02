// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <gtest/gtest.h>
#include <utility>

#define private public
#define protected public

#include "CWindow/Renderer/Shared/Uniform/UniformData.h"

#undef private
#undef protected



// =============================
// ======= Constructors ========
// =============================
// core
TEST(UniformDataDefaultConstructor, InitializesEmptyData) {
  CW::Renderer::Shared::UniformData data;

  EXPECT_TRUE(data.getData().empty());
  EXPECT_EQ(data.getData().size(), 0u);
};


// copy
TEST(UniformDataCopyConstructor, CopiesValues) {
  CW::Renderer::Shared::UniformData original;

  original.at("first").set<int>(10);
  original.at("second").set<float>(20.5f);

  CW::Renderer::Shared::UniformData copy(original);

  EXPECT_EQ(copy.getData().size(), 2u);
  EXPECT_EQ(copy.at("first").get<int>(), 10);
  EXPECT_EQ(copy.at("second").get<float>(), 20.5f);
};

TEST(UniformDataCopyConstructor, CopiesRecordMetadata) {
  CW::Renderer::Shared::UniformData original;

  original.at("value").set<glm::vec3>({1.0f, 2.0f, 3.0f}, 5);

  CW::Renderer::Shared::UniformData copy(original);

  EXPECT_EQ(copy.at("value").get<glm::vec3>(), glm::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(copy.at("value").getSize(), 5u);
  EXPECT_EQ(copy.at("value").getType(), &typeid(glm::vec3));
};

TEST(UniformDataCopyConstructor, PerformsDeepCopy) {
  CW::Renderer::Shared::UniformData original;

  original.at("value").set<int>(10);

  CW::Renderer::Shared::UniformData copy(original);

  copy.at("value").set<int>(50);

  EXPECT_EQ(original.at("value").get<int>(), 10);
  EXPECT_EQ(copy.at("value").get<int>(), 50);
};

TEST(UniformDataCopyConstructor, CopiesEmptyData) {
  CW::Renderer::Shared::UniformData original;
  CW::Renderer::Shared::UniformData copy(original);

  EXPECT_TRUE(copy.getData().empty());
  EXPECT_EQ(copy.getData().size(), 0u);
};

TEST(UniformDataCopyConstructor, CopiesMultipleTypes) {
  CW::Renderer::Shared::UniformData original;

  original.at("int").set<int>(10);
  original.at("float").set<float>(20.5f);
  original.at("double").set<double>(30.5);
  original.at("vector").set<glm::vec4>({1.0f, 2.0f, 3.0f, 4.0f});
  original.at("matrix").set<glm::mat4>(glm::mat4(1.0f));

  CW::Renderer::Shared::UniformData copy(original);

  EXPECT_EQ(copy.at("int").get<int>(), 10);
  EXPECT_EQ(copy.at("float").get<float>(), 20.5f);
  EXPECT_EQ(copy.at("double").get<double>(), 30.5);
  EXPECT_EQ(copy.at("vector").get<glm::vec4>(), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(copy.at("matrix").get<glm::mat4>(), glm::mat4(1.0f));
};

TEST(UniformDataCopyAssignment, CopiesValues) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData target;

  source.at("first").set<int>(10);
  source.at("second").set<double>(20.5);

  target.at("old").set<float>(50.0f);

  target = source;

  EXPECT_EQ(target.getData().size(), 2u);
  EXPECT_EQ(target.at("first").get<int>(), 10);
  EXPECT_EQ(target.at("second").get<double>(), 20.5);
};

TEST(UniformDataCopyAssignment, ReplacesPreviousData) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData target;

  source.at("new").set<int>(100);

  target.at("old1").set<int>(1);
  target.at("old2").set<int>(2);
  target.at("old3").set<int>(3);

  target = source;

  EXPECT_EQ(target.getData().size(), 1u);
  EXPECT_TRUE(target.getData().contains("new"));
  EXPECT_FALSE(target.getData().contains("old1"));
  EXPECT_FALSE(target.getData().contains("old2"));
  EXPECT_FALSE(target.getData().contains("old3"));
};

TEST(UniformDataCopyAssignment, PerformsDeepCopy) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData target;

  source.at("value").set<glm::ivec4>({1, 2, 3, 4});

  target = source;

  target.at("value").set<glm::ivec4>({5, 6, 7, 8});

  EXPECT_EQ(source.at("value").get<glm::ivec4>(), glm::ivec4(1, 2, 3, 4));
  EXPECT_EQ(target.at("value").get<glm::ivec4>(), glm::ivec4(5, 6, 7, 8));
};

TEST(UniformDataCopyAssignment, HandlesSelfAssignment) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(10);
  data.at("second").set<float>(20.0f);

  data = data;

  EXPECT_EQ(data.getData().size(), 2u);
  EXPECT_EQ(data.at("first").get<int>(), 10);
  EXPECT_EQ(data.at("second").get<float>(), 20.0f);
};


// move
TEST(UniformDataMoveConstructor, TransfersValues) {
  CW::Renderer::Shared::UniformData source;

  source.at("first").set<int>(10);
  source.at("second").set<float>(20.5f);

  CW::Renderer::Shared::UniformData moved(std::move(source));

  EXPECT_EQ(moved.getData().size(), 2u);
  EXPECT_EQ(moved.at("first").get<int>(), 10);
  EXPECT_EQ(moved.at("second").get<float>(), 20.5f);
};

TEST(UniformDataMoveConstructor, TransfersMultipleTypes) {
  CW::Renderer::Shared::UniformData source;

  source.at("int").set<int>(10);
  source.at("vector").set<glm::dvec3>({1.0, 2.0, 3.0});
  source.at("matrix").set<glm::mat3>(glm::mat3(1.0f));

  CW::Renderer::Shared::UniformData moved(std::move(source));

  EXPECT_EQ(moved.at("int").get<int>(), 10);
  EXPECT_EQ(moved.at("vector").get<glm::dvec3>(), glm::dvec3(1.0, 2.0, 3.0));
  EXPECT_EQ(moved.at("matrix").get<glm::mat3>(), glm::mat3(1.0f));
};

TEST(UniformDataMoveConstructor, HandlesEmptyData) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData moved(std::move(source));

  EXPECT_TRUE(moved.getData().empty());
};

TEST(UniformDataMoveConstructor, LeavesSourceUsable) {
  CW::Renderer::Shared::UniformData source;

  source.at("value").set<int>(10);

  CW::Renderer::Shared::UniformData moved(std::move(source));

  source.at("new").set<int>(20);

  EXPECT_EQ(source.at("new").get<int>(), 20);
  EXPECT_EQ(moved.at("value").get<int>(), 10);
};

TEST(UniformDataMoveAssignment, TransfersValues) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData target;

  source.at("first").set<int>(10);
  source.at("second").set<float>(20.5f);

  target.at("old").set<double>(100.0);

  target = std::move(source);

  EXPECT_EQ(target.getData().size(), 2u);
  EXPECT_EQ(target.at("first").get<int>(), 10);
  EXPECT_EQ(target.at("second").get<float>(), 20.5f);
};

TEST(UniformDataMoveAssignment, ReplacesPreviousData) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData target;

  source.at("new").set<glm::vec3>({1.0f, 2.0f, 3.0f});

  target.at("old1").set<int>(10);
  target.at("old2").set<int>(20);

  target = std::move(source);

  EXPECT_EQ(target.getData().size(), 1u);
  EXPECT_TRUE(target.getData().contains("new"));
  EXPECT_FALSE(target.getData().contains("old1"));
  EXPECT_FALSE(target.getData().contains("old2"));
  EXPECT_EQ(target.at("new").get<glm::vec3>(), glm::vec3(1.0f, 2.0f, 3.0f));
};

TEST(UniformDataMoveAssignment, LeavesSourceUsable) {
  CW::Renderer::Shared::UniformData source;
  CW::Renderer::Shared::UniformData target;

  source.at("value").set<int>(10);

  target = std::move(source);

  source.at("new").set<int>(20);

  EXPECT_EQ(source.at("new").get<int>(), 20);
  EXPECT_EQ(target.at("value").get<int>(), 10);
};

TEST(UniformDataMoveAssignment, HandlesSelfAssignment) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(10);
  data.at("second").set<float>(20.0f);

  data = std::move(data);

  EXPECT_EQ(data.getData().size(), 2u);
  EXPECT_EQ(data.at("first").get<int>(), 10);
  EXPECT_EQ(data.at("second").get<float>(), 20.0f);
};



// =============================
// ======= Data Control ========
// =============================
TEST(UniformDataAt, CreatesMissingRecord) {
  CW::Renderer::Shared::UniformData data;

  CW::Renderer::Shared::UniformDataRecord& record = data.at("value");

  EXPECT_EQ(data.getData().size(), 1u);
  EXPECT_TRUE(data.getData().contains("value"));
  EXPECT_EQ(record.get<float>(), 0.0f);
  EXPECT_EQ(record.getSize(), 1u);
  EXPECT_EQ(record.getType(), &typeid(float));
};

TEST(UniformDataAt, ReturnsExistingRecord) {
  CW::Renderer::Shared::UniformData data;

  data.at("value").set<int>(42);

  CW::Renderer::Shared::UniformDataRecord& first = data.at("value");
  CW::Renderer::Shared::UniformDataRecord& second = data.at("value");

  EXPECT_EQ(&first, &second);
  EXPECT_EQ(second.get<int>(), 42);
  EXPECT_EQ(data.getData().size(), 1u);
};

TEST(UniformDataAt, DoesNotOverwriteExistingRecord) {
  CW::Renderer::Shared::UniformData data;

  data.at("value").set<glm::vec4>({1.0f, 2.0f, 3.0f, 4.0f}, 5);

  data.at("value");

  EXPECT_EQ(data.at("value").get<glm::vec4>(), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(data.at("value").getSize(), 5u);
  EXPECT_EQ(data.at("value").getType(), &typeid(glm::vec4));
};

TEST(UniformDataAt, HandlesEmptyName) {
  CW::Renderer::Shared::UniformData data;

  data.at("").set<int>(10);

  EXPECT_TRUE(data.getData().contains(""));
  EXPECT_EQ(data.at("").get<int>(), 10);
};

TEST(UniformDataAt, HandlesMultipleRecords) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(1);
  data.at("second").set<int>(2);
  data.at("third").set<int>(3);

  EXPECT_EQ(data.getData().size(), 3u);
  EXPECT_EQ(data.at("first").get<int>(), 1);
  EXPECT_EQ(data.at("second").get<int>(), 2);
  EXPECT_EQ(data.at("third").get<int>(), 3);
};

TEST(UniformDataOperatorIndex, CreatesMissingRecord) {
  CW::Renderer::Shared::UniformData data;

  data["value"].set<float>(42.5f);

  EXPECT_EQ(data.getData().size(), 1u);
  EXPECT_TRUE(data.getData().contains("value"));
  EXPECT_EQ(data["value"].get<float>(), 42.5f);
};

TEST(UniformDataOperatorIndex, ReturnsExistingRecord) {
  CW::Renderer::Shared::UniformData data;

  data["value"].set<double>(123.456);

  CW::Renderer::Shared::UniformDataRecord& first = data["value"];
  CW::Renderer::Shared::UniformDataRecord& second = data["value"];

  EXPECT_EQ(&first, &second);
  EXPECT_EQ(second.get<double>(), 123.456);
  EXPECT_EQ(data.getData().size(), 1u);
};

TEST(UniformDataOperatorIndex, MatchesAtBehavior) {
  CW::Renderer::Shared::UniformData data;

  data.at("value").set<int>(100);

  EXPECT_EQ(&data.at("value"), &data["value"]);
  EXPECT_EQ(data["value"].get<int>(), 100);
};

TEST(UniformDataRemove, RemovesExistingRecord) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(10);
  data.at("second").set<int>(20);

  data.remove("first");

  EXPECT_EQ(data.getData().size(), 1u);
  EXPECT_FALSE(data.getData().contains("first"));
  EXPECT_TRUE(data.getData().contains("second"));
  EXPECT_EQ(data.at("second").get<int>(), 20);
};

TEST(UniformDataRemove, HandlesMissingRecord) {
  CW::Renderer::Shared::UniformData data;

  data.at("value").set<int>(10);

  data.remove("missing");

  EXPECT_EQ(data.getData().size(), 1u);
  EXPECT_TRUE(data.getData().contains("value"));
  EXPECT_EQ(data.at("value").get<int>(), 10);
};

TEST(UniformDataRemove, HandlesEmptyData) {
  CW::Renderer::Shared::UniformData data;

  data.remove("missing");

  EXPECT_TRUE(data.getData().empty());
};

TEST(UniformDataRemove, CanRemoveAllRecords) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(1);
  data.at("second").set<int>(2);
  data.at("third").set<int>(3);

  data.remove("first");
  data.remove("second");
  data.remove("third");

  EXPECT_TRUE(data.getData().empty());
};

TEST(UniformDataRemove, RemovedNameCanBeRecreated) {
  CW::Renderer::Shared::UniformData data;

  data.at("value").set<int>(10);

  data.remove("value");

  data.at("value").set<float>(20.5f);

  EXPECT_EQ(data.getData().size(), 1u);
  EXPECT_EQ(data.at("value").get<float>(), 20.5f);
  EXPECT_EQ(data.at("value").getType(), &typeid(float));
};

TEST(UniformDataClear, RemovesAllRecords) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(1);
  data.at("second").set<float>(2.0f);
  data.at("third").set<double>(3.0);

  data.clear();

  EXPECT_TRUE(data.getData().empty());
  EXPECT_EQ(data.getData().size(), 0u);
};

TEST(UniformDataClear, HandlesEmptyData) {
  CW::Renderer::Shared::UniformData data;

  data.clear();

  EXPECT_TRUE(data.getData().empty());
};

TEST(UniformDataClear, CanBeReusedAfterClear) {
  CW::Renderer::Shared::UniformData data;

  data.at("old").set<int>(10);

  data.clear();

  data.at("new").set<int>(20);

  EXPECT_EQ(data.getData().size(), 1u);
  EXPECT_FALSE(data.getData().contains("old"));
  EXPECT_TRUE(data.getData().contains("new"));
  EXPECT_EQ(data.at("new").get<int>(), 20);
};

TEST(UniformDataGetData, ReturnsInternalData) {
  CW::Renderer::Shared::UniformData data;

  data.at("first").set<int>(10);
  data.at("second").set<float>(20.5f);

  const auto& stored = data.getData();

  EXPECT_EQ(stored.size(), 2u);
  EXPECT_TRUE(stored.contains("first"));
  EXPECT_TRUE(stored.contains("second"));
  EXPECT_EQ(stored.at("first").get<int>(), 10);
  EXPECT_EQ(stored.at("second").get<float>(), 20.5f);
};

TEST(UniformDataGetData, ReturnsInternalMapReference) {
  CW::Renderer::Shared::UniformData data;

  const auto& first = data.getData();
  const auto& second = data.getData();

  EXPECT_EQ(&first, &second);
};

TEST(UniformDataCopyChain, KeepsDataIndependent) {
  CW::Renderer::Shared::UniformData first;

  first.at("value").set<int>(10);

  CW::Renderer::Shared::UniformData second(first);
  CW::Renderer::Shared::UniformData third(second);

  first.at("value").set<int>(20);
  second.at("value").set<int>(30);

  EXPECT_EQ(first.at("value").get<int>(), 20);
  EXPECT_EQ(second.at("value").get<int>(), 30);
  EXPECT_EQ(third.at("value").get<int>(), 10);
};

TEST(UniformDataCopyMoveChain, PreservesValues) {
  CW::Renderer::Shared::UniformData original;

  original.at("first").set<glm::vec4>({1.0f, 2.0f, 3.0f, 4.0f}, 4);
  original.at("second").set<glm::mat4>(glm::mat4(1.0f));

  CW::Renderer::Shared::UniformData copy(original);
  CW::Renderer::Shared::UniformData moved(std::move(copy));

  EXPECT_EQ(original.at("first").get<glm::vec4>(), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(moved.at("first").get<glm::vec4>(), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
  EXPECT_EQ(original.at("second").get<glm::mat4>(), glm::mat4(1.0f));
  EXPECT_EQ(moved.at("second").get<glm::mat4>(), glm::mat4(1.0f));
  EXPECT_EQ(original.at("first").getSize(), 4u);
  EXPECT_EQ(moved.at("first").getSize(), 4u);
};