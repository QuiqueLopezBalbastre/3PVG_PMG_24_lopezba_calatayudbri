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

void Program::unuse()
{
    glUseProgram(0);
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

void Program::setVec3(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(MapUniformLocation(name), 1, &value[0]);
}

void Program::setInt(const std::string& name, const int value)
{
    glUniform1i(MapUniformLocation(name), value);
}

void Program::setFloat(const std::string& name, const float value)
{
    glUniform1f(MapUniformLocation(name), value);
}

void Program::setMat4(const std::string& name, glm::mat4& value)
{
    glUniform4fv(MapUniformLocation(name), 1, &value[0][0]);
}

int Program::MapUniformLocation(const std::string& name)
{
    auto it = uniform_map.find(name);
    if (it == uniform_map.end())
    {
        int loc = GetUniformLocation(name);
        it = uniform_map.emplace_hint(uniform_map.end(), name, loc);
    }

    return it->second;
}

int Program::GetUniformLocation(const std::string& location)
{
    int loc = glGetUniformLocation(id, location.data());
    if (loc == -1)
    {
        printf("Location [%s] does not exist \n", location.data());
        
    }
    
    return loc;
}
