#ifndef __LUQUI_IMGUI__
#define __LUQUI_IMGUI__ 1


#include "window.hpp"
#include "ECS/Component.hpp"
#include "ECS/ECSManager.hpp"

class LuquiImgui {
public:
  LuquiImgui(Window* window);
  ~LuquiImgui();

  void NewFrame();

  // Creates all entity-related windows
  void EntityWindows(ECSManager& ecsManager);

  // Entity selector window
  void EntitySelector(ECSManager& ecsManager);

  // Entity properties window
  void EntityProperties(ECSManager& ecsManager);

  // Camera properties window
  void CameraProperties(CameraComponent& camera);

  // Getter and setter for selected entity
  Entity GetSelectedEntity() const;
  void SetSelectedEntity(Entity entity);

  // Render ImGui windows
  void Render();

private:
  Entity selectedEntity;
};

#endif // !__LUQUI_IMGUI__