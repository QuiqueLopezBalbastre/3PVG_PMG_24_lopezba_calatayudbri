#ifndef __MESH__
#define __MESH__ 1

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


class Mesh {
public:
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };
  /** Miembros de clase especiales */
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  ~Mesh();
  Mesh(const Mesh& other) = delete;
  Mesh(Mesh&& other);
  Mesh& operator=(const Mesh& other) = delete;
  Mesh& operator=( Mesh&& other);
 
  /** Métodos de clase */
  void Mesh::Draw() const;
private:
  bool destroy;
  std::vector<unsigned int> indices;
  GLuint VAO, VBO, EBO;

};

#endif // !__MESH__
