// Model.cpp
#include "ModelLoader/Model.hpp"
#include <iostream>

Model::Model(const std::string& path) : asyncMode(false) {
  loadModel(path);
}

Model::Model(const std::string& path, JobSystem& jobSystem) : asyncMode(true) {
  loadModelDataAsync(path, jobSystem);
}

Model::Model(Model&& other) noexcept :
  meshes(std::move(other.meshes)),
  meshesData(std::move(other.meshesData)),
  textures_loaded(std::move(other.textures_loaded)),
  directory(std::move(other.directory)),
  asyncMode(other.asyncMode),
  loadComplete(other.loadComplete),
  dataLoaded(other.dataLoaded),
  glResourcesCreated(other.glResourcesCreated) {
}

Model& Model::operator=(Model&& other) noexcept {
  if (this != &other) {
    meshes = std::move(other.meshes);
    meshesData = std::move(other.meshesData);
    textures_loaded = std::move(other.textures_loaded);
    directory = std::move(other.directory);
    asyncMode = other.asyncMode;
    loadComplete = other.loadComplete;
    dataLoaded = other.dataLoaded;
    glResourcesCreated = other.glResourcesCreated;
  }
  return *this;
}

void Model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
  dataLoaded = true;

  // In synchronous mode, we create GL resources immediately
  createGLResources();
}

void Model::loadModelDataAsync(const std::string& path, JobSystem& jobSystem) {
  auto self = std::shared_ptr<Model>(this, [](Model*) {});

  jobSystem.addJob(JobSystem::JobType::General, JobSystem::JobPriority::High,
    [self, path]() {
      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return;
      }

      self->directory = path.substr(0, path.find_last_of('/'));

      // Process nodes in parallel
      std::vector<std::future<void>> nodeFutures;
      for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++) {
        nodeFutures.push_back(std::async(std::launch::async,
          [self, node = scene->mRootNode->mChildren[i], scene]() {
            self->processNode(node, scene);
          }));
      }

      // Wait for all nodes to be processed
      for (auto& future : nodeFutures) {
        future.wait();
      }

      self->dataLoaded = true;
      std::cout << "Model data loaded asynchronously." << std::endl;
    });
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshesData.push_back(processMeshData(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Model::MeshLoadData Model::processMeshData(aiMesh* mesh, const aiScene* scene) {
  MeshLoadData data;

  // Process vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Mesh::Vertex vertex;
    vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

    if (mesh->HasNormals()) {
      vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
    }

    if (mesh->mTextureCoords[0]) {
      vertex.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
    }

    data.vertices.push_back(vertex);
  }

  // Process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      data.indices.push_back(face.mIndices[j]);
    }
  }

  // Process materials
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Store material information for later processing
    for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
      aiString texPath;
      material->GetTexture(aiTextureType_DIFFUSE, i, &texPath);

      MaterialInfo matInfo;
      matInfo.path = texPath.C_Str();
      matInfo.type = "texture_diffuse";
      matInfo.assimpType = aiTextureType_DIFFUSE;
      data.materialInfo.push_back(matInfo);
    }

    for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++) {
      aiString texPath;
      material->GetTexture(aiTextureType_SPECULAR, i, &texPath);

      MaterialInfo matInfo;
      matInfo.path = texPath.C_Str();
      matInfo.type = "texture_specular";
      matInfo.assimpType = aiTextureType_SPECULAR;
      data.materialInfo.push_back(matInfo);
    }
  }

  data.directory = directory;
  return data;
}

void Model::createGLResources() {
  if (!dataLoaded) {
    std::cerr << "Cannot create GL resources before data is loaded" << std::endl;
    return;
  }

  for (const auto& meshData : meshesData) {
    std::vector<Mesh::Texture> textures;

    // Process all materials for this mesh
    for (const auto& matInfo : meshData.materialInfo) {
      bool skip = false;

      // Check if we've already loaded this texture
      for (const auto& loadedTex : textures_loaded) {
        if (loadedTex.path == matInfo.path) {
          textures.push_back(loadedTex);
          skip = true;
          break;
        }
      }

      if (!skip) {
        Mesh::Texture texture;
        texture.id = TextureFromFile(matInfo.path.c_str(), meshData.directory);
        texture.type = matInfo.type;
        texture.path = matInfo.path;

        textures.push_back(texture);
        textures_loaded.push_back(texture);
      }
    }

    // Create mesh with OpenGL buffers
    meshes.emplace_back(meshData.vertices, meshData.indices, textures);
  }

  meshesData.clear();
  glResourcesCreated = true;
  loadComplete = true;
}

std::vector<Mesh::Texture> Model::get_LoadedTextures()
{
    return textures_loaded;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory) {
  std::string filename = directory + '/' + std::string(path);
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format = (nrComponents == 1) ? GL_RED :
      (nrComponents == 3) ? GL_RGB : GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

void Model::Draw(Program &program) const {
  for (const auto& mesh : meshes) {
    mesh.Draw(program);
  }
  glBindVertexArray(0);
  //glBindTexture(GL_TEXTURE_2D, 0);
  
}

void Model::finalizeModel() {
  if (dataLoaded && !glResourcesCreated) {
    createGLResources();
  }
}