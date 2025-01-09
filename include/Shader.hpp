#ifndef __SHADER_H__
#define __SHADER_H__ 1

#include <GL/glew.h>

class Shader {
public:
  enum ShaderType {
    kShaderType_Invalid,
    kShaderType_Vertex,
    kShaderType_Fragment,
    kShaderType_Geometry,
  };
  /** Miembros de clase especiales */
  Shader();
  ~Shader();

  /** Métodos de clase */
  bool loadSource(const ShaderType shader_type, const char* source, const unsigned int source_size);
  bool compile();
  bool get_isCompiled() const;
  unsigned int get_id();

private:
  /** Atributos */
  ShaderType type;
  bool isCompiled;
  unsigned int id;
};

#endif
