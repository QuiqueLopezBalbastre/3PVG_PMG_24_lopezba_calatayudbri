#include <GLFW/glfw3.h>
#include <stdio.h>

#include "WindowSystem.hpp"


std::optional<WindowSystem> WindowSystem::make() {
    WindowSystem ws;
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return std::nullopt;
    }
    ws.initialized = true;
    return std::optional<WindowSystem>{std::move(ws)};
}

WindowSystem::~WindowSystem() {
    if (initialized) {
        glfwTerminate();
    }
}

WindowSystem::WindowSystem(WindowSystem&& other) noexcept
    : initialized(other.initialized) {
    other.initialized = false;  // Transfer ownership
}

WindowSystem& WindowSystem::operator=(WindowSystem&& other) noexcept {
    if (this != &other) {
        if (initialized) {
            glfwTerminate();
        }
        initialized = other.initialized;
        other.initialized = false;
    }
    return *this;
}