#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>

#include "Window.hpp"
#include "Input.hpp"
#include "ECS/Component.hpp"
#include "ECS/System.hpp"
#include "ECS/ECSManager.hpp"
#include "Imgui.hpp"


void PrintShaderValues(Program program)
{
  GLint numUniforms = 0;
  glGetProgramiv(program.get_id(), GL_ACTIVE_UNIFORMS, &numUniforms);

  std::cout << "Uniforms activos en el shader:\n";
  for (GLint i = 0; i < numUniforms; i++) {
    char name[256];
    GLsizei length;
    GLint size;
    GLenum type;
    glGetActiveUniform(program.get_id(), i, sizeof(name), &length, &size, &type, name);
    std::cout << "  - " << name << " (size: " << size << ", type: " << type << ")\n";

    // Si es "ambientLight", obtenemos su valor y lo imprimimos
    if (std::strcmp(name, "LightType") == 0) {
      GLint location = glGetUniformLocation(program.get_id(), name);
      if (location != -1) {
        GLfloat ambientLight[3]; // Vec3
        glGetUniformfv(program.get_id(), location, ambientLight);
        std::cout << "    > LightType: ("
          << ambientLight[0] << ", "
          ")\n";
      }
    }

  }
}

int main() {
  glfwInit();

  auto window = Window::make(1280, 1040, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }
  srand((unsigned int)time(NULL));
  window->setCurrentWindowActive();

  // Declaramos un gestor de input asociado a la ventana en activo.
  Input input(window->window);


  /** Creating shaders */
  Shader vertex = Shader();
  if (!vertex.loadFromFile(Shader::ShaderType::kShaderType_Vertex, "../data/Shaders/vertex.vs")) {
    std::cerr << "Error al cargar el vertex shader desde archivo." << std::endl;
    return -2;
  }
  if (!vertex.compile()) {
    std::cerr << "Error al compilar el vertex shader." << std::endl;
    return -3;
  }

  Shader fragment = Shader();
  if (!fragment.loadFromFile(Shader::ShaderType::kShaderType_Fragment, "../data/Shaders/fragment.fs")) {
    std::cerr << "Error al cargar el fragment shader desde archivo." << std::endl;
    return -4;
  }
  if (!fragment.compile()) {
    std::cerr << "Error al compilar el fragment shader." << std::endl;
    return -5;
  }
  /** Creating Program */
  Program program = Program();
  program.attach(&vertex);
  program.attach(&fragment);
  if (!program.link()) {
    std::cout << "Error al linkar el programa" << std::endl;
    return -4;
  }

  ///////END OF PROGRAM & SHADERS/////

  // Creamos un gestor de entidades
  ECSManager ecsmanager;

  // Declaramos los sistemas que se utilizar�n en el programa.
  RenderSystem renderSystem;
  InputSystem inputSystem;

  // Crear listas de componentes
  ecsmanager.addComponentType<TransformComponent>();
  ecsmanager.addComponentType<InputComponent>();
  ecsmanager.addComponentType<RenderComponent>();
  ecsmanager.addComponentType<LightComponent>();
  ecsmanager.addComponentType<CameraComponent>();
  ecsmanager.addComponentType<NameComponent>();

  //Camera Entity
  Entity CameraEntity = ecsmanager.createEntity();
  ecsmanager.addComponent<CameraComponent>(CameraEntity);
  ecsmanager.editComponent<CameraComponent>(CameraEntity, [](CameraComponent& camera) {
    camera.position = glm::vec3(0.0f, 300.0f, 300.0f); // Nueva posición
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

  ecsmanager.addComponent<NameComponent>(CameraEntity);
  ecsmanager.editComponent<NameComponent>(CameraEntity, [](NameComponent& cameraName) {
    cameraName.name = "camera";
    });
  // Crear una entidad para la luz
  Entity lightEntity = ecsmanager.createEntity();

  ecsmanager.editComponent<LightComponent>(lightEntity, [](LightComponent& light) {
    light.type = LightType::Point; // Tipo de luz (Point Light)
    light.color = glm::vec3(1.0f, 0.0f, 0.0f); // Color de la luz (RGB)
    light.position = glm::vec3(20.0f, 100.0f, 0.0f); // Posición de la luz
    light.intensity = 2.0f; // Intensidad de la luz
    light.radius = 250.0f; // Radio de influencia de la luz
    });

  ecsmanager.addComponent<NameComponent>(lightEntity);
  ecsmanager.editComponent<NameComponent>(lightEntity, [](NameComponent& cameraName) {
    cameraName.name = "red pointlight 1 <3";
    });
  //ecsmanager.editComponent<LightComponent>(lightEntity, [](LightComponent& light) {
  //	light.type = LightType::Spot; // Tipo de luz (Spotlight)
  //	light.color = glm::vec3(1.0f, 1.0f, 1.0f); // Color de la luz (blanco)
  //	light.position = glm::vec3(0.0f, 800.0f, 500.0f); // Posición de la luz
  //	light.direction = glm::vec3(0.0f, -1.0f, -1.0f); // Dirección de la luz
  //	light.intensity = 2.0f; // Intensidad de la luz
  //	light.cutoff = glm::cos(glm::radians(12.5f)); // Ángulo de corte interior (12.5 grados)
  //	light.outerCutoff = glm::cos(glm::radians(17.5f)); // Ángulo de corte exterior (17.5 grados)
  //	});

  // Configurar la luz como Point Light
  Entity lightEntity2 = ecsmanager.createEntity();
  ecsmanager.editComponent<LightComponent>(lightEntity2, [](LightComponent& light) {
    light.type = LightType::Point; // Tipo de luz (Point Light)
    light.color = glm::vec3(0.0f, 1.0f, 0.0f); // Color de la luz (RGB)
    light.position = glm::vec3(0.0f, 100.0f, 0.0f); // Posición de la luz
    light.intensity = 2.0f; // Intensidad de la luz
    light.radius = 250.0f; // Radio de influencia de la luz
    });

  //ecsmanager.editComponent<LightComponent>(lightEntity2, [](LightComponent& light) {
  //	light.type = LightType::Ambient; // Tipo de luz (Point Light)
  //	light.color = glm::vec3(1.0f, 1.0f, 1.0f); // Color de la luz (blanco)
  //	light.position = glm::vec3(0.0f, 400.0f, 0.0f); // Posición de la luz
  //	light.intensity = 1.0f; // Intensidad de la luz
  //	light.radius = 0.0f; // Radio de influencia de la luz (point light)
  //	});

  auto mesh = std::make_shared<Model>("../data/Models/Alduin/Alduin.obj", "alduin");

  //ecsmanager.resources.push_back(nullptr);

  ecsmanager.resources.push_back(mesh);

  // Crear la primera entidad para el modelo 1
  Entity modelEntity1 = ecsmanager.createEntity();

  ecsmanager.editComponent<TransformComponent>(modelEntity1, [](TransformComponent& transform) {
    transform.position = { -200.0f, 0.0f, 0.0f }; // Posición del primer modelo
    transform.scale = { 1.0f, 1.0f, 1.0f };
    });

  ecsmanager.editComponent<RenderComponent>(modelEntity1, [&](RenderComponent& modelComp) {
    modelComp.model = mesh; // Cargar el primer modelo
    });
  // Crear la segunda entidad para el modelo 2
  Entity modelEntity2 = ecsmanager.createEntity();

  ecsmanager.editComponent<TransformComponent>(modelEntity2, [](TransformComponent& transform) {
    transform.position = { 200.0f, -300.0f, 50.0f }; // Posición del segundo modelo
    transform.scale = { 1.0f, 1.0f, 1.0f };

    transform.rotation = { 0.0f,45.0f,0.0f };
    });

  ecsmanager.editComponent<RenderComponent>(modelEntity2, [&](RenderComponent& modelComp) {
    modelComp.model = mesh; // Cargar el segundo modelo
    });
  //////////////////////////////////

  LuquiImgui LuquiImgui(&window.value());


  // Ciclo del juego
  while (!window->isOpen()) {

    LuquiImgui.NewFrame();

    float rotation = glm::radians((float)glfwGetTime() * 100.0f);
    //glEnable(GL_COLOR_BUFFER_BIT);
    for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity)
    {
      if (auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity))
      {
        transformOpt.value()->rotation.y = rotation;
        RenderSystem::UpdateTransformMatrix(*transformOpt.value());
      }
    }
    // Limpiar la pantalla
    glClearColor(0.4, 0.4, 0.4, 1.0);
    glFrontFace(GL_CCW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);


    LuquiImgui.EntityWindows(ecsmanager);
    //LuquiImgui.EntityProperties(ecsmanager);

    program.use();
    glm::mat4x4 model, view, projection;
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    // Gestion de camara
    auto cameraComponent = ecsmanager.getComponent<CameraComponent>(CameraEntity);
    auto inputComp = ecsmanager.getComponent<InputComponent>(CameraEntity);
    if (cameraComponent) {
      if (inputComp)
        inputSystem.update(inputComp.value(), cameraComponent.value(), input, 0.016f);

      //cameraComponent.value()->updatePosition(transformOpt.value());
      //cameraComponent.value()->updateForward(transformOpt.value());
      // Usa las matrices de la cámara del componente
      cameraComponent.value()->updateViewMatrix();
      cameraComponent.value()->updateProjectionMatrix();
      view = cameraComponent.value()->view;
      projection = cameraComponent.value()->projection;
    }

    for (Entity Light_entity = 1; Light_entity < ecsmanager.get_nextEntity(); ++Light_entity) {
      // Configurar la luz en el shader
      if (!ecsmanager.isEntityAlive(Light_entity)) continue;
      auto lightOpt = ecsmanager.getComponent<LightComponent>(Light_entity);
      if (!lightOpt.has_value()) continue;
      if (lightOpt && lightOpt.value()->type == LightType::Point) {
        //GLuint pointLightIntensityLoc = glGetUniformLocation(program.get_id(), "pointLightIntensity");
        program.setVec3("pointLightColor", lightOpt.value()->color);
        program.setVec3("pointLightPosition", lightOpt.value()->position);
        program.setFloat("pointLightRadius", lightOpt.value()->radius);
        program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
        //glUniform1f(pointLightIntensityLoc, lightOpt.value()->intensity);

      }

      if (lightOpt && lightOpt.value()->type == LightType::Ambient) {
        GLuint ambientLightColorLoc = glGetUniformLocation(program.get_id(), "ambientLight");
        GLuint ambientLightIntensityLoc = glGetUniformLocation(program.get_id(), "ambientIntensity");
        GLuint LightTypeLoc = glGetUniformLocation(program.get_id(), "LightType");

        glUniform3f(ambientLightColorLoc, lightOpt.value()->color.r, lightOpt.value()->color.g, lightOpt.value()->color.b);
        glUniform1f(ambientLightIntensityLoc, lightOpt.value()->intensity);
        glUniform1i(LightTypeLoc, static_cast<int>(lightOpt.value()->type));
      }

      if (lightOpt && lightOpt.value()->type == LightType::Spot) {
        GLuint spotlightColorLoc = glGetUniformLocation(program.get_id(), "spotlightColor");
        GLuint spotlightPositionLoc = glGetUniformLocation(program.get_id(), "spotlightPosition");
        GLuint spotlightDirectionLoc = glGetUniformLocation(program.get_id(), "spotlightDirection");
        GLuint spotlightIntensityLoc = glGetUniformLocation(program.get_id(), "spotlightIntensity");
        GLuint spotlightCutoffLoc = glGetUniformLocation(program.get_id(), "spotlightCutoff");
        GLuint spotlightOuterCutoffLoc = glGetUniformLocation(program.get_id(), "spotlightOuterCutoff");
        GLuint LightTypeLoc = glGetUniformLocation(program.get_id(), "LightType");

        glUniform3f(spotlightColorLoc, lightOpt.value()->color.r, lightOpt.value()->color.g, lightOpt.value()->color.b);
        glUniform3f(spotlightPositionLoc, lightOpt.value()->position.x, lightOpt.value()->position.y, lightOpt.value()->position.z);
        glUniform3f(spotlightDirectionLoc, lightOpt.value()->direction.x, lightOpt.value()->direction.y, lightOpt.value()->direction.z);
        glUniform1f(spotlightIntensityLoc, lightOpt.value()->intensity);
        glUniform1f(spotlightCutoffLoc, lightOpt.value()->cutoff);
        glUniform1f(spotlightOuterCutoffLoc, lightOpt.value()->outerCutoff);
        glUniform1i(LightTypeLoc, static_cast<int>(lightOpt.value()->type));
      }

      if (lightOpt && lightOpt.value()->type == LightType::Directional) {
        GLuint directionalLightColorLoc = glGetUniformLocation(program.get_id(), "directionalLightColor");
        GLuint directionalLightDirectionLoc = glGetUniformLocation(program.get_id(), "directionalLightDirection");
        GLuint directionalLightIntensityLoc = glGetUniformLocation(program.get_id(), "directionalLightIntensity");
        GLuint LightTypeLoc = glGetUniformLocation(program.get_id(), "LightType");

        glUniform3f(directionalLightColorLoc, lightOpt.value()->color.r, lightOpt.value()->color.g, lightOpt.value()->color.b);
        glUniform3f(directionalLightDirectionLoc, lightOpt.value()->direction.x, lightOpt.value()->direction.y, lightOpt.value()->direction.z);
        glUniform1f(directionalLightIntensityLoc, lightOpt.value()->intensity);
        glUniform1i(LightTypeLoc, static_cast<int>(lightOpt.value()->type));
      }
      // Renderizar todas las entidades
      for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity) {
        if (!ecsmanager.isEntityAlive(entity)) continue;
        if (ecsmanager.getComponent<LightComponent>(entity).has_value()) continue;
        // Obtener los componentes de la entidad
        auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity);
        auto modelOpt = ecsmanager.getComponent<RenderComponent>(entity);
        auto inputComponentOpt = ecsmanager.getComponent<InputComponent>(entity);

        if (transformOpt && modelOpt) {


          // Pasar la matriz de modelo al shader
          GLuint modelLoc = glGetUniformLocation(program.get_id(), "model");
          glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformOpt.value()->transform_matrix));

          // Dibujar el modelo
          renderSystem.drawModel(transformOpt.value(), modelOpt.value(), program);
        }




      }
      glBlendFunc(GL_ONE, GL_ONE);
      glDepthMask(false);
    }
    auto camera = ecsmanager.getComponent<CameraComponent>(CameraEntity).value();
    // Pasar las matrices de vista y proyección al shader
    GLuint viewLoc = glGetUniformLocation(program.get_id(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(program.get_id(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDepthMask(true);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    program.unuse();
    // Intercambiar buffers
    LuquiImgui.Render();
    window->render();
  }

  window->~Window();
  glfwTerminate();
  return 0;
}
