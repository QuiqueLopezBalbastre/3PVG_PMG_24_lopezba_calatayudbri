#include "Scripting.hpp"
#include <fstream>
#include <iostream>

LuaScript::LuaScript(std::string filepath) :
  state_{ luaL_newstate() , &lua_close },
  s_{ state_.get() } {
  luaL_openlibs(state_.get()); // Cargar las bibliotecas estándar de Lua
  content = readFileContent(filepath);
  if (content.empty())
    std::cerr << "\nContenido del script vacio\n";
  else
    std::cerr << "\nScript con contenido\n";
}

LuaScript& LuaScript::operator=(const LuaScript& other) {
  if (this != &other) {
    // Crear un nuevo estado de Lua y copiar los datos relevantes
    state_ = std::unique_ptr<lua_State, decltype(&lua_close)>(luaL_newstate(), &lua_close);
    s_ = state_.get();
    // Aquí puedes cargar nuevamente el script si es necesario
    // No olvides incluir una función para clonar o copiar el estado de Lua
  }
  return *this;
}

void LuaScript::check(int error) {
  if (error != LUA_OK) {
    std::string err = lua_tostring(s_, lua_gettop(s_));
    lua_pop(s_, lua_gettop(s_));
    throw std::runtime_error("Lua error: " + err);
  }
}

std::string LuaScript::getContent()
{
  return content;
}

void LuaScript::run(const std::string& str) {
  int loadStatus = luaL_loadstring(s_, str.c_str());
  check(loadStatus);
  int callStatus = lua_pcall(s_, 0, 0, 0);
  check(callStatus);
  lua_pop(s_, lua_gettop(s_));
}

std::string LuaScript::readFileContent(const std::filesystem::path& filepath) {
  std::ifstream file(filepath.c_str());
  using if_str = std::istreambuf_iterator<char>;
  std::string s(if_str{ file }, if_str{});
  std::cout << "Content of the file (" << filepath << "): " << s << std::endl;
  return s;
}