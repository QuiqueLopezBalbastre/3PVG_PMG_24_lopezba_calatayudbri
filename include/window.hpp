#include <GLFW/glfw3.h>

class Window {
public:
  Window() {}
  ~Window() {}
  int initWindow() {}
  int initWindow(unsigned int window_width, unsigned int window_height) {}
  void setCurrentWindowActive() {}
  void render() {}
  bool closeWindow() {}
  int destroyWindow() {}
  GLFWwindow* window;
private:
};