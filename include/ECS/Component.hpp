#ifndef __COMPONENT__
#define __COMPONENT__ 1

#include <string>
#include <unordered_map>
#include <functional>
#include "Entity.hpp"
#include "Input.hpp"

#define M_PI 3.141592f

struct Vec3 {
  float x, y, z;
  Vec3 operator+(const Vec3& other) const {
    return { x + other.x, y + other.y, z + other.z };
  }

  Vec3 operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
  }

  Vec3& operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  Vec3& operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
};
struct Vec4 {
  float x, y, z, w;
  Vec4 operator+(const Vec4& other) const {
    return { x + other.x, y + other.y, z + other.z, w + other.w };
  }

  Vec4 operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar , w * scalar };
  }

  Vec4& operator+=(const Vec4& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }
};
struct ComponentBase {
  bool active;
};

struct TransformComponent : ComponentBase {
  Vec3 position;
  Vec3 rotation;
  Vec3 scale;
};
struct RenderComponent : ComponentBase {
  std::string meshName;
  std::string textureName;
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
// CameraComponent
// PhysicsComponent
// AudioComponent
// ParticleComponent
struct ScriptComponent : ComponentBase {
  std::shared_ptr<Script*> script;
};
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
  Vec3 translation;  // Movimiento por frame (x, y, z)
  Vec3 rotation;     // Rotación por frame (pitch, yaw, roll)
  Vec3 scale;        // Escalado por frame (x, y, z)
  float duration;    // Duración de la animación en segundos
  float elapsedTime; // Tiempo transcurrido desde el inicio
};
// MaterialComponent
struct ScriptComponent : ComponentBase {
  std::function<void(float)> update;
};

#endif // !__COMPONENT__
