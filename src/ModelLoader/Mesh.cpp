#include "ModelLoader/Mesh.hpp"
#include "common.hpp"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
  : indices{ indices }
  , destroy{ true } {

  glGenVertexArrays(1, &VAO);
  std::cout << "Mesh() 1 " << glErrorString(glGetError()) << std::endl;
  glGenBuffers(1, &VBO);
  std::cout << "Mesh() 2 " << glErrorString(glGetError()) << std::endl;
  glGenBuffers(1, &EBO);
  std::cout << "Mesh() 3 " << glErrorString(glGetError()) << std::endl;

  glBindVertexArray(VAO);
  std::cout << "Mesh() 4 " << glErrorString(glGetError()) << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  std::cout << "Mesh() 5 " << glErrorString(glGetError()) << std::endl;
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  std::cout << "Mesh() 6 " << glErrorString(glGetError()) << std::endl;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  std::cout << "Mesh() 7 " << glErrorString(glGetError()) << std::endl;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  std::cout << "Mesh() 8 " << glErrorString(glGetError()) << std::endl;

  glEnableVertexAttribArray(0);
  std::cout << "Mesh() 9 " << glErrorString(glGetError()) << std::endl;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  std::cout << "Mesh() 10 " << glErrorString(glGetError()) << std::endl;

  glEnableVertexAttribArray(1);
  std::cout << "Mesh() 11 " << glErrorString(glGetError()) << std::endl;

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  std::cout << "Mesh() 12 " << glErrorString(glGetError()) << std::endl;

  glEnableVertexAttribArray(2);
  std::cout << "Mesh() 13 " << glErrorString(glGetError()) << std::endl;
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
  std::cout << "Mesh() 14 " << glErrorString(glGetError()) << std::endl;

  glBindVertexArray(0);
  std::cout << "Mesh() 15 " << glErrorString(glGetError()) << std::endl;

  //glErrorString(glGetError());

  //std::cout << "Mesh loaded correctly\n" << std::endl;
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
  //std::cout << "Draw Mesh First " << glErrorString(glGetError()) << std::endl;

  glBindVertexArray(VAO);
  //std::cout << "Draw Mesh Second " << glErrorString(glGetError()) << std::endl;
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
  //std::cout << "Draw Mesh Third " << glErrorString(glGetError()) << std::endl;
  glBindVertexArray(0);
}