#ifndef __MESH__
#define __MESH__ 1

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


class Mesh {
public:
  /*!
   *  
   */
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };
  /** Miembros de clase especiales */
  /*!
   *  Constructor.
   *
   *      @param [in] vertices 
   *      @param [in] indices  
   */
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  /*!
   *  Destructor.
   */
  ~Mesh();
  /*!
   *  Copy Constructor DELETED
   *
   *      @param [in] other 
   */
  Mesh(const Mesh& other) = delete;
  /*!
   *  Constructor.
   *
   *      @param [in] other 
   */
  Mesh(Mesh&& other);
  /*!
   *  Operator=s the mesh. DELETED
   *
   *      @param [in] other 
   *
   *      @return 
   */
  Mesh& operator=(const Mesh& other) = delete;
  /*!
   *  Operator=s the mesh.
   *
   *      @param [in,out] other 
   *
   *      @return 
   */
  Mesh& operator=( Mesh&& other);
 
  /** Métodos de clase */
  /*!
   *  Draws the mesh.
   */
  void Mesh::Draw() const;
private:
  bool destroy;
  std::vector<unsigned int> indices;
  GLuint VAO, VBO, EBO;

};

#endif // !__MESH__
