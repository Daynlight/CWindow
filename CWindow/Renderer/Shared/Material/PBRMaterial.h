#pragma once
#include <string>
#include "glm/glm.hpp"







namespace CW{
struct PBRMaterial {
  glm::vec3 albedo = glm::vec3(1.0f);
  float metallic = 0.0f;
  float roughness = 1.0f;
  glm::vec3 emission_color = glm::vec3(0.0f);
  float emission_strength = 0.0f;
  float ambient_occlusion = 1.0f;

  void setMaterialUniform(CW::Renderer::Uniform& uniform){
    uniform["albedo"]->set<glm::vec3>(albedo);
    uniform["roughness"]->set<float>(roughness);
    uniform["metallic"]->set<float>(metallic);
    uniform["emission_color"]->set<glm::vec3>(emission_color);
    uniform["emission_strength"]->set<float>(emission_strength);
    uniform["ambient_occlusion"]->set<float>(ambient_occlusion);
  };
};



inline const std::string PBRShaderBRDFUniforms = R"(

uniform vec3 albedo;
uniform float roughness;
uniform float metallic;
uniform float emission_strength;
uniform vec3 emission_color;
uniform float ambient_occlusion;

)";



inline const std::string PBRShaderBRDFInclude = R"(

const float PI = 3.14159265359;

// ---------------- BRDF FUNCTIONS ----------------

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;

  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return a2 / max(denom, 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
  float r = roughness + 1.0;
  float k = (r * r) / 8.0;

  return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);

  return GeometrySchlickGGX(NdotV, roughness) *
          GeometrySchlickGGX(NdotL, roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 BRDF_PBR(
  vec3 N,
  vec3 V,
  vec3 L,
  vec3 lightColor
)
{
  vec3 H = normalize(V + L);

  float NdotL = max(dot(N, L), 0.0);
  float NdotV = max(dot(N, V), 0.0);
  float NdotH = max(dot(N, H), 0.0);

  // ---------------- Fresnel ----------------
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);

  vec3 F = FresnelSchlick(max(dot(V, H), 0.0), F0);

  // ---------------- GGX ----------------
  float a = roughness * roughness;
  float a2 = a * a;

  float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
  float D = a2 / (PI * denom * denom + 0.0001);

  float k = (roughness + 1.0);
  k = (k * k) / 8.0;

  float G1 = NdotL / (NdotL * (1.0 - k) + k);
  float G2 = NdotV / (NdotV * (1.0 - k) + k);
  float G = G1 * G2;

  // ---------------- Specular ----------------
  vec3 spec = (D * G * F) / max(4.0 * NdotL * NdotV, 0.001);

  // ---------------- Diffuse (Blender-style energy conservation) ----------------
  vec3 kS = F;
  vec3 kD = (1.0 - kS) * (1.0 - metallic);

  vec3 diffuse = kD * albedo / PI;

  return (diffuse + spec) * lightColor * NdotL;
}

vec3 BRDF(
  vec3 Normal,
  vec3 FragPos,
  vec3 cameraPos,
  vec3 lightPos,
  vec3 lightColor
)
{
  vec3 N = normalize(Normal);
  vec3 V = normalize(cameraPos - FragPos);
  vec3 L = normalize(lightPos - FragPos);

  vec3 lighting = BRDF_PBR(N, V, L, lightColor);

  // // AO ONLY affects indirect approximation (simple hack)
  // lighting *= ambient_occlusion;

  vec3 ambient = 0.03 * albedo * ambient_occlusion;

  vec3 emissiveColor = emission_color * emission_strength;

  return lighting + ambient + emissiveColor;
}
)";
};