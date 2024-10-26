#include "Mesh.hpp"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

std::vector<Mesh> Mesh::loadModel(const std::string& path) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path,
    aiProcess_Triangulate |          // Convierte todo a triángulos
    aiProcess_FlipUVs |              // Voltea coordenadas UV si es necesario
    aiProcess_GenSmoothNormals |     // Genera normales suavizadas si no existen
    aiProcess_JoinIdenticalVertices);// Une vértices idénticos

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    printf("Error al cargar modelo: %s", importer.GetErrorString());
    return {};
  }

  std::vector<Mesh> meshes;
  processNode(scene->mRootNode, scene, meshes);
  return meshes;
}
Mesh Mesh::processMesh(aiMesh* mesh, const aiScene* scene) {
  Mesh result;

  // Procesa los vértices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    // Posición
    vertex.position[0] = mesh->mVertices[i].x;
    vertex.position[1] = mesh->mVertices[i].y;
    vertex.position[2] = mesh->mVertices[i].z;

    // Normal
    if (mesh->HasNormals()) {
      vertex.normal[0] = mesh->mNormals[i].x;
      vertex.normal[1] = mesh->mNormals[i].y;
      vertex.normal[2] = mesh->mNormals[i].z;
    }

    // Coordenadas de textura
    if (mesh->mTextureCoords[0]) { // Verifica si tiene coordenadas UV
      vertex.texCoords[0] = mesh->mTextureCoords[0][i].x;
      vertex.texCoords[1] = mesh->mTextureCoords[0][i].y;
    }
    else {
      vertex.texCoords[0] = 0.0f;
      vertex.texCoords[1] = 0.0f;
    }

    result.vertices.push_back(vertex);
  }

  // Procesa los índices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      result.indices.push_back(face.mIndices[j]);
    }
  }

  return result;
}

void Mesh::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes) {
  // Procesa todas las mallas en el nodo actual
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  // Procesa los hijos del nodo
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, meshes);
  }
}

void Mesh::setupMesh() {
  // Genera y configura los buffers de OpenGL
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  // Posición
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // Normales
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  // Coordenadas de textura
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
}

void Mesh::drawMesh() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}