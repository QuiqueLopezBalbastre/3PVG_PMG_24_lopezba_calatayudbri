#ifndef __MESH__ 
#define __MESH__ 1

#include <GL/glew.h>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh {
  struct Vertex {
    float position[3];
    float normal[3];
    float texCoords[2];
  };

public:
  /** Methods */
  Mesh() = default;
  ~Mesh() = default;
  std::vector<Mesh> loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  void setupMesh();
  void drawMesh() const;

  /** Attributes */
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;


private:
  Mesh(Mesh&) = default;
  GLuint VAO, VBO, EBO;
};

#endif // !1
