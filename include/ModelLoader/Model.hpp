#ifndef __MODEL__
#define __MODEL__ 1


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>

#include "Mesh.hpp"

class Model {
public:
  Model(const std::string& path);
  ~Model() = default;
  Model& operator=(Model&& other) noexcept;
  Model(const Model& other) = delete;
  Model(Model&& other) = default;

  void Draw() const;

private:
  std::vector<Mesh> meshes;

  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  
};
#endif // !__MODEL__
