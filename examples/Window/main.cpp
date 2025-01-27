#include "WindowSystem.hpp"
#include "Window.hpp"

int main(int argc, char** argv) {
    // Create the window system first
    auto windowSystem = WindowSystem::make();
    if (!windowSystem) {
        // Handle initialization failure
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Now create the window using the initialized system
    auto window = Window::make(640, 480, "LUQUI black window");
    if (!window || window->window == nullptr) {
        printf("Failed to create window\n");
        return -1;
    }

    window->setCurrentWindowActive();

    // Main render loop
    while (!window->isOpen()) {
        window->render();
    }

    // The WindowSystem destructor will handle GLFW termination
    return 0;
}

