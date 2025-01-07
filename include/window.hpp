#ifndef __WINDOW__
#define __WINDOW__ 1


#include <GLFW/glfw3.h>
#include <optional>
#include "Figure.hpp"

class Window {
public:
  /** Métodos de clase especiales */
  Window(GLFWwindow* glfwWindow);
  ~Window();
  Window(const Window& other) = delete;
  Window(Window&& other) noexcept;
  Window& operator=(const Window& other) = delete;
  Window& operator=(Window& other);

  /** Métodos de clase */
  static std::optional<Window> make(int window_width, int window_height, const char* window_name);
  void setCurrentWindowActive();
  void render();
  void clear();
  void renderFigure(Figure& figure);
  void handleInput(Figure& figure);

  int destroyWindow();
  bool isOpen();

  /** Atributos */
  GLFWwindow* window;
private:
  bool destroy;
};
#endif // !__WINDOW__