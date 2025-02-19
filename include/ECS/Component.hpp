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
    float outerCutoff;
};

struct TransformComponent : ComponentBase {
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::mat4 transform_matrix;
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

struct CameraComponent : ComponentBase {
  glm::vec3 position{ 0.0f, 0.0f, 5.0f };
  glm::vec3 front{ 0.0f, 0.0f, -1.0f };    // Dirección a la que mira la cámara
  glm::vec3 up{ 0.0f, 1.0f, 0.0f };
  glm::vec3 right{ 1.0f, 0.0f, 0.0f };     // Vector derecho para strafing

  // Ángulos de Euler para la rotación
  float yaw{ -90.0f };   // Rotación horizontal (en grados)
  float pitch{ 0.0f };   // Rotación vertical (en grados)

  // Configuración de la cámara
  float movementSpeed{ 50.0f };      // Unidades por segundo
  float sprintMultiplier{ 2.0f };   // Multiplicador de velocidad al correr
  float mouseSensitivity{ 0.1f };   // Sensibilidad del ratón

  // Matrices de la cámara
  glm::mat4 view{ 1.0f };
  glm::mat4 projection{ 1.0f };

  // Propiedades de proyección
  float fov{ 90.0f };
  float aspectRatio{ 4.0f / 3.0f };
  float nearPlane{ 0.1f };
  float farPlane{ 1000.0f };

  void updateCameraVectors() {
    // Calcula el vector front basado en los ángulos de Euler
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Recalcula los vectores right y up
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
  }

  void updateViewMatrix() {
    view = glm::lookAt(position, position + front, up);
  }

  void updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
  }
};
// TO DO:
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
