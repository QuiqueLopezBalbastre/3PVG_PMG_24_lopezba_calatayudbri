#ifndef __COMMON__
#define __COMMON__ 

#include <GL/glew.h>
#include <iostream>
#define OPENGL_CHECK do{std::cout << "OpenGL " << __FILE__ << __LINE__ << " "<< glErrorString(glGetError()) << std::endl;} while(false)

static const char* glErrorString(GLenum err) {
  switch (err) {
  case GL_NO_ERROR:
    return "GL_NO_ERROR";
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_STACK_OVERFLOW:
    return "GL_STACK_OVERFLOW";
  case GL_STACK_UNDERFLOW:
    return "GL_STACK_UNDERFLOW";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  default:
    return "Unknown error";
  }
}
struct Vec2 {
  float x, y;
  Vec2 operator+(const Vec2& other) const {
    return { x + other.x, y + other.y};
  }

  Vec2 operator*(float scalar) const {
    return { x * scalar, y * scalar};
  }

  Vec2& operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vec2& operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
};
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
#endif // !__COMMON__1