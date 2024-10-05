#include <GLFW/glfw3.h>
#include <optional>

class Window {
public:
   Window(GLFWwindow* glfwWindow);
  ~Window();
  static std::optional<Window> make(int window_width, int window_height, const char* window_name);
  void setCurrentWindowActive();
  void render();
  int destroyWindow();
  bool isOpen();

  GLFWwindow* window;
private:
};