#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"

#include "../Material/PBRMaterial.h"

#include <vector>







namespace CW{
class MeshLoader{
  public: 
  std::vector<GLfloat> vertices;
  std::vector<unsigned int> indices;
  std::vector<GLfloat> normals;
  std::vector<GLfloat> uvs;
  std::vector<GLfloat> tangents;
  std::vector<GLfloat> bitangents;
  std::vector<GLfloat> colors;
  CW::PBRMaterial material;

  Assimp::Importer importer;

  void LoadModel(const std::string& path){

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace
    );

    if (!scene || !scene->mRootNode){
      printf("Can't open file %s\b", path);
      return;
    };

    aiMesh* mesh = scene->mMeshes[0];

    extractVertices(mesh);
    extractIndices(mesh);
    extractNormals(mesh);
    extractUVs(mesh);
    extractTangents(mesh);
    extractColors(mesh);
    extractMaterial(mesh, scene);
  };

  void extractVertices(aiMesh* mesh){
    vertices.clear();
    vertices.reserve(mesh->mNumVertices * 4);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++){
      const aiVector3D& v = mesh->mVertices[i];
      vertices.push_back(v.x);
      vertices.push_back(v.y);
      vertices.push_back(v.z);
      vertices.push_back(1.0);
    }
  }

  void extractIndices(aiMesh* mesh){
    indices.clear();
    indices.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++){
      const aiFace& face = mesh->mFaces[i];

      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
  }

  void extractNormals(aiMesh* mesh){
    normals.clear();
    normals.reserve(mesh->mNumVertices * 3);

    if (mesh->HasNormals()){
      for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        const aiVector3D& n = mesh->mNormals[i];

        normals.push_back(n.x);
        normals.push_back(n.y);
        normals.push_back(n.z);
      }
    }
    else{
      for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
      }
    }
  }

  void extractUVs(aiMesh* mesh){
    uvs.clear();

    if (mesh->HasTextureCoords(0)){
      uvs.reserve(mesh->mNumVertices * 2);

      for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        const aiVector3D& uv = mesh->mTextureCoords[0][i];

        uvs.push_back(uv.x);
        uvs.push_back(uv.y);
      }
    }
  }

  void extractTangents(aiMesh* mesh){
    tangents.clear();
    bitangents.clear();

    if (mesh->HasTangentsAndBitangents()){
      tangents.reserve(mesh->mNumVertices * 3);
      bitangents.reserve(mesh->mNumVertices * 3);

      for (unsigned int i = 0; i < mesh->mNumVertices; i++)
      {
        const aiVector3D& t = mesh->mTangents[i];
        const aiVector3D& b = mesh->mBitangents[i];

        tangents.insert(tangents.end(), {t.x, t.y, t.z});
        bitangents.insert(bitangents.end(), {b.x, b.y, b.z});
      }
    }
  }

  void extractColors(aiMesh* mesh){
    colors.clear();

    if (mesh->HasVertexColors(0)){
      colors.reserve(mesh->mNumVertices * 3);

      for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        const aiColor4D& c = mesh->mColors[0][i];

        colors.push_back(c.r);
        colors.push_back(c.g);
        colors.push_back(c.b);
      }
    }
  }

  void extractMaterial(aiMesh* mesh, const aiScene* scene)
  {
    if (!scene || !mesh) return;

    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    // ---------------- BASE COLOR ----------------
    aiColor3D baseColor(1.0f, 1.0f, 1.0f);
    material.albedo = glm::vec3(1.0f);
    if (mat->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS ||
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS)
    {
      material.albedo = glm::vec3(baseColor.r, baseColor.g, baseColor.b);
    }

    // ---------------- ROUGHNESS ----------------
    float roughness = 1.0f;
    material.roughness = roughness;
    if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
    {
      material.roughness = roughness;
    }

    // ---------------- METALLIC ----------------
    float metallic = 0.0f;
    material.metallic = metallic;
    if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
    {
      material.metallic = metallic;
    }

    // ---------------- EMISSIVE (REAL ONE) ----------------
    aiColor3D emi_color(0.0f, 0.0f, 0.0f);
    material.emission_color = glm::vec3(0.0f);
    if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, emi_color) == AI_SUCCESS)
    {
      material.emission_color = glm::vec3(emi_color.r, emi_color.g, emi_color.b);
    }

    float emi_strength = 1.0f;
    material.emission_strength = emi_strength;
    if (mat->Get(AI_MATKEY_EMISSIVE_INTENSITY, emi_strength) == AI_SUCCESS)
    {
      material.emission_strength = emi_strength;
    }

    // ---------------- AO (fallback) ----------------
    material.ambient_occlusion = 1.0f;
  }
  
  void bindMaterialToUniform(CW::Renderer::Uniform& uniform){
    uniform["albedo"]->set<glm::vec3>(material.albedo);
    uniform["roughness"]->set<float>(material.roughness);
    uniform["metallic"]->set<float>(material.metallic);
    uniform["emission_color"]->set<glm::vec3>(material.emission_color);
    uniform["emission_strength"]->set<float>(material.emission_strength);
    uniform["ambient_occlusion"]->set<float>(material.ambient_occlusion);
  };

  void clear(){
    vertices.clear();
    indices.clear();
    normals.clear();
    uvs.clear();
    tangents.clear();
    bitangents.clear();
    colors.clear();
    material = CW::PBRMaterial();
  }
};

};