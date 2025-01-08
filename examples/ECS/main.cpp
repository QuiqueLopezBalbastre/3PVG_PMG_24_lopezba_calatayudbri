#include <GL/glew.h>
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

  // Declaramos un gestor de input asociado a la ventana en activo.
  Input input(window->window);

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

  // Creamos una entidad que será gestionada por el usuario.
  Entity player = ecsmanager.createEntity();
  ecsmanager.editComponent<InputComponent>(player, [](InputComponent& input) {input.active = true; });
  ecsmanager.editComponent<ShapeComponent>(player, [](ShapeComponent& shape) {shape = createSquare(0.2f, { 0.0f, 1.0f, 0.0f, 1.0f }); });
  ecsmanager.editComponent<TransformComponent>(player, [](TransformComponent& transform)
    {transform.position = { 0.5f, 0.5f, 0.0f };
  transform.scale = { 1.0f, 1.0f, 0.0f }; });
  std::cout << "Player is entity: " << player;

  // Creamos entidades y modificamos sus componentes.
  for (int i = 0; i < 0; i++) {
    Entity entity = ecsmanager.createEntity();
    ecsmanager.editComponent<TransformComponent>(entity, [](TransformComponent& transform)
      {int x = rand() % 10;
    int y = rand() % 10;
    int dirx = rand() % 2;
    if (0 == dirx)
      dirx = -1;
    int diry = rand() % 2;
    if (0 == diry)
      diry = -1;
    transform.position.x = (float)x * 0.1f * (float)dirx;      // Modificar la posición X
    transform.position.y = (float)y * 0.1f * (float)diry;      // Modificar la posición Y
    transform.scale = { 1.0f, 1.0f, 2.0f }; });                // Cambiar el escalado})
    ecsmanager.editComponent<ShapeComponent>(entity, [](ShapeComponent& shape) {shape = createSquare(0.01f, { 1.0f,0.0f,0.0f,1.0f }); });
    ecsmanager.editComponent<AnimationComponent>(entity, [](AnimationComponent& anim)
      {
        anim.active = true;
        anim.duration = 60.0f;
        anim.translation = { 0.0f, 0.0f, 0.0f };
        anim.scale = { 0.0f , 0.0f, 0.0f };
        anim.rotation = { 0.0f, 0.0f, 25.0f };
      });
  }
    Entity scriptentity = ecsmanager.createEntity();
    ecsmanager.editComponent<ShapeComponent>(scriptentity, [](ShapeComponent& shape) {shape = createTriangle(0.1f, { 0.0f, 0.0f, 1.0f, 1.0f }); });
    ecsmanager.editComponent<TransformComponent>(scriptentity, [](TransformComponent& transform)
      {
        transform.position = { -0.5f, 0.5f, 0.0f };
        transform.scale = { 1.0f, 1.0f, 0.0f };
      });
    ecsmanager.editComponent<ScriptComponent>(scriptentity, [](ScriptComponent& script)
      {
        script.script = std::make_shared<LuaScript>("C:/Users/quilo/Documents/GitHub/3PVG_PMG_24_lopezba_calatayudbri/data/Scripts/HelloWorld.lua");
      });
    auto scriptcmp = ecsmanager.getComponent<ScriptComponent>(scriptentity);
    scriptcmp.value()->script->run(scriptcmp.value()->script->getContent());

  // Ciclo del juego
  while (!window->isOpen()) {
    window->clear();

    for (Entity entity = 0; entity < ecsmanager.get_nextEntity(); ++entity) {
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
      auto inputComponentOpt = ecsmanager.getComponent<InputComponent>(entity);
      if (inputComponentOpt)
        inputSystem.update(inputComponentOpt.value(), transformOpt.value(), input);
    }


    // Intercambiar buffers
    window->render();
  }

  window->~Window();
  glfwTerminate();
  return 0;
}
