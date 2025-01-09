#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "ModelLoader/Mesh.hpp"
#include "ModelLoader/Model.hpp"
#include "Input.hpp"
#include "common.hpp"

#include <iostream>



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

  in vec2 TexCoords;

  uniform sampler2D texture_diffuse;

  void main()
  {

    FragColor = texture(texture_diffuse, TexCoords);
       // Muestra coordenadas UV como color
      //FragColor = vec4(TexCoords, 0.0, 1.0);
  }
    );


#define GLSL(x) "#version 330\n"#x
  static const char* kExampleVertexShader = GLSL(
    layout(location = 0) in vec3 aPos;
  layout(location = 1) in vec3 aNormal;
  layout(location = 2) in vec2 aTexCoords;

  out vec2 TexCoords;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main()
  {
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
  }
    );
  /** Creating shaders */
  Shader vertex = Shader();
  vertex.loadSource(Shader::ShaderType::kShaderType_Vertex, kExampleVertexShader, (unsigned int)strlen(kExampleVertexShader));
  vertex.compile();
  if (!vertex.get_isCompiled())
    return -1;
  Shader fragment = Shader();
  fragment.loadSource(Shader::ShaderType::kShaderType_Fragment, kExampleFragmentShader, (unsigned int)strlen(kExampleFragmentShader));
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

  Model texCube("../data/Models/cube/cube.obj");
  Model texChair("../data/Models/cube/wheelchair.obj");

  std::cout << "Model load error: " << glErrorString(glGetError()) << std::endl;



  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
    glEnable(GL_COLOR_BUFFER_BIT);

    /* Render here */
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4x4 model, view, projection;
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 10.0f), glm::vec3(0, 1, 0));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.f);

    glEnable(GL_TEXTURE_2D);
    GLuint model_loc = glGetUniformLocation(program.get_id(), "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint view_loc = glGetUniformLocation(program.get_id(), "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projection_loc = glGetUniformLocation(program.get_id(), "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));


    
    //texCube.Draw(program);
    texChair.Draw(program);


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

