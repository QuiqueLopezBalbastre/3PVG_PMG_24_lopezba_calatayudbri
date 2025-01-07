#ifndef __INPUT__
#define __INPUT__ 1

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <vector>
//#include "Window.hpp"    // Incluimos la clase Window

class Input {
public:
  // Definimos una enumeración para las acciones del motor
  enum Key {
  //TO DO: Ordenar las key por orden alfabético
    KEY_UNKNOWN = -1,
    KEY_A = 0,
    KEY_W,
    KEY_S,
    KEY_D,
    KEY_E,
    KEY_Q,
    KEY_Z, 
    KEY_X,
    KEY_SPACE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_ESCAPE,
    KEY_TAB,
  };
  enum Modifier_Key {
    MOD_KEY_SHIFT,
    MOD_KEY_CONTROL,
    MOD_KEY_ALT,
  };
  enum Action {
    MOVE_FORWARD,
    JUMP,
    SHOOT,
  };
  // Constructor que recibe una ventana personalizada (Window*)
  Input(GLFWwindow* window);

  // Métodos públicos para el usuario
  bool isKeyPressed(int key);
  bool isKeyReleased(int key);
  bool isMouseButtonPressed(int button);
  void getMousePosition(double& xPos, double& yPos);
  double getMouseScroll();
  bool isModifierPressed(int modifier);

  // Gamepad/joystick support
  bool isJoystickConnected(int joystickID);
  const float* getJoystickAxes(int joystickID, int& count);

  // Mapeo de acciones
  void mapAction(Action action, int key);
  bool isActionPressed(Action action);

  // Debouncing (evitar múltiples lecturas de teclas)
  bool wasKeyJustPressed(int key);

  int getCurrentlyPressedKey();

  // Inicialización de eventos de teclado/ratón y callbacks
  void initializeCallbacks();

private:
  // Ventana GLFW (obtenida desde la clase Window*)
  GLFWwindow* m_window;

  // Mapeo de teclas abstracto (Motor -> GLFW)
  std::unordered_map<int, int> keyMapping;
  std::unordered_map<int, int> mouseButtonMapping;
  std::unordered_map<int, int> modifierMapping;

  // Mapeo de acciones
  std::unordered_map<Action, int> actionMapping;

  // Estado del scroll del ratón
  double m_scrollOffset;

  // Tecla de estado para debouncing
  std::unordered_map<int, bool> keyWasPressed;

  // Métodos privados
  void setupKeyMapping();
  void setupActionMapping();

  // Callbacks para eventos
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
