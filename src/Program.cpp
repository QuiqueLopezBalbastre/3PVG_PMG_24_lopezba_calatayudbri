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

GLuint Program::get_depthMapFBO()
{
    return depthMapFBO;
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

void Program::setmat4(const std::string& name, glm::mat4& value)
{
    glUniformMatrix4fv(MapUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Program::CreateShadowMap()
{
    
    glGenFramebuffers(1, &depthMapFBO);

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
