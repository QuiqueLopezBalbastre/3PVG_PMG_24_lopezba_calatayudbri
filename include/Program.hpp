#ifndef __PROGRAM__
#define __PROGRAM__

#include <GL/glew.h>
#include "Shader.hpp"
#include <string>

class Program {
public:
  /** Miembros de clase especiales */
  Program();
  ~Program();

  /** Métodos de clase */
  void attach(Shader* shader);
  bool link();
  GLuint get_id();
  void setUniform(const std::string& name, int value);
  void use() const;
  static bool createProgram(Program &program, Shader* vertex_shader, Shader* fragment_shader);
private:
  /** Atributos */
  GLuint id;
};
#endif // !__PROGRAM__
