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

#endif // !__COMMON__1