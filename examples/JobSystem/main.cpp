#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>
#include <future>
#include <memory>
#include <utility>

#include "WindowSystem.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Figure.hpp"
#include "JobSystem.hpp"
#include "ModelLoader/Model.hpp"
#include "ModelLoader/Mesh.hpp"

int main(int argc, char** argv) {
  //auto ws = WindowSystem::make();

  glfwInit();
  auto window = Window::make(1000, 800, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();

  JobSystem js = JobSystem(std::thread::hardware_concurrency() * 0.5f);
  //for (int i = 0; i < 3; i++) {
  //  // Encolar trabajos de diferentes tipos y prioridades
  //  js.addJob(JobSystem::JobType::Render, JobSystem::JobPriority::High, []() {
  //    std::cout << "Trabajo de renderizado (Alta prioridad) completado.\n";
  //    });

  //  js.addJob(JobSystem::JobType::Physics, JobSystem::JobPriority::Medium, []() {
  //    std::cout << "Trabajo de fisica (Prioridad media) completado.\n";
  //    });

  //  js.addJob(JobSystem::JobType::AI, JobSystem::JobPriority::Low, []() {
  //    std::cout << "Trabajo de AI (Baja prioridad) completado.\n";
  //    });

  //  js.addJob(JobSystem::JobType::Render, JobSystem::JobPriority::Low, []() {
  //    std::cout << "Trabajo de renderizado (Baja prioridad) completado.\n";
  //    });
  //}

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


  std::vector<std::string> modelPaths;
  std::string path = "../data/Models/Cannon/cannon.obj";
  modelPaths.push_back(path);
  path = "../data/Models/PirateShip/ship-pirate-medium.obj";
  modelPaths.push_back(path);
  path = "../data/Models/ShipWreck/ship-wreck.obj";
  modelPaths.push_back(path);
  int currentModelIndex = 0;
  //Model modelsync(modelPaths[0]);
  //Model modelAsync(modelPaths[1]);
  std::unique_ptr<Model> currentModel = std::make_unique<Model>(Model::Model(modelPaths[0]));
  Model normalmodel(modelPaths[1]);
  std::unique_ptr<Model> backmodel;
  normalmodel.finalizeModel();
  bool modelLoading = false;
  std::future<std::unique_ptr<Model>> modelLoadFuture;
  //js.addJob(JobSystem::JobType::Render, JobSystem::JobPriority::High, [&renderedobjs, paths]() {renderedobjs = Model(paths[0]); });



  static Input input(window->window);
  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
    window->clear();
    if (input.wasKeyJustPressed(Input::KEY_D)) {
      if (backmodel)
        backmodel.reset();
      currentModelIndex = (currentModelIndex + 1) % modelPaths.size();
      backmodel = std::make_unique<Model>(modelPaths[currentModelIndex]);
      backmodel->finalizeModel();
    }
    if (input.wasKeyJustPressed(Input::KEY_RIGHT)) {
      if (!modelLoading) {
        // Inicia la carga del modelo asíncrona
        currentModelIndex = (currentModelIndex + 1) % modelPaths.size();
        modelLoading = true;

        modelLoadFuture = std::async(std::launch::async, [path = modelPaths[currentModelIndex], &js]() ->std::unique_ptr<Model> {
          std::cout << "Iniciando carga del modelo: " << path << std::endl;

          auto tempModel = std::make_unique<Model>(path, js);
          if (!tempModel)
            std::cerr << "Error: tempModel es nulo. " << std::endl;
          else
          {
            std::cout << "Modelo cargado correctamente." << std::endl;
            std::cout << "Numero de mallas procesadas: " << tempModel->meshes.size() << std::endl;
          }

          std::cout << "Terminando lambda de std::async para: " << path << std::endl;
          return tempModel;
          });
      }
    }
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
    view = glm::translate(view, glm::vec3(0.0f, -2.0f, -15.0f));
    projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 500.f);

    glEnable(GL_TEXTURE_2D);
    GLuint model_loc = glGetUniformLocation(program.get_id(), "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint view_loc = glGetUniformLocation(program.get_id(), "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projection_loc = glGetUniformLocation(program.get_id(), "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    if (modelLoading) {
      modelLoadFuture.wait();
      modelLoading = false;
      currentModel = std::move(modelLoadFuture.get());

      currentModel->finalizeModel(); // Asegura que el modelo está listo para usar OpenGL
    }
    if (!modelLoading) {
      if (currentModel)
        currentModel->Draw(program);
      //normalmodel.Draw(program);
      if (backmodel)
        backmodel->Draw(program);
    }

    window->render();
  }
  js.~JobSystem();
  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}
