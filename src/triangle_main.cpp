#include "window_system.hpp"
#include "figure.hpp"
#include "Window.hpp"


int WinMain(int argc, char** argv) {
  //auto ws = WindowSystem::make();

    glfwInit();
  auto window = Window::make(640, 480,"LUQUI");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();

  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
     Figure triangle;
    window->renderFigure(triangle);

  }

  //if (-1 == window.destroyWindow()) {
  //  return -1;
  //}

  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}

