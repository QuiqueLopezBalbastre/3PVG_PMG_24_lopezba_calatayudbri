#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>

#include "Window.hpp"
#include "Input.hpp"
#include "ECS/Component.hpp"
#include "ECS/System.hpp"
#include "ECS/ECSManager.hpp"


int main() {
  glfwInit();
  auto window = Window::make(1280, 1040, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }
  srand((unsigned int)time(NULL));
  window->setCurrentWindowActive();
  Input input(window->window);


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
    return -2;
  Shader fragment = Shader();
  fragment.loadSource(Shader::ShaderType::kShaderType_Fragment, kExampleFragmentShader, (unsigned int)strlen(kExampleFragmentShader));
  fragment.compile();
  if (!fragment.get_isCompiled())
    return -2;

  /** Creating Program */
  Program program = Program();
  program.attach(&vertex);
  program.attach(&fragment);
  if (!program.link()) {
    std::cout << "Error al linkar el programa" << std::endl;
    return -3;
  }
  program.use();


  // Creamos un gestor de entidades
  ECSManager ecsmanager;

  // Declaramos los sistemas que se utilizarán en el programa.
  RenderSystem renderSystem;
  AnimationSystem animationSystem;
  InputSystem inputSystem;

  // Crear listas de componentes
  ecsmanager.addComponentType<TransformComponent>();
  ecsmanager.addComponentType<ShapeComponent>();
  ecsmanager.addComponentType<AnimationComponent>();
  ecsmanager.addComponentType<InputComponent>();
  ecsmanager.addComponentType<ScriptComponent>();
  ecsmanager.addComponentType<RenderComponent>();
  ecsmanager.addComponentType<CameraComponent>();

  //Camera Entity
  Entity CameraEntity = ecsmanager.createEntity();
  ecsmanager.addComponent<CameraComponent>(CameraEntity);
  ecsmanager.editComponent<CameraComponent>(CameraEntity, [](CameraComponent& camera) {
    camera.position = glm::vec3(0.0f, 0.0f, 5.0f); // Nueva posición
    camera.updateViewMatrix(); // Actualizar la matriz de vista
    });
  ecsmanager.addComponent<InputComponent>(CameraEntity);

  ecsmanager.editComponent<InputComponent>(CameraEntity, [](InputComponent& input) {
    input.active = true;
    input.followingMouse = true;
    });
  ecsmanager.addComponent<TransformComponent>(CameraEntity);
  ecsmanager.editComponent<TransformComponent>(CameraEntity, [](TransformComponent& tr) {
    tr.position = { 0.0f, 0.0f, 5.0f };
    });


  //Model Entity
  Entity modelEntity = ecsmanager.createEntity();
  ecsmanager.editComponent<TransformComponent>(modelEntity, [](TransformComponent& transform) {
    transform.position = { 0.0f, 0.0f, 0.0f };
    transform.scale = { 1.0f, 1.0f, 1.0f };
    });

  ecsmanager.editComponent<RenderComponent>(modelEntity, [](RenderComponent& modelComp) {
    modelComp.model = std::make_shared<Model>("../data/Models/cube/cube.obj");
    });


  // Ciclo del juego
  while (!window->isOpen()) {
    ///Rendering//
    glEnable(GL_COLOR_BUFFER_BIT);

    ///* Render here */
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4x4 model, view, projection;
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);



    for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity) {
      if (!ecsmanager.isEntityAlive(entity)) continue;
      // Obtener referencias opcionales a los componentes
      auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity);
      auto animationOpt = ecsmanager.getComponent<AnimationComponent>(entity);
      // Actualizar animación si el componente existe
      if (transformOpt && animationOpt) {
        animationSystem.update(transformOpt.value(), animationOpt.value(), 0.01f);
      }

      // Dibujar la entidad si tiene transform y shape
      auto shapeOpt = ecsmanager.getComponent<ShapeComponent>(entity);
      if (transformOpt && shapeOpt) {
        renderSystem.drawShape(transformOpt.value(), shapeOpt.value());
      }


      //Dibujado del modelo//
      auto modelOpt = ecsmanager.getComponent<RenderComponent>(entity);
      if (transformOpt && modelOpt) {
        renderSystem.drawModel(transformOpt.value(), modelOpt.value(), program);
      }
      // Gestion de camara
      auto inputComponentOpt = ecsmanager.getComponent<InputComponent>(entity);
      auto cameraComponent = ecsmanager.getComponent<CameraComponent>(entity);
      if (cameraComponent) {
        if (inputComponentOpt)
          inputSystem.update(inputComponentOpt.value(), cameraComponent.value(), input, 0.016f);
        model = glm::mat4(1.0f);
        auto modelTransform = ecsmanager.getComponent<TransformComponent>(modelEntity);
        // Aplica las transformaciones del modelo como antes
        if (modelTransform) {
          model = glm::translate(model, modelTransform.value()->position);
          //model = glm::rotate(model, glm::radians((float)glfwGetTime() * 10.0f), glm::vec3(0, 1, 0));
          model = glm::scale(model, modelTransform.value()->scale);
        }

        //cameraComponent.value()->updatePosition(transformOpt.value());
        //cameraComponent.value()->updateForward(transformOpt.value());
        // Usa las matrices de la cámara del componente
        cameraComponent.value()->updateViewMatrix();
        cameraComponent.value()->updateProjectionMatrix();
        view = cameraComponent.value()->view;
        projection = cameraComponent.value()->projection;
      }
    }
    auto camera = ecsmanager.getComponent<CameraComponent>(CameraEntity).value();

    glEnable(GL_TEXTURE_2D);
    GLuint model_loc = glGetUniformLocation(program.get_id(), "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint view_loc = glGetUniformLocation(program.get_id(), "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projection_loc = glGetUniformLocation(program.get_id(), "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    window->render();
  }
  return 0;
}
