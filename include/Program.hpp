#ifndef __PROGRAM__
#define __PROGRAM__

#include <GL/glew.h>
#include "Shader.hpp"

class Program {
public:
  /** Miembros de clase especiales */
  Program();
  ~Program();
  /** M�todos de clase */
  void attach(Shader* shader);
  bool link();
  GLuint get_id();
  void use() const;

private:
  /** Atributos */
  GLuint id;
};
#endif // !__PROGRAM__
