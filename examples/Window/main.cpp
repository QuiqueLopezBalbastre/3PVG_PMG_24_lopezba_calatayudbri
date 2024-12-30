#include "Window_System.hpp"
#include "Window.hpp"

int WinMain(int argc, char** argv) {
  //auto ws = WindowSystem::make();

    glfwInit();
  auto window = Window::make(640, 480,"LUQUI black window");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();

  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
    window->render();
  }

  //if (-1 == window.destroyWindow()) {
  //  return -1;
  //}

  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}

