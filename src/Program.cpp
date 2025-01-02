#include "Program.hpp"
#include <iostream>
#include "common.hpp"

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

GLuint Program::get_id()
{
  return id;
}

void Program::setUniform(const std::string& name, int value)
{
  GLint location = glGetUniformLocation(id, name.c_str());
  std::cout << "Program: " << glErrorString(glGetError()) << std::endl;

  if (location != -1) {
    //std::cout << "Uniform location = " << location << std::endl;
    glUniform1i(location, value);
  }
}

void Program::use() const
{
  glUseProgram(id);

}

bool Program::createProgram(Program& program, Shader* vertex_shader, Shader* fragment_shader)
{
  //Program program = Program();
  program.attach(vertex_shader);
  program.attach(fragment_shader);
  if (!program.link()) {
    std::cout << "Error al linkar el programa" << std::endl;
    return false;
  }
  return true;
}
