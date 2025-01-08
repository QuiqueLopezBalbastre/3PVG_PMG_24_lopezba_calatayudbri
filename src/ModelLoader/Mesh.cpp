#include "ModelLoader/Mesh.hpp"
#include "common.hpp"

Mesh::Mesh(std::vector<Vertex>vertices, std::vector<unsigned int>indices, std::vector<Texture> textures)
  : vertices_{ vertices }
  , indices_{ indices }
  , indices_size_{ (unsigned int)indices.size() }
  , destroy{ true }
  , textures_{ textures }
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  std::cout << "VAO: " << VAO << std::endl;
  std::cout << "VBO: " << VBO << std::endl;
  std::cout << "EBO: " << EBO << std::endl;
  OPENGL_CHECK;
  glBindVertexArray(VAO);
  OPENGL_CHECK;
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  OPENGL_CHECK;
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  OPENGL_CHECK;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  OPENGL_CHECK;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  OPENGL_CHECK;
  glEnableVertexAttribArray(0);
  OPENGL_CHECK;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  OPENGL_CHECK;
  glEnableVertexAttribArray(1);
  OPENGL_CHECK;
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  OPENGL_CHECK;

  glEnableVertexAttribArray(2);
  OPENGL_CHECK;
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
  OPENGL_CHECK;

  glBindVertexArray(0);
}

Mesh::~Mesh()
{
  if (destroy) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
}

Mesh::Mesh(Mesh&& other) noexcept
{
  vertices_ = std::move(other.vertices_);
  textures_ = std::move(other.textures_);
  VAO = std::move(other.VAO);
  VBO = std::move(other.VBO);
  EBO = std::move(other.EBO);
  indices_size_ = std::move(other.indices_size_);
  destroy = std::move(other.destroy);
  other.destroy = false;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
  if (&other != this) {
    textures_ = std::move(other.textures_);
    VAO = std::move(other.VAO);
    VBO = std::move(other.VBO);
    EBO = std::move(other.EBO);
    indices_size_ = std::move(other.indices_size_);
    destroy = std::move(other.destroy);
    other.destroy = false;
  }
  return *this;
}

void Mesh::Draw(Program program) const
{
  // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < textures_.size(); i++)
  {


    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = textures_[i].type;
    printf("%s \n", textures_[i].type);
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++); // transfer unsigned int to string
    else if (name == "texture_normal")
      number = std::to_string(normalNr++); // transfer unsigned int to string
    else if (name == "texture_height")
      number = std::to_string(heightNr++); // transfer unsigned int to string

    // now set the sampler to the correct texture unit
    glUniform1i(glGetUniformLocation(program.get_id(), (name + number).c_str()), i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);


  }
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices_size_, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);

  // Limpia estados
  glActiveTexture(GL_TEXTURE0);
}

GLuint Mesh::getVAO()
{
  return VAO;
}

GLuint Mesh::getVBO()
{
  return VBO;
}

GLuint Mesh::getEBO()
{
  return EBO;
}

std::vector<Mesh::Vertex> Mesh::getVertices()
{
  return vertices_;
}

