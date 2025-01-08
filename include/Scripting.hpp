#ifndef __SCRIPTING__
#define __SCRIPTING__

#include "lua.hpp"
#include <string>
#include <filesystem>
#include <memory>

class LuaScript {
public:

  LuaScript(std::string filepath);
  ~LuaScript() = default;
  LuaScript& operator=(const LuaScript& other);

  void check(int error);
  std::string getContent();
  void run(const std::string& str);

private:
  std::unique_ptr<lua_State, decltype(&lua_close)> state_;
  lua_State* s_;
  std::string content;
  std::string readFileContent(const std::filesystem::path& filepath);
};

#endif // !__SCRIPTING__
