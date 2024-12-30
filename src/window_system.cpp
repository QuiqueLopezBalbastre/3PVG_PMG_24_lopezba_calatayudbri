#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Window_System.hpp"



std::optional<WindowSystem> WindowSystem::make() {
  WindowSystem ws;
  if (!glfwInit()) {
    return std::nullopt;
  }
  return std::optional<WindowSystem>{ws};
}

WindowSystem::~WindowSystem() {
  glfwTerminate();
}