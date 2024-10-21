#include "Input.hpp"
#include <iostream>

Input::Input(GLFWwindow* window) : m_window(window), m_scrollOffset(0.0) {
  setupKeyMapping();
  setupActionMapping();
}

void Input::setupKeyMapping() {
  // Asignamos cada tecla del motor a su correspondiente tecla en GLFW
  keyMapping[KEY_W] = GLFW_KEY_W;
  keyMapping[KEY_A] = GLFW_KEY_A;
  keyMapping[KEY_S] = GLFW_KEY_S;
  keyMapping[KEY_D] = GLFW_KEY_D;
  keyMapping[KEY_SPACE] = GLFW_KEY_SPACE;
  keyMapping[KEY_LEFT] = GLFW_KEY_LEFT;
  keyMapping[KEY_RIGHT] = GLFW_KEY_RIGHT;
  keyMapping[KEY_UP] = GLFW_KEY_UP;
  keyMapping[KEY_DOWN] = GLFW_KEY_DOWN;
  keyMapping[KEY_ESCAPE] = GLFW_KEY_ESCAPE;
  keyMapping[KEY_TAB] = GLFW_KEY_ESCAPE;

  // Mapeo de teclas modificadoras
  modifierMapping[GLFW_MOD_SHIFT] = GLFW_KEY_LEFT_SHIFT;
  modifierMapping[GLFW_MOD_CONTROL] = GLFW_KEY_LEFT_CONTROL;
  modifierMapping[GLFW_MOD_ALT] = GLFW_KEY_LEFT_ALT;
}

void Input::setupActionMapping() {
  // Mapeamos acciones a teclas por defecto
  actionMapping[Action::MOVE_FORWARD] = keyMapping[KEY_W];
  actionMapping[Action::JUMP] = keyMapping[KEY_SPACE];
}

bool Input::isKeyPressed(int key) {
  int glfwKey = keyMapping[key];
  int state = glfwGetKey(m_window, glfwKey);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isKeyReleased(int key) {
  int glfwKey = keyMapping[key];
  int state = glfwGetKey(m_window, glfwKey);
  return state == GLFW_RELEASE;
}

bool Input::isMouseButtonPressed(int button) {
  int glfwButton = mouseButtonMapping[button];
  int state = glfwGetMouseButton(m_window, glfwButton);
  return state == GLFW_PRESS;
}

void Input::getMousePosition(double& xPos, double& yPos) {
  glfwGetCursorPos(m_window, &xPos, &yPos);
}

double Input::getMouseScroll() {
  return m_scrollOffset;
}

bool Input::isModifierPressed(int modifier) {
  int glfwModifier = modifierMapping[modifier];
  int state = glfwGetKey(m_window, glfwModifier);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

// Implementación de soporte para gamepads/joysticks
bool Input::isJoystickConnected(int joystickID) {
  return glfwJoystickPresent(joystickID);
}

const float* Input::getJoystickAxes(int joystickID, int& count) {
  return glfwGetJoystickAxes(joystickID, &count);
}

// Mapeo de acciones
void Input::mapAction(Action action, int key) {
  actionMapping[action] = key;
}

bool Input::isActionPressed(Action action) {
  return isKeyPressed(actionMapping[action]);
}

// Implementación de debouncing
bool Input::wasKeyJustPressed(int key) {
  bool currentlyPressed = isKeyPressed(key);
  if (currentlyPressed && !keyWasPressed[key]) {
    keyWasPressed[key] = true;
    return true;
  }
  else if (!currentlyPressed) {
    keyWasPressed[key] = false;
  }
  return false;
}

// Callbacks de teclado y scroll
void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    std::cout << "Key pressed: " << key << std::endl;
  }
  else if (action == GLFW_RELEASE) {
    std::cout << "Key released: " << key << std::endl;
  }
}

void Input::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  std::cout << "Mouse scroll: " << yoffset << std::endl;
  Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
  if (input) {
    input->m_scrollOffset = yoffset;
  }
}

// Inicialización de eventos de teclado y ratón
void Input::initializeCallbacks() {
  glfwSetKeyCallback(m_window, keyCallback);
  glfwSetScrollCallback(m_window, mouseScrollCallback);
}

