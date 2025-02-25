#ifndef __SYSTEM__
#define __SYSTEM__ 1

#include <GL/glew.h>
#include "Component.hpp"
#include "ECS/ECSManager.hpp"

class RenderSystem {
public:
  RenderSystem();
  void update(const std::vector<TransformComponent>& transforms, const std::vector<RenderComponent>& renders);
  void drawShape(const TransformComponent* transform, const ShapeComponent* shape);
  void drawModel(const TransformComponent* transform, const RenderComponent* model, Program &program);
  static void UpdateTransformMatrix(TransformComponent &transform);
  void RenderScene(ECSManager& ecsmanager, Program& program, glm::mat4x4& model, glm::mat4x4& view, glm::mat4x4& projection);
private:
  void renderEntity(Entity entity, const TransformComponent& transform, const RenderComponent& render);
};
class AnimationSystem {
public:
  void AnimationSystem::update(TransformComponent* transform, AnimationComponent* anim, float deltaTime);

};
class InputSystem {
public:
  //void update(InputComponent* inputComponent, TransformComponent* transform, Input& input);
  void update(InputComponent* input, CameraComponent* camera, Input& inputManager, float deltaTime);
};
#endif // !__SYSTEMS__
