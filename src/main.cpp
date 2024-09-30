#include "Window.hpp"


int WinMain(int argc, char** argv) {
  
  Window window;
  if (-1 == window.initWindow()) {
    return -1;
  }

  window.setCurrentWindowActive();

  /* Loop until the user closes the window */
  while (!window.closeWindow())
  {
    window.render();
  }

  if (-1 == window.destroyWindow()) {
    return -1;
  }
  return 0;
}

