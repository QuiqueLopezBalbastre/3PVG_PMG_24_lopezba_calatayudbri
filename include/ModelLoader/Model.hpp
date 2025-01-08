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
#include "JobSystem.hpp"

class Model {
public:
  Model(const std::string& path);
  Model(const std::string& path, JobSystem& jobSystem);
  // Método para finalizar el modelo (si se usó JobSystem)
  void finalizeModel();
  ~Model() = default;
  
  Model(const Model& other) = delete;
  Model& operator=(const Model& other) = delete;;
  Model(Model&& other) noexcept : meshes(std::move(other.meshes)) {}
  Model& operator=(Model&& other) noexcept;
  
  std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
  bool isLoadComplete();
  //std::vector <Mesh> getMeshes();
  std::vector<Mesh> meshes;
  void Draw(Program program) const;

private:

  struct MeshData {
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Mesh::Texture> textures;
  };   
  std::vector<MeshData> tempMeshes;
  std::vector<Mesh::Texture> textures_loaded;
  std::string directory;
  bool asyncMode;
  bool loadComplete = false;
  void loadModel(const std::string& path);
  void asyncLoadModel(const std::string& path, JobSystem& jobSystem);
  void processNode(aiNode* node, const aiScene* scene);
  Model::MeshData processMesh(aiMesh* mesh, const aiScene* scene);
  unsigned int TextureFromFile(const char* path, const std::string& directory);
};

#endif

