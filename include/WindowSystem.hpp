#pragma once

#ifndef __WINDOW_SYSTEM__ 
#define __WINDOW_SYSTEM__ 1

#include <optional>



class WindowSystem {
public:
  static std::optional<WindowSystem> make();  // Inicializador
  ~WindowSystem();                            // Destructor
  WindowSystem(WindowSystem&);
private:
  WindowSystem() = default;                   // Constructor
  WindowSystem(const WindowSystem&) = delete; // Constructor de copia
  WindowSystem* ws;
};

#endif // !__WINDOW_SYSTEM__ 1