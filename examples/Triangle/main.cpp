#include "WindowSystem.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Figure.hpp"


int WinMain(int argc, char** argv) {
  //auto ws = WindowSystem::make();

  glfwInit();
  auto window = Window::make(640, 480, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();

  std::vector<Figure::Vec2> customVertices = {
        { -0.3f, -0.3f },
        { 0.3f, -0.3f },
        { 0.0f, 0.6f }
  };

//printf("Number of threads: %d", std::thread::hardware_concurrency());
Figure triangle(customVertices);
triangle.setOffset({ 1.0f, 1.0f });

static Input input(window->window);
/* Loop until the user closes the window */
while (!window->isOpen())
{
  /*offset.x += 0.001f;
  offset.y -= 0.001f;
  triangle.setOffset(offset);*/
  triangle.moveFigure(input);
  //window->handleInput(triangle);
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

