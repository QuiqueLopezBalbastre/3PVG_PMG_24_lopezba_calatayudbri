#ifndef __MESH__
#define __MESH__ 1

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <optional>
#include <vector>
#include <string>
#include "../Program.hpp"

class Mesh {
public:
  //struct Texture {
  //  std::optional<unsigned int> diffuse;
  //  std::optional<unsigned int> specular;
  //  std::optional<unsigned int> normal;
  //  std::optional<unsigned int> height;
  //};
  struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
  };
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };
  Mesh(std::vector<Vertex>vertices, std::vector<unsigned int>indices, std::vector<Texture> textures);
  ~Mesh();
  Mesh(const Mesh&) = delete;
  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;
  void Draw(Program program) const;
  std::vector<Texture> textures_;
private:
  bool destroy;
  GLuint VAO, VBO, EBO;
  unsigned int indices_size_;
  std::vector<Vertex> vertices_;
};


#endif