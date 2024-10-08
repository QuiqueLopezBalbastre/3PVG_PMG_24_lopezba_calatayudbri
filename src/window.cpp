#include "Window.hpp"
#include <stdio.h>


std::optional<Window> Window::make(int window_width, int window_height, const char* window_name)
{
  GLFWwindow* w = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);
  
  return std::optional<Window>{w};
}

Window::Window(GLFWwindow* w) {
    window = w;
    
}

Window::~Window() {
  glfwDestroyWindow(window);
}

void Window::setCurrentWindowActive() {
  glfwMakeContextCurrent(window);
}

void Window::render() {
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Swap front and back buffers */
  glfwSwapBuffers(window);

  /* Poll for and process events */
  glfwPollEvents();
}

void Window::renderFigure(Figure& figure)
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // Create a Figure object and draw the triangle
    
    figure.drawFigure();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

int Window::destroyWindow()
{
	return 0;
}

bool Window::isOpen() {
  return glfwWindowShouldClose(window);
}

//int Window::destroyWindow() {
//  if (this == nullptr) {
//    printf("Window already closed\n");
//    return -1;
//  }
//  return 0;
//}