#ifndef __INPUT__
#define __INPUT__ 1

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <vector>
//#include "Window.hpp"    // Incluimos la clase Window

class Input {
public:
  // Definimos una enumeraci�n para las acciones del motor
  enum Key
  {
   
    KEY_UNKNOWN = -1,
    // letters
    KEY_A = 1,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    // numbers
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    // Fs
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    // movement
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    // others
    KEY_SPACE,
    KEY_TAB,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_ESCAPE,
  };
  enum Mouse_Key {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_WHEEL,
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

  // M�todos p�blicos para el usuario
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

  // Debouncing (evitar m�ltiples lecturas de teclas)
  bool wasKeyJustPressed(int key);

  int getCurrentlyPressedKey();

  // Inicializaci�n de eventos de teclado/rat�n y callbacks
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

  // Estado del scroll del rat�n
  double m_scrollOffset;

  // Tecla de estado para debouncing
  std::unordered_map<int, bool> keyWasPressed;

  // M�todos privados
  void setupKeyMapping();
  void setupActionMapping();

  // Callbacks para eventos
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
