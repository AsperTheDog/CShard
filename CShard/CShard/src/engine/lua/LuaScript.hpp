#pragma once
#include <lua.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

class LuaScript
{
public:
	LuaScript() = default;

	void commit(const std::string& source)
	{
		std::ifstream wf("pak/resources/scripts/" + source + ".lua", std::ios::in);
		if (!wf) {
			throw std::runtime_error("Could not find script");
		}
		std::ostringstream ss;
		ss << wf.rdbuf();
		this->source = ss.str();
		wf.close();
		name = source;
	}

	void execute(lua_State* L)
	{
		execLua(source, L);
	}
	
	std::string source;
	std::string name;
private:

	static bool execLua(const std::string& filename, lua_State* st)
	{
		int r = luaL_dostring(st, filename.c_str());
		if (r != LUA_OK)
		{
			throw std::runtime_error(lua_tostring(st, -1));
		}
		return r == LUA_OK;
	}

	static bool callLua(lua_State* st, uint32_t argnum, uint32_t retnum, uint32_t something)
	{
		int r = lua_pcall(st, argnum, retnum, something);
		if (r != LUA_OK)
		{
			throw std::runtime_error(lua_tostring(st, -1));
		}
		return r == LUA_OK;
	}
};
