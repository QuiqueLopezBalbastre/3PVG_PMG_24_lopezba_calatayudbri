// Model.hpp
#ifndef __MODEL__ 
#define __MODEL__ 1

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <future>
#include <memory>
#include "Mesh.hpp"
#include "../Program.hpp"
#include "JobSystem.hpp"

class Model {
public:
  // Constructors for synchronous and asynchronous loading
  Model(const std::string& path, std::string name_);
  Model(const std::string& path, JobSystem& jobSystem);

  // Destructor and move semantics
  ~Model() = default;
  Model(const Model& other) = delete;
  Model& operator=(const Model& other) = delete;
  Model(Model&& other) noexcept;
  Model& operator=(Model&& other) noexcept;

  // Public interface
  void finalizeModel();
  void Draw(Program &program) const;
  bool isLoadComplete() const { return loadComplete; }
  bool isDataLoaded() const { return dataLoaded; }
  bool isGLResourcesCreated() const { return glResourcesCreated; }

  // Public data (consider making private with accessors)
  std::vector<Mesh> meshes;
  void createGLResources();
  std::string get_name();
private:
  // Structures for handling loading state
  struct MaterialInfo {
    std::string path;
    std::string type;
    aiTextureType assimpType;
  };

  struct MeshLoadData {
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MaterialInfo> materialInfo;
    std::string directory;
  };

  std::string name;
  // Member variables for loading state
  std::vector<MeshLoadData> meshesData;
  std::vector<Mesh::Texture> textures_loaded;
  std::string directory;
  bool asyncMode = false;
  bool loadComplete = false;
  bool dataLoaded = false;
  bool glResourcesCreated = false;

  // Private methods for loading and processing
  void loadModel(const std::string& path);
  void loadModelDataAsync(const std::string& path, JobSystem& jobSystem);
  void processNode(aiNode* node, const aiScene* scene);
  MeshLoadData processMeshData(aiMesh* mesh, const aiScene* scene);
  std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
  unsigned int TextureFromFile(const char* path, const std::string& directory);
};

#endif