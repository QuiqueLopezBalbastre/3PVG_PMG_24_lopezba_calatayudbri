#ifndef __MODEL__ 
#define __MODEL__ 1

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include "Mesh.hpp"
#include "../Program.hpp"

class Model {
public:
  Model(const std::string& path);
  ~Model() = default;
  Model(const Model& other) = delete;
  Model& operator=(const Model& other) = delete;;
  Model(Model&& other) noexcept : meshes(std::move(other.meshes)) {}
  Model& operator=(Model&& other) noexcept;
  
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

  void Draw(Program program) const;
  std::vector<Mesh> meshes;

private:
  unsigned int TextureFromFile(const char* path, const std::string& directory);
  std::vector<Mesh::Texture> textures_loaded;
  std::string directory;
};

#endif

