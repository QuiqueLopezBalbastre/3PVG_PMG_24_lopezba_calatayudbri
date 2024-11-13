#include <GL/glew.h>

#include "Window_System.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "ModelLoader/Mesh.hpp"
#include "ModelLoader/Model.hpp"
#include "Input.hpp"

int main(int argc, char** argv) {
  //auto ws = WindowSystem::make();
  glfwInit();
  auto window = Window::make(640, 480, "LUQUI");
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
  //Model objModel("../data/Candle.obj");
  //Model fbxModel("../data/Minotaur_Male_Lores.fbx");

  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
    //fbxModel.Draw();
    glEnable(GL_COLOR_BUFFER_BIT);
    /* Render here */
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    objModel.Draw();
    /* Swap front and back buffers */
    glfwSwapBuffers(window->window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  //if (-1 == window.destroyWindow()) {
  //  return -1;
  //}

  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}

