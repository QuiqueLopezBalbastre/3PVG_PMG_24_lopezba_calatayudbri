#ifndef __PROGRAM__
#define __PROGRAM__

#include <GL/glew.h>
#include "Shader.hpp"

class Program {
public:
  /** Miembros de clase especiales */
  Program();
  ~Program();
  /** Métodos de clase */
  void attach(Shader* shader);
  bool link();
  void use() const;

private:
  /** Atributos */
  GLuint id;
};
#endif // !__PROGRAM__
