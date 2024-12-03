#include "Shader.hpp"
#include <iostream>

Shader::Shader()
{
  type = kShaderType_Invalid;
  isCompiled = false;
  id = -1;
}

Shader::~Shader()
{
}

bool Shader::loadSource(const ShaderType shader_type, const char* source, const unsigned int source_size)
{
  glewInit();
  type = shader_type;
  switch (shader_type) {
  case kShaderType_Vertex:
    id = glCreateShader(GL_VERTEX_SHADER); break;

  case kShaderType_Fragment:
    id = glCreateShader(GL_FRAGMENT_SHADER); break;

  case kShaderType_Geometry:
    id = glCreateShader(GL_GEOMETRY_SHADER); break;
  default: return false; break;
  }
  GLint size = source_size;
  glShaderSource(id, 1, &source, &size);
  return true;
}

bool Shader::compile()
{
  glCompileShader(id);
  GLint params = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &params);
  if (GL_FALSE == params) {
    isCompiled = false;
    std::cout << "Error al compilar el shader: " << std::endl;
  }
  else
    isCompiled = true;
  return isCompiled;
}

unsigned int Shader::get_id()
{
  return id;
}

bool Shader::get_isCompiled() const
{
  return isCompiled;
}
