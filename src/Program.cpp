#include "Program.hpp"
#include <iostream>

Program::Program()
{
  id = glCreateProgram();
}

Program::~Program()
{
  glDeleteProgram(id);
}

void Program::attach(Shader* shader)
{
  glAttachShader(id, shader->get_id());

}

bool Program::link()
{
  glLinkProgram(id);

  GLint params = 0;
  glGetProgramiv(id, GL_LINK_STATUS, &params);
  if (GL_FALSE == params) {
    std::cout << "Could not link the shader to the program" << std::endl;
    return false;
  }
  std::cout << "Linked succesfully the shader to the program" << std::endl;
  return true;
}

void Program::use() const
{
  glUseProgram(id);

}
