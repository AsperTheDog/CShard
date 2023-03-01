#pragma once
#include <lua.hpp>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../AssetPath.hpp"

class LuaScript
{
public:
	LuaScript() = default;

	void commit(const AssetPath source)
	{
		this->source = source.path;
		this->name = source.path.filename().string();
		this->isTracked = source.isTracked;
	}

	void execute(lua_State* L)
	{
		execLua(source, L);
	}

	std::filesystem::path source;
	std::string name;
	bool isTracked = false;
private:

	static bool execLua(const std::filesystem::path& filename, lua_State* st)
	{
		//int r = luaL_dostring(st, filename.c_str());
		int r = luaL_dofile(st, filename.string().c_str());
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
