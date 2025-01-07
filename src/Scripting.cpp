#include "Scripting.hpp"
#include <fstream>

LuaScript::LuaScript() : state_{ luaL_newstate() , &lua_close }, s_{ state_.get() } {}

void LuaScript::check(int error) {
  if (error != LUA_OK) {
    std::string err = lua_tostring(s_, lua_gettop(s_));
    lua_pop(s_, lua_gettop(s_));
    throw std::runtime_error("Lua error: " + err);
  }
}
void LuaScript::run(const std::string& str) {
  luaL_loadstring(s_, str.c_str());
  check(lua_pcall(s_, 0, 0, 0));
  lua_pop(s_, lua_gettop(s_));
}

std::string LuaScript::readFileContent(const std::filesystem::path& filepath) {
  std::ifstream file(filepath.c_str());
  using if_str = std::istreambuf_iterator<char>;
  std::string s(if_str{ file }, if_str{});
  file.close();
  return s;
}