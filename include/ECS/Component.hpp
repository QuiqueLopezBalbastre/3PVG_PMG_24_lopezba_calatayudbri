#ifndef __COMPONENT__
#define __COMPONENT__ 1

#include <string>
#include <unordered_map>
#include <functional>
#include "common.hpp"
#include "Input.hpp"
#include "Scripting.hpp"
#include "ModelLoader/Model.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using Entity = unsigned int;

#define M_PI 3.141592f


struct ComponentBase {
  bool active;
};

enum class LightType {
  Directional,
  Point,
  Spot,
  Ambient
};

struct LightComponent : ComponentBase
{
  LightType type;
  glm::vec3 color;
  glm::vec3 position;
  glm::vec3 direction;
  float intensity;
  float radius; // Para point lights y spotlight
  float cutoff; // Para spotlight
};

struct TransformComponent : ComponentBase {
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
};
struct RenderComponent : ComponentBase {

  std::shared_ptr<Model> model;
};
enum class ShapeType {
  Square,
  Triangle,
  Circle,
  Cube,
  Sphere
};

struct ShapeComponent : ComponentBase {
  ShapeType type; // Tipo de la forma (cuadrado, triángulo, etc.)
  std::vector<Vec3> vertices; // Puntos de la forma
  Vec4 color; // Color de la forma (RGBA)
};
namespace {
  ShapeComponent createSquare(float size, const Vec4& color) {
    return { true, ShapeType::Square, {
        {-size / 2, -size / 2, 0},
        { size / 2, -size / 2, 0},
        { size / 2,  size / 2, 0},
        {-size / 2,  size / 2, 0}
    }, color };
  }

  ShapeComponent createTriangle(float size, const Vec4& color) {
    return { true, ShapeType::Triangle, {
        {0, size / 2, 0},
        {-size / 2, -size / 2, 0},
        { size / 2, -size / 2, 0}
    }, color };
  }

  ShapeComponent createCircle(float radius, const Vec4& color, int segments = 32) {
    std::vector<Vec3> vertices;
    for (int i = 0; i < segments; ++i) {
      float angle = 2.0f * M_PI * i / segments;
      vertices.push_back({ radius * cos(angle), radius * sin(angle), 0 });
    }
    return { true, ShapeType::Circle, vertices, color };
  }
}
// TO DO:
struct CameraComponent : ComponentBase {
  // Propiedades de la cámara
  glm::vec3 position{ 0.0f, 0.0f, 5.0f };    // Posición por defecto
  glm::vec3 target{ 0.0f, 0.0f, 0.0f };      // Punto al que mira
  glm::vec3 up{ 0.0f, 1.0f, 0.0f };          // Vector up

  // Matrices de la cámara
  glm::mat4 view{ 1.0f };
  glm::mat4 projection{ 1.0f };

  // Propiedades de proyección
  float fov{ 45.0f };
  float aspectRatio{ 640.0f / 480.0f };
  float nearPlane{ 0.1f };
  float farPlane{ 100.0f };
  // Método para actualizar la matriz de vista
  void updateViewMatrix() {
    view = glm::lookAt(position, target, up);
  }

  // Método para actualizar la matriz de proyección
  void updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
  }
};
// PhysicsComponent
// AudioComponent
// ParticleComponent
struct InputComponent : ComponentBase {
  bool isMovingForward = false;
  bool isJumping = false;
  bool isShooting = false;
  int keyReceived;
  bool followingMouse = false;
  double mouseX = 0.0;
  double mouseY = 0.0;
};
struct AnimationComponent : ComponentBase {
  glm::vec3 translation;  // Movimiento por frame (x, y, z)
  glm::vec3 rotation;     // Rotación por frame (pitch, yaw, roll)
  glm::vec3 scale;        // Escalado por frame (x, y, z)
  float duration;    // Duración de la animación en segundos
  float elapsedTime; // Tiempo transcurrido desde el inicio
};
// MaterialComponent
struct ScriptComponent : ComponentBase {
  std::shared_ptr<LuaScript> script;
};
#endif // !__COMPONENT__
