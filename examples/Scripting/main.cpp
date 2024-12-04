#include "WindowSystem.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Figure.hpp"
#include "JobSystem.hpp"
#include "lua.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

class LuaScript {
public:
  LuaScript() :state_{ luaL_newstate() , &lua_close }, s_{ state_.get() } {}
  void check(int error) {
    if (error != LUA_OK) {
      std::string err = lua_tostring(s_, lua_gettop(s_));
      lua_pop(s_, lua_gettop(s_));
      throw std::runtime_error("Lua error: " + err);
    }
  }
  void run(const std::string& str) {
    luaL_loadstring(s_, str.c_str());
    check(lua_pcall(s_, 0, 0, 0));
    lua_pop(s_, lua_gettop(s_));
  }
private:
  std::unique_ptr<lua_State, decltype(&lua_close)> state_;
  lua_State* s_;
};
std::string readFileContent(const std::filesystem::path& filepath) {
  std::ifstream file(filepath.c_str());
  using if_str = std::istreambuf_iterator<char>;
  std::string s(if_str{ file }, if_str{});
  file.close();
  return s;
}

int main() {
  try {
    LuaScript script;
    auto s = readFileContent("../src/script.lua");
    if (s.empty()) {
      throw std::runtime_error("El archivo Lua está vacío.");
    }
    std::cout << "Contenido del script Lua:\n" << s << std::endl;
    script.run(s);
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
/*
int main(int argc, char** argv) {
  //auto ws = WindowSystem::make();

  glfwInit();
  auto window = Window::make(1920, 1080, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }

  window->setCurrentWindowActive();


  static Input input(window->window);
  while (!window->isOpen())
  {

  }
  window->~Window();
  glfwTerminate();
  //ws->~WindowSystem();
  return 0;
}
*/

