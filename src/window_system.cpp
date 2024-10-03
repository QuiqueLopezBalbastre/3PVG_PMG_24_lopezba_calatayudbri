#include <GLFW/glfw3.h>
#include <stdio.h>

#include "window_system.hpp"



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