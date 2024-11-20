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
  /*!
   *  Constructor.
   *
   *      @param [in] path
   */
  Model(const std::string& path);
  /*!
   *  Destructor
   */
  ~Model() = default;
  /*!
   *  Operator=s the model.
   *
   *      @param [in,out] other
   *
   *      @return
   */
  Model& operator=(Model&& other) noexcept;
  /*!
   *  Copy Constructor.
   *
   *      @param [in] other
   */
  Model(const Model& other) = delete;
  /*!
   *  Move Constructor
   */
  Model(Model&& other) = default;

  void Draw() const;

private:
  std::vector<Mesh> meshes;

  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);

};
#endif // !__MODEL__
