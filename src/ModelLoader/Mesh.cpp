#include "ModelLoader/Mesh.hpp"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
  : indices{ indices }
  , destroy{ true } {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

  glBindVertexArray(0);

  std::cout << "Mesh loaded correctly\n" << std::endl;
}
// Destructor
Mesh::~Mesh() {
  if (destroy)
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
}

Mesh::Mesh(Mesh&& other)
{
  indices = std::move(other.indices);
  VAO = std::move(other.VAO);
  VBO = std::move(other.VBO);
  EBO = std::move(other.EBO);
  destroy = std::move(other.destroy);
  other.destroy = false;
}

Mesh& Mesh::operator=(Mesh&& other)
{
  if (this != &other) {
    // TODO: Insertar una instrucción "return" aquí
    indices = std::move(other.indices);
    VAO = std::move(other.VAO);
    VBO = std::move(other.VBO);
    EBO = std::move(other.EBO);
    destroy = std::move(other.destroy);
    other.destroy = false;
  }
  return *this;
}

void Mesh::Draw() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
  glBindVertexArray(0);
}