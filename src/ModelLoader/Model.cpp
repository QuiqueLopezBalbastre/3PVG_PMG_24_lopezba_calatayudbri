#include "ModelLoader/Model.hpp"

// Constructor
Model::Model(const std::string& path) {
  loadModel(path);
}
// Operador de asignación por movimiento
Model& Model::operator=(Model&& other) noexcept {
  if (this != &other) {
    meshes = std::move(other.meshes);
  }
  return *this;
}

void Model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "Error al cargar modelo: " << importer.GetErrorString() << std::endl;
    return;
  }
  else {
    std::cout << "No hubo error al cargar el modelo: "  << path << std::endl;
  }

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Mesh::Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Mesh::Vertex vertex;
    vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

    if (mesh->HasNormals()) {
      vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
    }

    if (mesh->mTextureCoords[0]) {
      vertex.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
    }
    else {
      vertex.texCoords = { 0.0f, 0.0f };
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return Mesh{ vertices, indices };
}


void Model::Draw() const {
  for (const auto& mesh : meshes) {
    mesh.Draw();
  }
}
