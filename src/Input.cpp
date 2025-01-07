#include "Input.hpp"
#include <iostream>

Input::Input(GLFWwindow *window) : m_window(window), m_scrollOffset(0.0)
{
  setupKeyMapping();
  setupActionMapping();
}

void Input::setupKeyMapping()
{
  // Asignamos cada tecla del motor a su correspondiente tecla en GLFW
  keyMapping[KEY_A] = GLFW_KEY_A;
  keyMapping[KEY_B] = GLFW_KEY_B;
  keyMapping[KEY_C] = GLFW_KEY_C;
  keyMapping[KEY_D] = GLFW_KEY_D;
  keyMapping[KEY_E] = GLFW_KEY_E;
  keyMapping[KEY_F] = GLFW_KEY_F;
  keyMapping[KEY_G] = GLFW_KEY_G;
  keyMapping[KEY_H] = GLFW_KEY_H;
  keyMapping[KEY_I] = GLFW_KEY_I;
  keyMapping[KEY_J] = GLFW_KEY_J;
  keyMapping[KEY_K] = GLFW_KEY_K;
  keyMapping[KEY_L] = GLFW_KEY_L;
  keyMapping[KEY_M] = GLFW_KEY_M;
  keyMapping[KEY_N] = GLFW_KEY_N;
  keyMapping[KEY_O] = GLFW_KEY_O;
  keyMapping[KEY_P] = GLFW_KEY_P;
  keyMapping[KEY_Q] = GLFW_KEY_Q;
  keyMapping[KEY_R] = GLFW_KEY_R;
  keyMapping[KEY_S] = GLFW_KEY_S;
  keyMapping[KEY_T] = GLFW_KEY_T;
  keyMapping[KEY_U] = GLFW_KEY_U;
  keyMapping[KEY_V] = GLFW_KEY_V;
  keyMapping[KEY_W] = GLFW_KEY_W;
  keyMapping[KEY_X] = GLFW_KEY_X;
  keyMapping[KEY_Y] = GLFW_KEY_Y;
  keyMapping[KEY_Z] = GLFW_KEY_Z;

  keyMapping[KEY_0] = GLFW_KEY_0;
  keyMapping[KEY_1] = GLFW_KEY_1;
  keyMapping[KEY_2] = GLFW_KEY_2;
  keyMapping[KEY_3] = GLFW_KEY_3;
  keyMapping[KEY_4] = GLFW_KEY_4;
  keyMapping[KEY_5] = GLFW_KEY_5;
  keyMapping[KEY_6] = GLFW_KEY_6;
  keyMapping[KEY_7] = GLFW_KEY_7;
  keyMapping[KEY_8] = GLFW_KEY_8;
  keyMapping[KEY_9] = GLFW_KEY_9;

  keyMapping[KEY_F1] = GLFW_KEY_F1;
  keyMapping[KEY_F2] = GLFW_KEY_F2;
  keyMapping[KEY_F3] = GLFW_KEY_F3;
  keyMapping[KEY_F4] = GLFW_KEY_F4;
  keyMapping[KEY_F5] = GLFW_KEY_F5;
  keyMapping[KEY_F6] = GLFW_KEY_F6;
  keyMapping[KEY_F7] = GLFW_KEY_F7;
  keyMapping[KEY_F8] = GLFW_KEY_F8;
  keyMapping[KEY_F9] = GLFW_KEY_F9;
  keyMapping[KEY_F10] = GLFW_KEY_F10;
  keyMapping[KEY_F11] = GLFW_KEY_F11;
  keyMapping[KEY_F12] = GLFW_KEY_F12;

  keyMapping[KEY_UP] = GLFW_KEY_UP;
  keyMapping[KEY_DOWN] = GLFW_KEY_DOWN;
  keyMapping[KEY_LEFT] = GLFW_KEY_LEFT;
  keyMapping[KEY_RIGHT] = GLFW_KEY_RIGHT;

  keyMapping[KEY_SPACE] = GLFW_KEY_SPACE;
  keyMapping[KEY_TAB] = GLFW_KEY_TAB;
  keyMapping[KEY_ENTER] = GLFW_KEY_ENTER;
  keyMapping[KEY_BACKSPACE] = GLFW_KEY_BACKSPACE;
  keyMapping[KEY_ESCAPE] = GLFW_KEY_ESCAPE;

  // Mapeo de teclas modificadoras
  modifierMapping[GLFW_MOD_SHIFT] = GLFW_KEY_LEFT_SHIFT;
  modifierMapping[GLFW_MOD_CONTROL] = GLFW_KEY_LEFT_CONTROL;
  modifierMapping[GLFW_MOD_ALT] = GLFW_KEY_LEFT_ALT;
}

void Input::setupActionMapping()
{
  // Mapeamos acciones a teclas por defecto
  actionMapping[Action::MOVE_FORWARD] = keyMapping[KEY_W];
  actionMapping[Action::JUMP] = keyMapping[KEY_SPACE];
}

bool Input::isKeyPressed(int key)
{
  int glfwKey = keyMapping[key];
  int state = glfwGetKey(m_window, glfwKey);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isKeyReleased(int key)
{
  int glfwKey = keyMapping[key];
  int state = glfwGetKey(m_window, glfwKey);
  return state == GLFW_RELEASE;
}

bool Input::isMouseButtonPressed(int button)
{
  int glfwButton = mouseButtonMapping[button];
  int state = glfwGetMouseButton(m_window, glfwButton);
  return state == GLFW_PRESS;
}

void Input::getMousePosition(double &xPos, double &yPos)
{
  glfwGetCursorPos(m_window, &xPos, &yPos);
}

double Input::getMouseScroll()
{
  return m_scrollOffset;
}

bool Input::isModifierPressed(int modifier)
{
  int glfwModifier = modifierMapping[modifier];
  int state = glfwGetKey(m_window, glfwModifier);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

// Implementaci�n de soporte para gamepads/joysticks
bool Input::isJoystickConnected(int joystickID)
{
  return glfwJoystickPresent(joystickID);
}

const float *Input::getJoystickAxes(int joystickID, int &count)
{
  return glfwGetJoystickAxes(joystickID, &count);
}

// Mapeo de acciones
void Input::mapAction(Action action, int key)
{
  actionMapping[action] = key;
}

bool Input::isActionPressed(Action action)
{
  return isKeyPressed(actionMapping[action]);
}

// Implementaci�n de debouncing
bool Input::wasKeyJustPressed(int key)
{
  bool currentlyPressed = isKeyPressed(key);
  if (currentlyPressed && !keyWasPressed[key])
  {
    keyWasPressed[key] = true;
    return true;
  }
  else if (!currentlyPressed)
  {
    keyWasPressed[key] = false;
  }
  return false;
}
int Input::getCurrentlyPressedKey()
{
  for (const auto &[key, glfwKey] : keyMapping)
  {
    if (isKeyPressed(key))
    {             // Utiliza la funci�n isKeyPressed
      return key; // Devuelve la primera tecla detectada como pulsada
    }
  }
  return KEY_UNKNOWN; // Si no hay ninguna tecla pulsada
}
// Callbacks de teclado y scroll
void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    std::cout << "Key pressed: " << key << std::endl;
  }
  else if (action == GLFW_RELEASE)
  {
    std::cout << "Key released: " << key << std::endl;
  }
}

void Input::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
  std::cout << "Mouse scroll: " << yoffset << std::endl;
  Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));
  if (input)
  {
    input->m_scrollOffset = yoffset;
  }
}

// Inicializaci�n de eventos de teclado y rat�n
void Input::initializeCallbacks()
{
  glfwSetKeyCallback(m_window, keyCallback);
  glfwSetScrollCallback(m_window, mouseScrollCallback);
}
