#ifndef __SCRIPTING__
#define __SCRIPTING__

#include "lua.hpp"
#include <string>
#include <filesystem>
#include <memory>

class LuaScript {
public:

  LuaScript();

  void check(int error);

  void run(const std::string& str);

private:
  std::unique_ptr<lua_State, decltype(&lua_close)> state_;
  lua_State* s_;
  std::string readFileContent(const std::filesystem::path& filepath);
};

#endif // !__SCRIPTING__
