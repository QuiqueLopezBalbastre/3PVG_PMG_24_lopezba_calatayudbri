#include "Window.hpp"


int WinMain(int argc, char** argv) {
  
  Window window;
  window.initWindow();

  window.setCurrentWindowActive();

  /* Loop until the user closes the window */
  while (!window.closeWindow())
  {
    window.render();
  }

  window.destroyWindow();
  return 0;
}

