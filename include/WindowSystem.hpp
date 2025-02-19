#pragma once

#ifndef __WINDOW_SYSTEM__ 
#define __WINDOW_SYSTEM__ 1

#include <optional>



class WindowSystem {
public:
    // Factory method for creating window system
    static std::optional<WindowSystem> make();

    // Destructor
    ~WindowSystem();

    // Move constructor (instead of copy)
    WindowSystem(WindowSystem&& other) noexcept;
    // Move assignment
    WindowSystem& operator=(WindowSystem&& other) noexcept;

private:
    WindowSystem() = default;
    // Delete copy constructor and assignment
    WindowSystem(const WindowSystem&) = delete;
    WindowSystem& operator=(const WindowSystem&) = delete;

    // Remove this pointer as it's not needed
    // WindowSystem* ws;  

    // Add a flag to track initialization
    bool initialized = false;
};

#endif // !__WINDOW_SYSTEM__ 1