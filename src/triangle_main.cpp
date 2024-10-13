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

  std::vector<Figure::Vec2> customVertices = {
        { -0.3f, -0.3f },
        { 0.3f, -0.3f },
        { 0.0f, 0.6f }
  };

  Figure triangle(customVertices);

  Figure::Vec2 offset = { 0.0f, 0.0f };

  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
      /*offset.x += 0.001f;
      offset.y -= 0.001f;
      triangle.setOffset(offset);*/

    window->handleInput(triangle);
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

