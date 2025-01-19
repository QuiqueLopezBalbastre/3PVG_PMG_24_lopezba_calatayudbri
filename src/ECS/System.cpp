#include "ECS/System.hpp"
#include <iostream>
#include <Input.hpp>

RenderSystem::RenderSystem()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fondo gris oscuro
  // Configurar la matriz de proyección (ortográfica para simplicidad)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  // Configurar la matriz de modelo/vista
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void RenderSystem::update(const std::vector<TransformComponent>& transforms, const std::vector<RenderComponent>& renders)
{
  if (transforms.size() != renders.size()) {
    std::cout << "Error: transforms y renders no tienen el mismo tamaño.";
    return;
  }

  for (size_t i = 0; i < renders.size(); ++i) {
    const TransformComponent& transform = transforms[i];
    const RenderComponent& render = renders[i];

    Entity entity = static_cast<Entity>(i);

    renderEntity(entity, transform, render);
  }
}

void RenderSystem::renderEntity(Entity entity, const TransformComponent& transform, const RenderComponent& render)
{
  std::cout << "Rendering entity " << entity << " at position ("
    << transform.position.x << ", " << transform.position.y << ", " << transform.position.z
    << ".\n";
}

void RenderSystem::drawModel(const TransformComponent* transform, const RenderComponent* model, Program program) {
    if (!model || !model->model) return;

    glPushMatrix();

    // Aplicar transformaciones
    glTranslatef(transform->position.x, transform->position.y, transform->position.z);
    glRotatef(transform->rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(transform->rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(transform->rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(transform->scale.x, transform->scale.y, transform->scale.z);

    // Dibujar el modelo
    model->model->Draw(program);

    glPopMatrix();
}


void RenderSystem::drawShape(const TransformComponent* transform, const ShapeComponent* shape) {
  glPushMatrix();

  // Aplicar transformaciones
  glTranslatef(transform->position.x, transform->position.y, transform->position.z);
  glRotatef(transform->rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(transform->rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(transform->rotation.z, 0.0f, 0.0f, 1.0f);
  glScalef(transform->scale.x, transform->scale.y, transform->scale.z);

  // Dibujar la forma
  glColor4f(shape->color.x, shape->color.y, shape->color.z, shape->color.w);
  glBegin(GL_POLYGON);
  for (const Vec3& vertex : shape->vertices) {
    glVertex3f(vertex.x, vertex.y, vertex.z);
  }
  glEnd();

  glPopMatrix();
}


void AnimationSystem::update(TransformComponent* transform, AnimationComponent* anim, float deltaTime) {
  if (anim->active) {

    // Actualizar tiempo de animación
    anim->elapsedTime += deltaTime;
    if (anim->elapsedTime > anim->duration) {
      anim->elapsedTime = anim->duration; // Detener en el tiempo máximo
      return;
    }

    // Aplicar animación al transform
    transform->position += anim->translation * deltaTime;
    transform->rotation += anim->rotation * deltaTime;
    transform->scale += anim->scale * deltaTime;
  }

  //std::cout << "Animation in progress";
}

void InputSystem::update(InputComponent* inputComponent, TransformComponent* transform, Input& input) {
  if (inputComponent->active) {

    inputComponent->keyReceived = input.getCurrentlyPressedKey();
    // Gestiona el movimiento de la entidad
    if (inputComponent->followingMouse) {
      double mouseX, mouseY;
      input.getMousePosition(mouseX, mouseY);
      inputComponent->mouseX = mouseX;
      inputComponent->mouseY = mouseY;
      transform->position = { (float)inputComponent->mouseX, (float)inputComponent->mouseY, 0.0f };
    }
    else {
      if (inputComponent->keyReceived == Input::KEY_W) {
        transform->position += {0.0f, 0.25f * 0.016f, 0.0f};
      }
      if (inputComponent->keyReceived == Input::KEY_S) {
        transform->position -= {0.0f, 0.25f * 0.016f, 0.0f};
      }
      if (inputComponent->keyReceived == Input::KEY_D) {
        transform->position += {0.25f * 0.016f, 0.0f, 0.0f};
      }
      if (inputComponent->keyReceived == Input::KEY_A) {
        transform->position -= {0.25f * 0.016f, 0.0f, 0.0f};
      }
    }

    // Gestiona la rotación de la entidad en el eje Z
    if (inputComponent->keyReceived == Input::KEY_E) {
      transform->rotation -= {0.0f, 0.0f, 25.0f * 0.016f};
    }
    if (inputComponent->keyReceived == Input::KEY_Q) {
      transform->rotation += {0.0f, 0.0f, 25.0f * 0.016f};
    }
    if (inputComponent->keyReceived == Input::KEY_Z) {
      transform->scale += {0.25f * 0.016f, 0.25f * 0.016f, 0.0f};
    }
    if (inputComponent->keyReceived == Input::KEY_X) {
      transform->scale -= {0.25f * 0.016f, 0.25f * 0.016f, 0.0f};
    }
  }
}
