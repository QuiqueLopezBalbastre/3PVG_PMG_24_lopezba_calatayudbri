#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>

#include "Window_System.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "ModelLoader/Mesh.hpp"
#include "ModelLoader/Model.hpp"
#include "Input.hpp"
#include "common.hpp"

//static const char* glErrorString(GLenum err) {
//  switch (err) {
//  case GL_NO_ERROR:
//    return "GL_NO_ERROR";
//  case GL_INVALID_ENUM:
//    return "GL_INVALID_ENUM";
//  case GL_INVALID_VALUE:
//    return "GL_INVALID_VALUE";
//  case GL_INVALID_OPERATION:
//    return "GL_INVALID_OPERATION";
//  case GL_STACK_OVERFLOW:
//    return "GL_STACK_OVERFLOW";
//  case GL_STACK_UNDERFLOW:
//    return "GL_STACK_UNDERFLOW";
//  case GL_OUT_OF_MEMORY:
//    return "GL_OUT_OF_MEMORY";
//  case GL_INVALID_FRAMEBUFFER_OPERATION:
//    return "GL_INVALID_FRAMEBUFFER_OPERATION";
//  default:
//    return "Unknown error";
//  }
//}

int main(int argc, char** argv) {
  //auto ws = WindowSystem::make();
  glfwInit();
  auto window = Window::make(1280, 720, "LUQUI");
  if (nullptr == window->window) {
    window->~Window();
    glfwTerminate();
    return -1;
  }
  window->setCurrentWindowActive();
  static Input input(window->window);
  glewInit();

#define GLSL(x) "#version 330\n"#x
  static const char* kExampleFragmentShader = GLSL(
    out vec4 FragColor;
  void main()
  {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Color naranja
  }
    );


#define GLSL(x) "#version 330\n"#x
  static const char* kExampleVertexShader = GLSL(
    layout(location = 0) in vec3 position;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main()
  {
    gl_Position = projection * view * model * vec4(position, 1.0);
  }
    );

  /** Creating shaders */
  Shader vertex = Shader();
  vertex.loadSource(Shader::ShaderType::kShaderType_Vertex, kExampleVertexShader, strlen(kExampleVertexShader));
  vertex.compile();
  if (!vertex.get_isCompiled())
    return -1;
  Shader fragment = Shader();
  fragment.loadSource(Shader::ShaderType::kShaderType_Fragment, kExampleFragmentShader, strlen(kExampleFragmentShader));
  fragment.compile();
  if (!fragment.get_isCompiled())
    return -1;

  /** Creating Program */
  Program program = Program();
  program.attach(&vertex);
  program.attach(&fragment);
  if (!program.link()) {
    std::cout << "Error al linkar el programa" << std::endl;
    return -1;
  }
  program.use();

  /** Uploading Models to represent */
  Model objModel("../data/Models/cubo.obj");
  Model objModel1("../data/Models/Candle.obj");
  //Model fbxModel("../data/Models/Minotaur_Male_Lores.fbx");


  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
    //std::cout << "First get error: " << glErrorString(glGetError()) << std::endl;
    glEnable(GL_COLOR_BUFFER_BIT);
    //std::cout << "Second get error: " << glErrorString(glGetError()) << std::endl;

    /* Render here */
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //std::cout << "Third get error: " << glErrorString(glGetError()) << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //std::cout << "Fourth get error: " << glErrorString(glGetError()) << std::endl;
    glEnable(GL_DEPTH_TEST);
    //std::cout << "Fifth get error: " << glErrorString(glGetError()) << std::endl;
    glClear(GL_DEPTH_BUFFER_BIT);
    //std::cout << "Before shader calcs " << glErrorString(glGetError()) << std::endl;

    glm::mat4x4 model, view, projection;
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 5.0f), glm::vec3(0, 1, 0));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.f);
    GLuint model_loc = glGetUniformLocation(program.get_id(), "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    GLuint view_loc = glGetUniformLocation(program.get_id(), "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    GLuint projection_loc = glGetUniformLocation(program.get_id(), "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));


    //std::cout << "Before Draw " << glErrorString(glGetError()) << std::endl;
    //objModel.Draw();
    objModel1.Draw();
    //std::cout << "After Draw " << glErrorString(glGetError()) << std::endl;

    //fbxModel.Draw();
    /* Swap front and back buffers */
    glfwSwapBuffers(window->window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}

