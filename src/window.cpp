#include <stdio.h>
#include "Window.hpp"

Window::Window() {}
Window::~Window() {}

int Window::initWindow() {
  GLFWwindow* window;
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "LUQUI Engine", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  return 0;
}

int Window::initWindow(unsigned int window_width, unsigned int window_height) {
  GLFWwindow* window;
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(window_width, window_height, "LUQUI Engine", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  return 0;
}

void Window::setCurrentWindowActive() {
  glfwMakeContextCurrent(window);
}

void Window::render() {
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Swap front and back buffers */
  glfwSwapBuffers(window);

  /* Poll for and process events */
  glfwPollEvents();
}

bool Window::closeWindow() {
  return glfwWindowShouldClose(window);
}

int Window::destroyWindow() {
  if (this == nullptr) {
    printf("Window already closed\n");
    return -1;
  }
  glfwTerminate();
  return 0;
}