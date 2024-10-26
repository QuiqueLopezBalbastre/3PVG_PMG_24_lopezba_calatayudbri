#include "Window_System.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Mesh.hpp"

int main(int argc, char** argv) {
  auto ws = WindowSystem::make();

  auto window = Window::make(640, 480,"LUQUI");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();
  Mesh mesh;

  std::vector<Mesh> loadedMeshes = mesh.loadModel("../data/Minotaur_Male_Lores.fbx");
  
  static Input input(window->window);
  /* Loop until the user closes the window */
  while (!window->isOpen())
  {
     for (auto& m : loadedMeshes)
     {
       m.drawMesh();
     }
     window->render();
  }

  //if (-1 == window.destroyWindow()) {
  //  return -1;
  //}

  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}

