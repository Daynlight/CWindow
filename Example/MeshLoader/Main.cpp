#include "Renderer.h"
#include "Material/PBRMaterial.h"
#include "Shaders.h"

#include "FreeCamera/FreeCamera3D.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/euler_angles.hpp"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <iostream>
#include <unordered_map>



class Model{
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
      std::cout << "Assimp Error: " 
                << importer.GetErrorString() 
                << std::endl;
      throw errno;
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



int main(){
  CW::Renderer::Renderer window;
  window.setWindowTitle("Mesh Creation and Loading");
  CW::Renderer::FreeCamera3D camera(&window);
  window.setCursorVisibility(false);



  CW::Renderer::Shader shader(Shader::vertex, Shader::fragment);
  CW::Renderer::Uniform uniform;

  shader.getUniforms().emplace_back(&uniform);
  
  Model data = Model(); 
  data.LoadModel("../Example/MeshLoader/asset.stl");

  CW::Renderer::Mesh asset;
  asset.addVertices(data.vertices, 4, 0);
  asset.addIndices(data.indices);
  asset.setData<GLfloat>(data.normals, 3, 1, GL_FLOAT);
  // asset.setData<GLfloat>(data.colors, 3, 2, GL_FLOAT);

  data.bindMaterialToUniform(uniform);

  uniform["lightPos"]->set<glm::vec3>({50.0f, 100.0f, 20.0f});
  uniform["lightColor"]->set<glm::vec3>({1.0f, 1.0f, 1.0f});

  float time = 0.0f;
  float cursor_visible_lock = 0.0f;
  bool cursor_lock = true;

  while(!window.getWindowData()->should_close){
    window.beginFrame();

    // time += window.getWindowData()->delta_time;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
    model = model * glm::eulerAngleXYZ(-3.14f / 2.0f, 0.0f, 3.14f);
    model = glm::scale(model, glm::vec3(0.1f));

    glm::mat4 mvp = camera.transformation(&window) * model;

    uniform["transformation"]->set<glm::mat4>(mvp);
    uniform["model"]->set<glm::mat4>(model);


    if(cursor_lock) window.setCursorOn(true);
    else window.setCursorOn(false);

    if(window.getInputData()->is_key_down("ESC") && cursor_visible_lock <= 0.0f) {
      cursor_lock = !cursor_lock;
      cursor_visible_lock = 0.5f;
      camera.resetMouse();
    }
    else if(cursor_visible_lock > 0.0f) cursor_visible_lock -= window.getWindowData()->delta_time;

    if(!cursor_lock) camera.event(&window);


    shader.bind();
    asset.render();
    shader.unbind();

    window.windowEvents();
    window.swapBuffer();
  };

  return 0;
}