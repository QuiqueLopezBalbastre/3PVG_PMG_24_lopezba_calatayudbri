#include "WindowSystem.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Figure.hpp"
#include "JobSystem.hpp"
#include <iostream>

int main(int argc, char** argv) {
  //auto ws = WindowSystem::make();

  glfwInit();
  auto window = Window::make(640, 480, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();

  std::vector<Figure::Vec2> customVertices = {
        { -0.3f, -0.3f },
        { 0.3f, -0.3f },
        { 0.0f, 0.6f }
  };
  JobSystem js = JobSystem(std::thread::hardware_concurrency());

  // Encolar trabajos de diferentes tipos y prioridades
  js.addJob(JobSystem::JobType::Render, JobSystem::JobPriority::High, []() {
    std::cout << "Trabajo de renderizado (Alta prioridad) completado.\n";
    });

  js.addJob(JobSystem::JobType::Physics, JobSystem::JobPriority::Medium, []() {
    std::cout << "Trabajo de fisica (Prioridad media) completado.\n";
    });

  js.addJob(JobSystem::JobType::AI, JobSystem::JobPriority::Low, []() {
    std::cout << "Trabajo de AI (Baja prioridad) completado.\n";
    });

  js.addJob(JobSystem::JobType::Render, JobSystem::JobPriority::Low, []() {
    std::cout << "Trabajo de renderizado (Baja prioridad) completado.\n";
    });
  //std::this_thread::sleep_for(std::chrono::seconds(1));
  Figure triangle(customVertices);


  static Input input(window->window);
  /* Loop until the user closes the window */
  while (!window->isOpen())
  {

    triangle.moveFigure(input);
    window->renderFigure(triangle);

  }
  js.~JobSystem();
  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}

